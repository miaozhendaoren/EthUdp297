/**
 * \file IfxDma_Dma.h
 * \brief DMA DMA details
 * \ingroup IfxLld_Dma
 *
 * \version iLLD_1_0_0_3_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Dma_Dma_Usage How to use the DMA Interface driver?
 * \ingroup IfxLld_Dma
 *
 * The DMA interface driver provides a default configuration for data moves without intervention of the CPU or other on chip devices.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework for different use cases.
 *
 * \section IfxLld_Dma_Dma_Preparation Preparation
 * \subsection IfxLld_Dma_Dma_Include Include Files
 *
 * Include following header file into your C code:
 *
 * \code
 *     #include <Dma/Dma/IfxDma_Dma.h>
 *     #include <Vadc/Adc/IfxVadc_Adc.h>
 *
 * \endcode
 *
 *
 * \subsection IfxLld_Dma_Dma_Module Module initialisation
 *
 * Declare the dma handle as a global variable:
 *
 * \code
 * // DMA handle
 * IfxDma_Dma dma;
 * \endcode
 *
 * Initialize the DMA with following code:
 * \code
 *     // create module config
 *     IfxDma_Dma_Config dmaConfig;
 *     IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);
 *
 *     // initialize module
 *     // IfxDma_Dma dma; // declared globally
 *     IfxDma_Dma_initModule(&dma, &dmaConfig);
 * \endcode
 *
 * This only has to be done once in the application.
 *
 * The "IfxDma_Dma dma" handle should either be declared as a global variable (as shown in this example),
 * or it can be created locally if desired:
 *
 * \code
 *     IfxDma_Dma dma;
 *     IfxDma_Dma_createModuleHandle(&dma, &MODULE_DMA);
 * \endcode
 *
 * \subsection IfxLld_Dma_Dma_Simple Memory-to-Memory Transfers
 *
 * A large amount of data should be copied between SRI based memories, e.g. from Flash into the DSPR
 * of the current CPU. It's recommended to use 256 bit moves for this purpose for best performance.
 *
 * This requires, that source and target locations are 256 bit (32 byte) aligned. With the GCC compiler
 * this can be achieved by adding __attribute__ ((aligned(64))) to the arrays:
 *
 * \code
 * #define MEMORY_TRANSFER_NUM_WORDS 1024
 * uint32 __attribute__ ((aligned(64))) memoryDestination[MEMORY_TRANSFER_NUM_WORDS];
 * \endcode
 *
 * Channel configuration and handling for the data move:
 * \code
 *     // construct the channel configuration
 *     IfxDma_Dma_ChannelConfig chnCfg;
 *     IfxDma_Dma_initChannelConfig(&chnCfg, &dma);
 *
 *     // select DMA channel which should be used
 *     chnCfg.channelId           = IfxDma_ChannelId_0;
 *
 *     // source and destination address
 *     chnCfg.sourceAddress       = (uint32)0x80000000; // somewhere in flash section, here: start of PFlash (only for demo)
 *     chnCfg.destinationAddress  = (uint32)memoryDestination;
 *
 *     // move size, transfer count and request/operation mode
 *     chnCfg.moveSize            = IfxDma_ChannelMoveSize_256bit;
 *     chnCfg.transferCount       = (4 * MEMORY_TRANSFER_NUM_WORDS) / 32; // e.g. 1024 words require 128 * 256 bit moves
 *     chnCfg.requestMode         = IfxDma_ChannelRequestMode_completeTransactionPerRequest;
 *     chnCfg.operationMode       = IfxDma_ChannelOperationMode_continuous;
 *
 *     // transfer configuration into DMA channel registers
 *     IfxDma_Dma_Channel chn;
 *     IfxDma_Dma_initChannel(&chn, &chnCfg);
 *
 *     // start transfer and wait until it's finished
 *     IfxDma_Dma_startChannelTransaction(&chn);
 *     while( IfxDma_Dma_isChannelTransactionPending(&chn) == TRUE );
 * \endcode
 *
 *
 * \subsection IfxLld_Dma_Dma_Peripheral Peripheral-to-Memory Transfers
 *
 * The content of 8 ADC result registers should be transfered to a memory location in DSPR whenever
 * an VADC autoscan has been finished. After the DMA transaction, an interrupt should be triggered
 * so that the CPU can process the conversion results.
 *
 * We use following global variables:
 * \code
 * // buffer for autoscanned conversion result values
 * #define NUM_SCANNED_CHANNELS  8
 * static uint16 vadcResultBuffer[NUM_SCANNED_CHANNELS];
 *
 * // VADC handle
 * IfxVadc_Adc vadc;
 *
 * // VADC group handle
 * static IfxVadc_Adc_Group adcGroup;
 *
 * // DMA channel handle
 * static IfxDma_Dma_Channel dmaChn;
 * \endcode
 *
 *
 * Create an interrupt handler for the DMA channel request:
 * \code
 * // priorities are normally defined in Ifx_IntPrioDef.h
 * #define IFX_INTPRIO_DMA_CH0  1
 *
 * IFX_INTERRUPT(dmaCh0ISR, 0, IFX_INTPRIO_DMA_CH0)
 * {
 *     // ...
 *     // do something with the conversion results in vadcResultBuffer[]
 *     // ...
 *
 *     // re-init DMA channel destination address
 *     IfxDma_Dma_setChannelDestinationAddress(&dmaChn, ADDR_CPU_DSPR(IfxCpu_getCoreId(), &vadcResultBuffer[0]));
 *
 *     // start new transaction
 *     IfxDma_Dma_setChannelTransferCount(&dmaChn, NUM_SCANNED_CHANNELS);
 *
 *     {
 *      uint32 channels = 0xff; // all 8 channels
 *      uint32 mask = 0xff; // modify the selection of all channels
 *
 *      // configure autoscan (single shot, not continuous scan)
 *      IfxVadc_Adc_setScan(&adcGroup, channels, mask);
 *     }
 * }
 * \endcode
 *
 * ADC configuration:
 * \code
 *     // create configuration
 *     IfxVadc_Adc_Config adcConfig;
 *     IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);
 *
 *     adcConfig.startupCalibration = TRUE;
 *
 *     // initialize module
 *     // IfxVadc_Adc vadc; // declared globally
 *     IfxVadc_Adc_initModule(&vadc, &adcConfig);
 *
 *     // create group config
 *     IfxVadc_Adc_GroupConfig adcGroupConfig;
 *     IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);
 *
 *     // initialize the group
 *     //IfxVadc_Adc_Group adcGroup; // defined globally
 *     adcGroupConfig.groupId = IfxVadc_GroupId_0;
 *     adcGroupConfig.master = adcGroupConfig.groupId;
 *
 *     // enable all arbiter request sources
 *     adcGroupConfig.arbiter.requestSlotQueueEnabled            = TRUE; // enable Queue mode
 *     adcGroupConfig.arbiter.requestSlotScanEnabled             = TRUE; // enable Scan mode
 *     adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled   = TRUE; // enable Background scan
 *
 *     // enable all gates in "always" mode (no edge detection)
 *     adcGroupConfig.queueRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;
 *     adcGroupConfig.scanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;
 *     adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;
 *
 *     IfxVadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
 *
 *     {
 *          // create channel config
 *          IfxVadc_Adc_ChannelConfig adcChannelConfig;
 *          IfxVadc_Adc_initChannelConfig(&adcChannelConfig, &adcGroup);
 *
 *          // initialize the channels
 *          for(int i=0; i<NUM_SCANNED_CHANNELS; ++i) {
 *           adcChannelConfig.channelId = (IfxVadc_ChannelId)i;
 *           adcChannelConfig.resultRegister = IfxVadc_ChannelResult_0;
 *
 *           // initialize the channel
 *           IfxVadc_Adc_Channel adcChannel;
 *           IfxVadc_Adc_initChannel(&adcChannel, &adcChannelConfig);
 *
 *           IfxVadc_Adc_configureWaitForReadMode(&adcChannel, TRUE);
 *          }
 *     }
 *     adcGroup.group->RCR[0].B.SRGEN = 1; // interrupt when new result is available
 *
 *     // send service request to DMA Channel 0
 *     IfxSrc_init((Ifx_SRC_SRCR*)&MODULE_SRC.VADC.G[0], IfxSrc_Tos_dma, 0);
 *     IfxSrc_enable((Ifx_SRC_SRCR*)&MODULE_SRC.VADC.G[0]);
 * \endcode
 *
 * And finally the DMA channel configuration
 * \code
 *     // create module config
 *     IfxDma_Dma_Config dmaConfig;
 *     IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);
 *
 *     // initialize module
 *     IfxDma_Dma dma;
 *     IfxDma_Dma_initModule(&dma, &dmaConfig);
 *
 *     {
 *         // construct the channel configuration
 *         IfxDma_Dma_ChannelConfig chnCfg;
 *         IfxDma_Dma_initChannelConfig(&chnCfg, &dma);
 *
 *         // select DMA channel which should be used
 *         chnCfg.channelId               = IfxDma_ChannelId_0;
 *         chnCfg.hardwareRequestEnabled  = TRUE; // will be triggered from VADC service request
 *
 *         // interrupt configuration
 *         chnCfg.channelInterruptEnabled       = TRUE; // service request from DMA after all words have been transfered
 *         chnCfg.channelInterruptPriority      = IFX_INTPRIO_DMA_CH0;
 *         chnCfg.channelInterruptTypeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreId());
 *
 *         // source and destination address
 *         chnCfg.sourceAddress                   = (uint32)&adcGroup.group->RES[0]; // first result register
 *         chnCfg.sourceCircularBufferEnabled     = TRUE;
 *         chnCfg.sourceAddressCircularRange      = IfxDma_ChannelIncrementCircular_1; // keep this address
 *
 *         chnCfg.destinationAddress              = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &vadcResultBuffer[0]); // move into result buffer
 *         chnCfg.destinationAddressIncrementStep = IfxDma_ChannelIncrementStep_1; // increment once (=2 bytes) with each write
 *
 *         // move size, transfer count and request/operation mode
 *         chnCfg.moveSize            = IfxDma_ChannelMoveSize_16bit;
 *         chnCfg.transferCount       = NUM_SCANNED_CHANNELS; // for the scanned channels
 *         chnCfg.requestMode         = IfxDma_ChannelRequestMode_oneTransferPerRequest;
 *         chnCfg.operationMode       = IfxDma_ChannelOperationMode_continuous; // hw request enable remains set after transaction
 *
 *
 *         // transfer configuration into DMA channel registers
 *         // IfxDma_Dma_Channel dmaChn; // declared globally
 *         IfxDma_Dma_initChannel(&dmaChn, &chnCfg);
 *
 *         // configure IRQ handler which will be called after all result registers have been transfered
 *         IfxCpu_Irq_installInterruptHandler(&dmaCh0ISR, IFX_INTPRIO_DMA_CH0);
 *
 *         // enable CPU interrupts
 *         IfxCpu_enableInterrupts();
 *     }
 * \endcode
 *
 * In order to start the initial channel conversions, use:
 * \code
 *     {
 *      uint32 channels = 0xff; // all 8 channels
 *      uint32 mask = 0xff; // modify the selection of all channels
 *
 *      // configure and start autoscan (single shot, not continuous mode)
 *      IfxVadc_Adc_setScan(&adcGroup, channels, mask);
 *     }
 * \endcode
 *
 * DMA will transfer the results to DSPR during the autoscan (whenever a new result is availale), and
 * invoke the dmaCh0ISR function once all channels have been converted.
 *
 * The ISR will re-configure the DMA channel and re-start the autoscan.
 *
 *
 * \subsection IfxLld_Dma_Dma_LinkedList Linked Lists
 *
 * Linked lists allow to initiate multiple DMA transactions from independent transaction sets which are
 * typically stored in a DSPR memory location, and fetched and executed from the DMA channel without
 * further CPU interaction.
 *
 * Following example demonstrates, how 5 different transactions can be initiated from a single request.
 * We copy the data of 5 CAN message objects to a DSPR location.
 *
 * Includes and global variables:
 * \code
 * #include <Dma/Dma/IfxDma_Dma.h>
 * #include <Scu/Std/IfxScuWdt.h>
 * #include <IfxCan_reg.h>
 *
 * // DMA channel handle
 * IfxDma_Dma_Channel chn;
 *
 * // Linked List storage
 * // IMPORTANT: it has to be aligned to an 64bit address, otherwise DMA can't read it
 * #define NUM_LINKED_LIST_ITEMS 5
 * __attribute__ ((aligned(64))) Ifx_DMA_CH linkedList[NUM_LINKED_LIST_ITEMS] ;
 *
 * // transfer these values to various CAN_MODATA[LH] registers via linked lists
 * #define NUM_TRANSFERED_WORDS 2
 * uint32 sourceBuffer[NUM_LINKED_LIST_ITEMS][NUM_TRANSFERED_WORDS];
 *
 * const uint32 destinationAddresses[NUM_LINKED_LIST_ITEMS] = {
 *     (uint32)&CAN_MODATAL0,
 *     (uint32)&CAN_MODATAL1,
 *     (uint32)&CAN_MODATAL2,
 *     (uint32)&CAN_MODATAL3,
 *     (uint32)&CAN_MODATAL4,
 * };
 * \endcode
 *
 * Following code to prepare CAN for this demo:
 * \code
 *     // enable CAN (no Ifx LLD available yet)
 *     {
 *         uint32 passwd = IfxScuWdt_getCpuWatchdogPassword();
 *         IfxScuWdt_clearCpuEndinit(passwd);
 *
 *         CAN_CLC.U = 0x0100;
 *         if( CAN_CLC.U ); // synch access
 *
 *         // select f_clc as kernel clock
 *         CAN_MCR.B.CLKSEL = 1;
 *
 *         // configure fractional divider
 *         CAN_FDR.U = 0x43ff;
 *
 *         // wait until RAM has been initialized
 *         while( CAN_PANCTR.B.BUSY );
 *
 *         IfxScuWdt_setCpuEndinit(passwd);
 *     }
 * \endcode
 *
 *
 * Build a linked list
 * \code
 *     // create module config
 *     IfxDma_Dma_Config dmaConfig;
 *     IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);
 *
 *     // initialize module
 *     IfxDma_Dma dma;
 *     IfxDma_Dma_initModule(&dma, &dmaConfig);
 *
 *     // initial channel configuration
 *     IfxDma_Dma_ChannelConfig cfg;
 *     IfxDma_Dma_initChannelConfig(&cfg, &dma);
 *
 *     // following settings are used by all transactions
 *     cfg.transferCount = NUM_TRANSFERED_WORDS;
 *     cfg.requestMode = IfxDma_ChannelRequestMode_completeTransactionPerRequest;
 *     cfg.moveSize = IfxDma_ChannelMoveSize_32bit;
 *     cfg.shadowControl = IfxDma_ChannelShadow_linkedList;
 *
 *     // generate linked list items
 *     for(int i=0; i<NUM_LINKED_LIST_ITEMS; ++i) {
 *         cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &sourceBuffer[i]);
 *         cfg.destinationAddress = destinationAddresses[i];
 *
 *         // address to next transaction set
 *         cfg.shadowAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)&linkedList[(i + 1) % NUM_LINKED_LIST_ITEMS]);
 *
 *         // transfer first transaction set into DMA channel
 *         if( i == 0 ) {
 *             IfxDma_Dma_initChannel(&chn, &cfg);
 *         }
 *
 *         // transfer into linked list storage
 *         IfxDma_Dma_initLinkedListEntry((void *)&linkedList[i], &cfg);
 *
 *         if( i == 0 ) {
 *             // - trigger channel interrupt once the first transaction set has been loaded (again) into DMA channel
 *             linkedList[i].CHCSR.B.SIT = 1;
 *         } else {
 *             // - activate SCH (transaction request) for each entry, expect for the first one (linked list terminated here)
 *             linkedList[i].CHCSR.B.SCH = 1;
 *         }
 *     }
 * \endcode
 *
 * The transfer can be started via software with:
 * \code
 *     // clear service request flag
 *     (IfxDma_Dma_getSrcPointer(&chn))->B.CLRR = 1;
 *
 *     // start linked list transaction
 *     IfxDma_Dma_startChannelTransaction(&chn);
 *
 *     // wait for service request which is triggered at the end of linked list transfers
 *     while( !(IfxDma_Dma_getSrcPointer(&chn))->B.SRR );
 * \endcode
 *
 * In order to synchronize with the end of linked list operations, it's recommended to poll the service request flag (triggered via linkedList[NUM_LINKED_LIST_ITEMS-1].CHCSR.B.SIT after the last word has been transfered), and not the transaction count as shown before, because a linked list will initiate multiple transactions.
 *
 * \defgroup IfxLld_Dma_Dma Dma
 * \ingroup IfxLld_Dma
 * \defgroup IfxLld_Dma_Dma_Data_Structures Data Structures
 * \ingroup IfxLld_Dma_Dma
 * \defgroup IfxLld_Dma_Dma_Module_Initialize Module Initialization
 * \ingroup IfxLld_Dma_Dma
 * \defgroup IfxLld_Dma_Dma_Channel_Initialize Channel Initialization
 * \ingroup IfxLld_Dma_Dma
 * \defgroup IfxLld_Dma_Dma_Linked_List Linked Lists
 * \ingroup IfxLld_Dma_Dma
 * \defgroup IfxLld_Dma_Dma_Channel_Transaction_Initiate Channel Transactions
 * \ingroup IfxLld_Dma_Dma
 */

#ifndef IFXDMA_DMA_H
#define IFXDMA_DMA_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxDma_cfg.h"
#include "Dma/Std/IfxDma.h"
#include "Cpu/Std/IfxCpu.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Dma_Dma_Data_Structures
 * \{ */
/** \brief DMA base address data structure (Module handle)
 */
typedef struct
{
    Ifx_DMA *dma;     /**< \brief Specifies the pointer to the DMA registers */
} IfxDma_Dma;

/** \} */

/** \addtogroup IfxLld_Dma_Dma_Data_Structures
 * \{ */
/** \brief Channel handle
 */
typedef struct
{
    Ifx_DMA         *dma;           /**< \brief Specifies the pointer to the DMA registers */
    IfxDma_ChannelId channelId;     /**< \brief Specifies the DMA channel */
    Ifx_DMA_CH      *channel;       /**< \brief Specifies the pointer to DMA channel registers */
} IfxDma_Dma_Channel;

/** \brief Configuration data structure of the channel
 */
typedef struct
{
    IfxDma_Dma                      *module;                                     /**< \brief Specifies pointer to the IfxDma_Dma module handle */
    IfxDma_ChannelId                 channelId;                                  /**< \brief Specifies the channel being used */
    uint32                           sourceAddress;                              /**< \brief Source address for the DMA channel */
    uint32                           destinationAddress;                         /**< \brief Destination address for the DMA channel */
    uint32                           shadowAddress;                              /**< \brief Initial content of shadow address for the DMA channel */
    uint32                           readDataCrc;                                /**< \brief Checksum for read data of the channel */
    uint32                           sourceDestinationAddressCrc;                /**< \brief Checksum for source and destination address of channel */
    uint16                           transferCount;                              /**< \brief Number of transfers in a transaction */
    IfxDma_ChannelMove               blockMode;                                  /**< \brief Number of moves in a transfer */
    IfxDma_ChannelRequestMode        requestMode;                                /**< \brief A service request initiates a single transfer, or the complete transaction */
    IfxDma_ChannelOperationMode      operationMode;                              /**< \brief keep enable/disable the hardware channel request after a transaction */
    IfxDma_ChannelMoveSize           moveSize;                                   /**< \brief Read/write data size */
    IfxDma_ChannelPattern            pattern;                                    /**< \brief Pattern selection operation modes */
    IfxDma_ChannelRequestSource      requestSource;                              /**< \brief Request of channel transfer through hardware or daisy chain. channel transfer complete interrupt of previous channel will trigger the next channel request */
    IfxDma_ChannelBusPriority        busPriority;                                /**< \brief Bus priority selection */
    boolean                          hardwareRequestEnabled;                     /**< \brief Enabling channel transaction via hardware request */
    IfxDma_ChannelIncrementStep      sourceAddressIncrementStep;                 /**< \brief Describes the address offset with which the source address should be modified after each move */
    IfxDma_ChannelIncrementDirection sourceAddressIncrementDirection;            /**< \brief Decides whether the source address offset after each move should be added or decremented from the exisiting address */
    IfxDma_ChannelIncrementCircular  sourceAddressCircularRange;                 /**< \brief Determines which part of the source address remains unchanged and therby not updated after each move */
    IfxDma_ChannelIncrementStep      destinationAddressIncrementStep;            /**< \brief Describes the address offset with which the destination address should be modified after each move */
    IfxDma_ChannelIncrementDirection destinationAddressIncrementDirection;       /**< \brief Decides whether the destination address offset after each move should be added or decremented from the exisiting address */
    IfxDma_ChannelIncrementCircular  destinationAddressCircularRange;            /**< \brief Determines which part of the destination address remains unchanged and therby not updated after each move */
    IfxDma_ChannelShadow             shadowControl;                              /**< \brief selects the shadow transfer mode */
    boolean                          sourceCircularBufferEnabled;                /**< \brief Enables/Disables the source circular buffering */
    boolean                          destinationCircularBufferEnabled;           /**< \brief Enables/Disables the destination circular buffering */
    boolean                          timestampEnabled;                           /**< \brief Enables/Disables the appendage of the time stamp after end of the last DMA move in a transaction */
    boolean                          wrapSourceInterruptEnabled;                 /**< \brief An interrupt should be triggered whenever source address is wrapped */
    boolean                          wrapDestinationInterruptEnabled;            /**< \brief An interrupt should be triggered whenever destination address is wrapped */
    boolean                          channelInterruptEnabled;                    /**< \brief The channel transfer interrupt should be triggered. See also channelInterruptControl */
    IfxDma_ChannelInterruptControl   channelInterruptControl;                    /**< \brief The channel transfer interrupt can either be triggered depending on the interruptRaiseThreshold, or each time the transaction count is decremented */
    uint8                            interruptRaiseThreshold;                    /**< \brief The value of the transferCount at which the interrupt should be raised */
    boolean                          transactionRequestLostInterruptEnabled;     /**< \brief Enables/Disables the channel transaction request lost interrupt */
    Ifx_Priority                     channelInterruptPriority;                   /**< \brief Priority of the channel interrupt trigger */
    IfxSrc_Tos                       channelInterruptTypeOfService;              /**< \brief Interrupt service provider */
} IfxDma_Dma_ChannelConfig;

/** \brief Configuration data structure of the Module
 */
typedef struct
{
    Ifx_DMA *dma;     /**< \brief Specifies the pointer to the DMA registers */
} IfxDma_Dma_Config;

/** \} */

/** \addtogroup IfxLld_Dma_Dma_Module_Initialize
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief initializes a DMA module handle based on the current configuration.
 * Can be used in code where it's ensure that the DMA module is already initialized, and a DMA handle isn't globally available.
 * \param dmaHandle pointer to the DMA module handle
 * \param dma pointer to the DMA registers
 * \return None
 *
 * \code
 *     IfxDma_Dma dma;
 *     IfxDma_Dma_createModuleHandle(&dma, &MODULE_DMA);
 * \endcode
 *
 */
IFX_EXTERN void IfxDma_Dma_createModuleHandle(IfxDma_Dma *dmaHandle, Ifx_DMA *dma);

/** \brief de-initialize the DMA module
 * \param dma pointer to the DMA module handle
 * \return None
 */
IFX_EXTERN void IfxDma_Dma_deInitModule(IfxDma_Dma *dma);

/** \brief Initialize the DMA module
 * \param dma pointer to the DMA module handle
 * \param config Pointer to configuration structure of the DMA module
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_EXTERN void IfxDma_Dma_initModule(IfxDma_Dma *dma, IfxDma_Dma_Config *config);

/** \brief initialize the DMA module configuration
 * \param config Pointer to configuration structure of the DMA module
 * \param dma pointer to the DMA registers
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_EXTERN void IfxDma_Dma_initModuleConfig(IfxDma_Dma_Config *config, Ifx_DMA *dma);

/** \} */

/** \addtogroup IfxLld_Dma_Dma_Channel_Initialize
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief initialize the DMA channel
 * \param channel pointer to the DMA base address and channel ID
 * \param config pointer to the DMA default channel configuration structure
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_EXTERN void IfxDma_Dma_initChannel(IfxDma_Dma_Channel *channel, IfxDma_Dma_ChannelConfig *config);

/** \brief initialize the DMA module channel configuration
 * \param config pointer to the DMA default channel configuration structure
 * \param dma pointer to the DMA module handle
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_EXTERN void IfxDma_Dma_initChannelConfig(IfxDma_Dma_ChannelConfig *config, IfxDma_Dma *dma);

/** \} */

/** \addtogroup IfxLld_Dma_Dma_Linked_List
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the SRC pointer for given DMA channel
 * \param channel pointer to the DMA base address and channel ID
 * \return SRC pointer for given DMA channel
 */
IFX_INLINE volatile Ifx_SRC_SRCR *IfxDma_Dma_getSrcPointer(IfxDma_Dma_Channel *channel);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief linked list functionality within the DMA module
 * \param ptrToAddress pointer to the memory location where the linked list entry should be stored
 * \param config pointer to the DMA default channel configuration structure
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_LinkedList
 *
 */
IFX_EXTERN void IfxDma_Dma_initLinkedListEntry(void *ptrToAddress, IfxDma_Dma_ChannelConfig *config);

/** \} */

/** \addtogroup IfxLld_Dma_Dma_Channel_Transaction_Initiate
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clear a channel transfer interrupt flag
 * \param channel pointer to the DMA base address and channel ID
 * \return None
 */
IFX_INLINE void IfxDma_Dma_clearChannelInterrupt(IfxDma_Dma_Channel *channel);

/** \brief Return and clear a channel transfer interrupt flag
 * The flag is automatically cleared with the call to this function
 * \param channel pointer to the DMA base address and channel ID
 * \return TRUE if the interrupt flag is set
 * FALSE if the interrupt flag is not set
 */
IFX_INLINE boolean IfxDma_Dma_getAndClearChannelInterrupt(IfxDma_Dma_Channel *channel);

/** \brief Return a channel transfer interrupt flag
 * \param channel pointer to the DMA base address and channel ID
 * \return TRUE if the interrupt flag is set
 * FALSE if the interrupt flag is not set
 */
IFX_INLINE boolean IfxDma_Dma_getChannelInterrupt(IfxDma_Dma_Channel *channel);

/** \brief Poll for an ongoing transaction
 * \param channel pointer to the DMA base address and channel ID
 * \return TRUE if a transaction request for the given channel is pending
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_INLINE boolean IfxDma_Dma_isChannelTransactionPending(IfxDma_Dma_Channel *channel);

/** \brief Re-initialize the destination address after a transaction
 * \param channel pointer to the DMA base address and channel ID
 * \param address is the Initial address of the destination pointer
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_INLINE void IfxDma_Dma_setChannelDestinationAddress(IfxDma_Dma_Channel *channel, uint32 address);

/** \brief Re-initialize the source address after a transaction
 * \param channel pointer to the DMA base address and channel ID
 * \param address is the Initial address of the source pointer
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_INLINE void IfxDma_Dma_setChannelSourceAddress(IfxDma_Dma_Channel *channel, uint32 address);

/** \brief Re-initialize the transfer count after a transaction
 * \param channel pointer to the DMA base address and channel ID
 * \param transferCount value holds the DMA transfers within a transaction (1..16383; 0 handled like 1 transaction)
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_INLINE void IfxDma_Dma_setChannelTransferCount(IfxDma_Dma_Channel *channel, uint32 transferCount);

/** \brief initiate the DMA move transaction
 * \param channel pointer to the DMA base address and channel ID
 * \return None
 *
 * See \ref IfxLld_Dma_Dma_Simple
 *
 */
IFX_INLINE void IfxDma_Dma_startChannelTransaction(IfxDma_Dma_Channel *channel);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE volatile Ifx_SRC_SRCR *IfxDma_Dma_getSrcPointer(IfxDma_Dma_Channel *channel)
{
    return IfxDma_getSrcPointer(channel->dma, channel->channelId);
}


IFX_INLINE void IfxDma_Dma_clearChannelInterrupt(IfxDma_Dma_Channel *channel)
{
    IfxDma_clearChannelInterrupt(channel->dma, channel->channelId);
}


IFX_INLINE boolean IfxDma_Dma_getAndClearChannelInterrupt(IfxDma_Dma_Channel *channel)
{
    return IfxDma_getAndClearChannelInterrupt(channel->dma, channel->channelId);
}


IFX_INLINE boolean IfxDma_Dma_getChannelInterrupt(IfxDma_Dma_Channel *channel)
{
    return IfxDma_getChannelInterrupt(channel->dma, channel->channelId);
}


IFX_INLINE boolean IfxDma_Dma_isChannelTransactionPending(IfxDma_Dma_Channel *channel)
{
    return IfxDma_isChannelTransactionPending(channel->dma, channel->channelId);
}


IFX_INLINE void IfxDma_Dma_setChannelDestinationAddress(IfxDma_Dma_Channel *channel, uint32 address)
{
    IfxDma_setChannelDestinationAddress(channel->dma, channel->channelId, (void *)address);
}


IFX_INLINE void IfxDma_Dma_setChannelSourceAddress(IfxDma_Dma_Channel *channel, uint32 address)
{
    IfxDma_setChannelSourceAddress(channel->dma, channel->channelId, (void *)address);
}


IFX_INLINE void IfxDma_Dma_setChannelTransferCount(IfxDma_Dma_Channel *channel, uint32 transferCount)
{
    IfxDma_setChannelTransferCount(channel->dma, channel->channelId, transferCount);
}


IFX_INLINE void IfxDma_Dma_startChannelTransaction(IfxDma_Dma_Channel *channel)
{
    channel->channel->CHCSR.U = 0;  // for linked lists: ensure that this memory location is initialized
    IfxDma_startChannelTransaction(channel->dma, channel->channelId);
}


#endif /* IFXDMA_DMA_H */
