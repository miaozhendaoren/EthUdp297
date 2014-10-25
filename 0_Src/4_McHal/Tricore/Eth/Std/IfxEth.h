/**
 * \file IfxEth.h
 * \brief ETH  basic functionality
 * \ingroup IfxLld_Eth
 *
 * \version iLLD_1_0_0_0_0
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
 * \defgroup IfxLld_Eth ETH
 * \ingroup IfxLld
 * \defgroup IfxLld_Eth_Std Standard Driver
 * \ingroup IfxLld_Eth
 * \defgroup IfxLld_Eth_Std_DataStructures Data Structures
 * \ingroup IfxLld_Eth_Std
 * \defgroup IfxLld_Eth_Std_Unions Unions
 * \ingroup IfxLld_Eth_Std
 * \defgroup IfxLld_Eth_Std_Configuration Configuration Functions
 * \ingroup IfxLld_Eth_Std
 * \defgroup IfxLld_Eth_Std_Utility Utility Functions
 * \ingroup IfxLld_Eth_Std
 * \defgroup IfxLld_Eth_Std_Initialisation Initialisation Functions
 * \ingroup IfxLld_Eth_Std
 */

#ifndef IFXET_H
#define IFXET_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxEth_cfg.h"
#include "Cpu/Std/Ifx_Types.h"
#include "IfxEth_reg.h"
#include "Src/Std/IfxSrc.h"
#include "_PinMap/IfxEth_PinMap.h"
#include "_Utilities/Ifx_Assert.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Size of one ethernet frame buffer
 */
#define IFXETH_RTX_BUFFER_SIZE   1536

#ifndef IFXETH_TX_BUFFER_BY_USER
#define IFXETH_TX_BUFFER_BY_USER 0
#endif

#ifndef IFXETH_RX_BUFFER_BY_USER
#define IFXETH_RX_BUFFER_BY_USER 0
#endif

/** \brief Rx buffers (ring mode)
 */
#define IFXETH_MAX_RX_BUFFERS    8

/** \brief Tx buffers (ring mode)
 */
#define IFXETH_MAX_TX_BUFFERS    16

/** \brief 4 DWORDS (16 bytes)
 */
#define IFXETH_DESCR_SIZE        4

/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

typedef enum
{
    IfxEth_ChecksumMode_bypass            = 0,
    IfxEth_ChecksumMode_ipv4              = 1,
    IfxEth_ChecksumMode_tcpUdpIcmpSegment = 2,
    IfxEth_ChecksumMode_tcpUdpIcmpFull    = 3
} IfxEth_ChecksumMode;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Structure for Alternate/Enhanced RX descriptor DWORD 0 Bit field access
 */
typedef struct
{
    uint32 ext : 1;    /**< \brief Extended Status Available/Rx MAC Address */
    uint32 CE : 1;     /**< \brief CRC Error */
    uint32 DBE : 1;    /**< \brief Dribble Bit Error */
    uint32 RE : 1;     /**< \brief Receive Error */
    uint32 RWT : 1;    /**< \brief Receive Watchdog Timeout */
    uint32 FT : 1;     /**< \brief Frame Type */
    uint32 LC : 1;     /**< \brief Late Collision */
    uint32 IPC : 1;    /**< \brief IPC Checksum Error/Giant Frame */
    uint32 LS : 1;     /**< \brief Last Descriptor */
    uint32 FS : 1;     /**< \brief First Descriptor */
    uint32 VLAN : 1;   /**< \brief VLAN Tag */
    uint32 OE : 1;     /**< \brief Overflow Error */
    uint32 LE : 1;     /**< \brief Length Error */
    uint32 SAF : 1;    /**< \brief Source Address Filter Fail */
    uint32 DE : 1;     /**< \brief Descriptor Error */
    uint32 ES : 1;     /**< \brief Error Summary, ES = PCE | CE | RE | RWT | LC | IPC | OE | DE */
    uint32 FL : 14;    /**< \brief Frame Length */
    uint32 AFM : 1;    /**< \brief Destination Address Filter Fail */
    uint32 OWN : 1;    /**< \brief Own Bit, 1 = own by DMA */
} IfxEth_AltRxDescr0_Bits;

/** \brief Structure for Alternate/Enhanced RX descriptor DWORD 1 Bit field access
 */
typedef struct
{
    uint32 RBS1 : 13;   /**< \brief Receive Buffer 1 Size */
    uint32 resv1 : 1;   /**< \brief reserved */
    uint32 RCH : 1;     /**< \brief Second Address Chained */
    uint32 RER : 1;     /**< \brief Receive End of Ring */
    uint32 RBS2 : 13;   /**< \brief Receive Buffer 2 Size */
    uint32 resv : 2;    /**< \brief reserved) */
    uint32 DIC : 1;     /**< \brief Disable Interrupt on Completion */
} IfxEth_AltRxDescr1_Bits;

/** \brief Structure for Alternate/Enhanced TX descriptor DWORD 0 Bit field access
 */
typedef struct
{
    uint32 DB : 1;      /**< \brief Deferred bit */
    uint32 UF : 1;      /**< \brief Underflow error */
    uint32 ED : 1;      /**< \brief Excessive deferral */
    uint32 CC : 4;      /**< \brief Collision count */
    uint32 VLAN : 1;    /**< \brief VLAN TAG */
    uint32 EC : 1;      /**< \brief Excessive Collision */
    uint32 LC : 1;      /**< \brief Late Collision */
    uint32 NC : 1;      /**< \brief No Carrier */
    uint32 LOC : 1;     /**< \brief Loss of Carrier */
    uint32 PCE : 1;     /**< \brief Payload Checksum Error */
    uint32 FF : 1;      /**< \brief Frame Flushed */
    uint32 JT : 1;      /**< \brief Jabber Timeout */
    uint32 ES : 1;      /**< \brief Error Summary, ES = JT | FF | LOC | NC | LC | EC | ED | UF */
    uint32 IHE : 1;     /**< \brief IP Header Error */
    uint32 TTSS : 1;    /**< \brief Transmit Time Stamp Status */
    uint32 resv : 2;    /**< \brief (reserved) */
    uint32 TCH : 1;     /**< \brief Second Address Chained */
    uint32 TER : 1;     /**< \brief Transmit End of Ring */
    uint32 CIC : 2;     /**< \brief Checksum Insertion Control */
    uint32 resv1 : 1;   /**< \brief (Reserved) */
    uint32 TTSE : 1;    /**< \brief Transmit Time Stamp Enable */
    uint32 DP : 1;      /**< \brief Disable Padding */
    uint32 DC : 1;      /**< \brief Disable CRC */
    uint32 FS : 1;      /**< \brief First Segment */
    uint32 LS : 1;      /**< \brief Last Segment */
    uint32 IC : 1;      /**< \brief Interrupt on Completion */
    uint32 OWN : 1;     /**< \brief Own Bit, 1 = own by DMA */
} IfxEth_AltTxDescr0_Bits;

/** \brief Structure for Alternate/Enhanced TX descriptor DWORD 1 Bit field access
 */
typedef struct
{
    uint32 TBS1 : 13;   /**< \brief Transmit Buffer 1 Size */
    uint32 resv1 : 3;   /**< \brief (reserved) */
    uint32 TBS2 : 13;   /**< \brief Transmit Buffer 2 Size */
    uint32 resv2 : 3;   /**< \brief (reserved) */
} IfxEth_AltTxDescr1_Bits;

/** \addtogroup IfxLld_Eth_Std_Unions
 * \{ */
/** \brief Union for RX descriptor DWORD 0
 */
typedef union
{
    IfxEth_AltRxDescr0_Bits A;     /**< \brief Structure for RX descriptor DWORD 0 Bit field access */
    uint32                  U;     /**< \brief Unsigned long access */
} IfxEth_RxDescr0;

/** \brief Union for RX descriptor DWORD 1
 */
typedef union
{
    IfxEth_AltRxDescr1_Bits A;     /**< \brief Structure for RX descriptor DWORD 1 Bit field access */
    uint32                  U;     /**< \brief unsigned long access */
} IfxEth_RxDescr1;

/** \brief Union for RX descriptor DWORD 2
 */
typedef union
{
    uint32 U;     /**< \brief unsigned long access */
} IfxEth_RxDescr2;

/** \brief Union for RX descriptor DWORD 3
 */
typedef union
{
    uint32 U;     /**< \brief unsigned long access */
} IfxEth_RxDescr3;

/** \brief Union for TX descriptor DWORD 0
 */
typedef union
{
    IfxEth_AltTxDescr0_Bits A;     /**< \brief Structure for TX descriptor DWORD 0 Bit field access */
    uint32                  U;     /**< \brief Unsigned long access */
} IfxEth_TxDescr0;

/** \brief Union for TX descriptor DWORD 1
 */
typedef union
{
    IfxEth_AltTxDescr1_Bits A;     /**< \brief Structure for RX descriptor DWORD 1 Bit field access */
    uint32                  U;     /**< \brief unsigned long access */
} IfxEth_TxDescr1;

/** \brief Union for TX descriptor DWORD 2
 */
typedef union
{
    uint32 U;     /**< \brief unsigned long access */
} IfxEth_TxDescr2;

/** \brief Union for TX descriptor DWORD 3
 */
typedef union
{
    uint32 U;     /**< \brief unsigned long access */
} IfxEth_TxDescr3;

/** \} */

/** \addtogroup IfxLld_Eth_Std_DataStructures
 * \{ */
/** \brief Normal RX descriptor
 */
typedef struct
{
    IfxEth_RxDescr0 RDES0;     /**< \brief RX descriptor DWORD 0 */
    IfxEth_RxDescr1 RDES1;     /**< \brief RX descriptor DWORD 1 */
    IfxEth_RxDescr2 RDES2;     /**< \brief RX descriptor DWORD 2 */
    IfxEth_RxDescr3 RDES3;     /**< \brief RX descriptor DWORD 3 */
} IfxEth_RxDescr;

/** \brief Normal TX descriptor
 */
typedef struct
{
    IfxEth_TxDescr0 TDES0;     /**< \brief TX descriptor DWORD 0 */
    IfxEth_TxDescr1 TDES1;     /**< \brief TX descriptor DWORD 1 */
    IfxEth_TxDescr2 TDES2;     /**< \brief TX descriptor DWORD 2 */
    IfxEth_TxDescr3 TDES3;     /**< \brief TX descriptor DWORD 3 */
} IfxEth_TxDescr;

/** \} */

/** \brief Port pins configuration
 */
typedef struct
{
    IfxEth_Crsdv_In   *crsDiv;     /**< \brief pointer to CRSDIV input pin configtring_here */
    IfxEth_Refclk_In  *refClk;     /**< \brief Pointer to REFCLK input pin config */
    IfxEth_Rxd_In     *rxd0;       /**< \brief Pointer to RXD0 input pin config */
    IfxEth_Rxd_In     *rxd1;       /**< \brief Pointer to RXD1 input pin config */
    IfxEth_Mdc_Out    *mdc;        /**< \brief Pointer to MDC output pin config */
    IfxEth_Mdio_InOut *mdio;       /**< \brief Pointer to MDIO pin config */
    IfxEth_Txd_Out    *txd0;       /**< \brief Pointer to TXD0 output pin config */
    IfxEth_Txd_Out    *txd1;       /**< \brief Pointer to TXD1 output pin config */
    IfxEth_Txen_Out   *txEn;       /**< \brief Pointer to TXEN output pin config */
} IfxEth_PortPins;

/** \addtogroup IfxLld_Eth_Std_DataStructures
 * \{ */
/** \brief ETH configuration structure
 */
typedef struct
{
    uint8 macAddress[6];                         /**< \brief MAC address for the ethernet, should be unique in the network */
    uint32 (*phyInit)(void);                     /**< \brief Pointer to the transceiver init function */
    boolean (*phyLink)(void);                    /**< \brief Pointer to the transceiver link function */
    const IfxEth_PortPins *portPins;             /**< \brief Pointer to port pins configuration */
    Ifx_Priority           isrPriority;          /**< \brief Interrupt service priority */
    IfxSrc_Tos             isrProvider;          /**< \brief Interrupt service provider */
} IfxEth_Config;

/** \} */

typedef union
{
    IfxEth_RxDescr items[IFXETH_MAX_RX_BUFFERS];
    uint32         U[IFXETH_MAX_RX_BUFFERS][IFXETH_DESCR_SIZE];
} IfxEth_RxDescrList;

typedef union
{
    IfxEth_TxDescr items[IFXETH_MAX_TX_BUFFERS];
    uint32         U[IFXETH_MAX_TX_BUFFERS][IFXETH_DESCR_SIZE];
} IfxEth_TxDescrList;

/** \addtogroup IfxLld_Eth_Std_DataStructures
 * \{ */
/** \brief ETH driver structure
 */
typedef struct
{
    Ifx_ETH_STATUS      status;         /**< \brief Intermediate variable to use register content in control structure */
    uint32              rxCount;        /**< \brief Number of frames received */
    uint32              txCount;        /**< \brief Number of frames transmitted */
    uint32              error;          /**< \brief Indicate an error has occurred during execution */
    sint32              isrRxCount;     /**< \brief Count of RX ISR */
    sint32              isrTxCount;     /**< \brief Count of TX ISR */
    sint32              txDiff;         /**< \brief Difference between isrTxCount and txCount */
    sint32              rxDiff;         /**< \brief Difference between isrRxCount and rxCount */
    sint32              isrCount;       /**< \brief count of all ISR */
    IfxEth_Config       config;         /**< \brief Copy of the configuration passed through IfxEth_init() */
    IfxEth_RxDescrList *rxDescr;        /**< \brief pointer to RX descriptor RAM */
    IfxEth_TxDescrList *txDescr;        /**< \brief pointer to TX descriptor RAM */
    IfxEth_RxDescr     *pRxDescr;
    IfxEth_TxDescr     *pTxDescr;
} IfxEth;

/** \brief Structure for RX descriptor DWORD 0 Bit field access
 */
typedef struct
{
    uint32 PCE : 1;    /**< \brief Rx MAC Address/Payload Checksum Error */
    uint32 CE : 1;     /**< \brief CRC Error */
    uint32 DBE : 1;    /**< \brief Dribble Bit Error */
    uint32 RE : 1;     /**< \brief Receive Error */
    uint32 RWT : 1;    /**< \brief Receive Watchdog Timeout */
    uint32 FT : 1;     /**< \brief Frame Type */
    uint32 LC : 1;     /**< \brief Late Collision */
    uint32 IPC : 1;    /**< \brief IPC Checksum Error/Giant Frame */
    uint32 LS : 1;     /**< \brief Last Descriptor */
    uint32 FS : 1;     /**< \brief First Descriptor */
    uint32 VLAN : 1;   /**< \brief VLAN Tag */
    uint32 OE : 1;     /**< \brief Overflow Error */
    uint32 LE : 1;     /**< \brief Length Error */
    uint32 SAF : 1;    /**< \brief Source Address Filter Fail */
    uint32 DE : 1;     /**< \brief Descriptor Error */
    uint32 ES : 1;     /**< \brief Error Summary, ES = PCE | CE | RE | RWT | LC | IPC | OE | DE */
    uint32 FL : 14;    /**< \brief Frame Length */
    uint32 AFM : 1;    /**< \brief Destination Address Filter Fail */
    uint32 OWN : 1;    /**< \brief Own Bit, 1 = own by DMA */
} IfxEth_RxDescr0_Bits;

/** \brief Structure for RX descriptor DWORD 1 Bit field access
 */
typedef struct
{
    uint32 RBS1 : 11;   /**< \brief Receive Buffer 1 Size */
    uint32 RBS2 : 11;   /**< \brief Receive Buffer 2 Size */
    uint32 resv : 2;    /**< \brief (reserved) */
    uint32 RCH : 1;     /**< \brief Second Address Chained */
    uint32 RER : 1;     /**< \brief Receive End of Ring */
    uint32 resv2 : 5;   /**< \brief (reserved) */
    uint32 DIC : 1;     /**< \brief Disable Interrupt on Completion */
} IfxEth_RxDescr1_Bits;

/** \brief Structure for TX descriptor DWORD 0 Bit field access
 */
typedef struct
{
    uint32 DB : 1;      /**< \brief Deferred Bit */
    uint32 UF : 1;      /**< \brief Underflow Error */
    uint32 ED : 1;      /**< \brief Excessive Deferral */
    uint32 CC : 4;      /**< \brief Collision Count */
    uint32 VLAN : 1;    /**< \brief VLAN Tag */
    uint32 EC : 1;      /**< \brief Excessive Collision */
    uint32 LC : 1;      /**< \brief Late Collision */
    uint32 NC : 1;      /**< \brief No Carrier */
    uint32 LOC : 1;     /**< \brief Loss of Carrier */
    uint32 PCE : 1;     /**< \brief Payload Checksum Error */
    uint32 FF : 1;      /**< \brief Frame Flushed */
    uint32 JT : 1;      /**< \brief Jabber Timeout */
    uint32 ES : 1;      /**< \brief Error Summary, ES = JT | FF | LOC | NC | LC | EC | ED | UF */
    uint32 IHE : 1;     /**< \brief IP Header Error */
    uint32 TTSS : 1;    /**< \brief Tx Time Stamp Status */
    uint32 resv : 13;   /**< \brief (reserved) */
    uint32 OWN : 1;     /**< \brief Own Bit, 1 = own by DMA */
} IfxEth_TxDescr0_Bits;

/** \brief Structure for TX descriptor DWORD 1 Bit field access
 */
typedef struct
{
    uint32 TBS1 : 11;   /**< \brief Transmit Buffer 1 Size */
    uint32 TBS2 : 11;   /**< \brief Transmit Buffer 2 Size */
    uint32 TTSE : 1;    /**< \brief Transmit Time Stamp Enable */
    uint32 DP : 1;      /**< \brief Disable Padding */
    uint32 TCH : 1;     /**< \brief Second Address Chained */
    uint32 TER : 1;     /**< \brief Transmit End of Ring */
    uint32 DC : 1;      /**< \brief Disable CRC */
    uint32 CIC : 2;     /**< \brief Checksum Insertion Control */
    uint32 FS : 1;      /**< \brief First Segment */
    uint32 LS : 1;      /**< \brief Last Segment */
    uint32 IC : 1;      /**< \brief Interrupt on Completion */
} IfxEth_TxDescr1_Bits;

/** \} */

/** \addtogroup IfxLld_Eth_Std_Configuration
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Waits for one TX buffer becomes available
 * \param eth ETH driver structure
 * retval non NULL_PTR TX buffer is available at the address pointed by the returned value
 * retval NULL_PTR TX buffer is busy.
 */
IFX_INLINE void *IfxEth_waitTransmitBuffer(IfxEth *eth);

/** \brief Sets the MAC address
 * \param eth ETH driver structure
 * \param loopbackMode loopback mode enable/disbale
 * \return None
 */
IFX_INLINE void IfxEth_setLoopbackMode(IfxEth *eth, boolean loopbackMode);

/** \brief Clear transmit interrupt request
 * \param eth ETH driver structure
 * \return None
 */
IFX_INLINE void IfxEth_clearTxInterrupt(IfxEth *eth);

/** \brief Clear receive interrupt request
 * \param eth ETH driver structure
 * \return None
 */
IFX_INLINE void IfxEth_clearRxInterrupt(IfxEth *eth);

/** \brief Set buffer of a TX descriptor
 * \param descr Entdescr Pointer to a TX descriptorer_String_here
 * \return None
 */
IFX_INLINE void IfxEth_TxDescr_setBuffer(IfxEth_TxDescr *descr, void *buffer);

/** \brief Return TRUE if a TX descriptor is available for setup
 */
IFX_INLINE boolean IfxEth_TxDescr_isAvailable(IfxEth_TxDescr *descr);

/** \brief Get pointer to next TX descriptor
 * \param descr descr Pointer to a TX descriptor
 */
IFX_INLINE IfxEth_TxDescr *IfxEth_TxDescr_getNext(IfxEth_TxDescr *descr);

/** \brief Set buffer of an RX descriptor
 * \param descr descr Pointer to an RX descriptor
 * \return None
 */
IFX_INLINE void IfxEth_RxDescr_setBuffer(IfxEth_RxDescr *descr, void *buffer);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Start the receiver functions
 * \param eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_startReceiver(IfxEth *eth);

/** \brief Sets the MAC address
 * \param eth ETH driver structure
 * \param macAddress MAC address
 * \return None
 */
IFX_EXTERN void IfxEth_setMacAddress(IfxEth *eth, const uint8 *macAddress);

/** \brief Request to send the transmit buffer
 *
 * The transmit buffer is the last one specified by IfxEth_getTransmitBuffer()
 * \param eth ETH driver structure
 * \param len Length of the data put in the transmit buffer (in bytes)
 * \return None
 */
IFX_EXTERN void IfxEth_sendTransmitBuffer(IfxEth *eth, uint16 len);

/** \brief Free the receive buffer, enabling it for the further reception
 * \param eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_freeReceiveBuffer(IfxEth *eth);

/** \} */

/** \addtogroup IfxLld_Eth_Std_Utility
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get pointer to next RX descriptor
 * \param descr descr Pointer to an RX descriptor
 */
IFX_INLINE IfxEth_RxDescr *IfxEth_RxDescr_getNext(IfxEth_RxDescr *descr);

/** \brief Get pointer to actual RX descriptor
 * \param eth eth ETH driver structure
 */
IFX_INLINE IfxEth_RxDescr *IfxEth_getActualRxDescriptor(IfxEth *eth);

/** \brief Shuffle to next TX descriptor
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_INLINE void IfxEth_shuffleTxDescriptor(IfxEth *eth);

/** \brief Shuffle to next RX descriptor
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_INLINE void IfxEth_shuffleRxDescriptor(IfxEth *eth);

/** \brief Checks whether transmit interrupt is requested
 * \param eth ETH driver structure
 * \return TRUE/FALSE
 */
IFX_INLINE boolean IfxEth_isTxInterrupt(IfxEth *eth);

/** \brief Checks whether receive interrupt is requested
 * \param eth ETH driver structure
 * \return TRUE/FALSE
 */
IFX_INLINE boolean IfxEth_isRxInterrupt(IfxEth *eth);

/** \brief Checks whether one or more RX data is available
 * \param eth ETH driver structure
 * \return retval TRUE one or more RX data is available
 * retval FALSE no RX data is available
 */
IFX_INLINE boolean IfxEth_isRxDataAvailable(IfxEth *eth);

/**
 * \param eth pointer to ETH driver structure
 */
IFX_INLINE boolean IfxEth_isRxChecksumError(IfxEth *eth);

/** \brief Checks whether physical connection is active
 * \param eth ETH driver structure
 * \return retval zero Connection is inactive
 * retval non zero Connection is active
 */
IFX_INLINE boolean IfxEth_isLinkActive(IfxEth *eth);

/** \brief Returns length of the oldest available RX data
 * \param eth ETH driver structure
 * \return Data length
 */
IFX_INLINE uint16 IfxEth_getRxDataLength(IfxEth *eth);

/** \brief Returns pointer to the MAC address configured for this ETH
 * \param eth ETH driver structure
 */
IFX_INLINE void *IfxEth_getMacAddressPointer(IfxEth *eth);

/** \brief Get a free transmit buffer
 * \param eth ETH driver structure
 * \return Loop back mode status (TRUE / FALSE)
 */
IFX_INLINE boolean IfxEth_getLoopbackMode(IfxEth *eth);

/** \brief Get pointer to base TX descriptor
 * \param eth eth ETH driver structure
 */
IFX_INLINE IfxEth_TxDescr *IfxEth_getBaseTxDescriptor(IfxEth *eth);

/** \brief Get pointer to base RX descriptor
 * \param eth eth ETH driver structure
 */
IFX_INLINE IfxEth_RxDescr *IfxEth_getBaseRxDescriptor(IfxEth *eth);

/** \brief Get pointer to actual TX descriptor
 * \param eth eth ETH driver structure
 */
IFX_INLINE IfxEth_TxDescr *IfxEth_getActualTxDescriptor(IfxEth *eth);

/**
 */
IFX_INLINE uint32 IfxEth_getActualRxIndex(IfxEth *eth);

/** \brief Release a TX descriptor for transmit queue
 * \param descr Enter_String_herdescr Pointer to a TX descriptore
 * \return None
 */
IFX_INLINE void IfxEth_TxDescr_release(IfxEth_TxDescr *descr);

/** \brief release RX descriptor
 * \return None
 */
IFX_INLINE void IfxEth_RxDescr_release(IfxEth_RxDescr *descr);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Wakeup the transmitter functions
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_wakeupTransmitter(IfxEth *eth);

/** \brief Wakeup the receiver functions
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_wakeupReceiver(IfxEth *eth);

/** \brief Stop the transmitter functions
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_stopTransmitter(IfxEth *eth);

/** \brief Start the transmitter functions
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_startTransmitter(IfxEth *eth);

/** \brief Set up checksum Engine
 * \param eth eth ETH driver structure
 * \return None
 */
IFX_EXTERN void IfxEth_setupChecksumEngine(IfxEth *eth, IfxEth_ChecksumMode mode);

/**
 * \param eth pointer to ETH driver structure
 * \param len length of buffer
 * \return None
 */
IFX_EXTERN void IfxEth_setAndSendTransmitBuffer(IfxEth *eth, void *buffer, uint16 len);

/** \brief Reads the MAC address from module register
 * \param eth ETH driver structure
 * \param macAddress MAC address
 * \return None
 */
IFX_EXTERN void IfxEth_readMacAddress(IfxEth *eth, uint8 *macAddress);

/** \brief Get a free transmit buffer
 * \param eth ETH driver structure
 * \return retval NULL_PTR no free transmit buffer is available
 * retval !NULL_PTR a free transmit buffer is available
 */
IFX_EXTERN void *IfxEth_getTransmitBuffer(IfxEth *eth);

/** \brief Gets receive buffer\n
 * note: IfxEth_freeReceiveBuffer() shall be called after the data from the RX buffer has been processed
 * \param eth ETH driver structure
 * \return retval NULL_PTR no received frame
 * retval !NULL_PTR a frame has been received
 */
IFX_EXTERN void *IfxEth_getReceiveBuffer(IfxEth *eth);

/** \brief Enable ETH Module
 * \return None
 */
IFX_EXTERN void IfxEth_enableModule(void);

/** \} */

/** \addtogroup IfxLld_Eth_Std_Initialisation
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Setup some properties of a TX descriptor
 * \param descr Enter_Sdescr Pointer to a TX descriptortring_here
 * \return None
 */
IFX_INLINE void IfxEth_TxDescr_setup(IfxEth_TxDescr *descr, uint16 length, boolean firstSegment, boolean lastSegment);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief set output pin of port
 * \param eth eth pointer to ETH driver structure
 * \param portPins portPins pin of port to be set
 * \return None
 */
IFX_EXTERN void IfxEth_setupOutputPins(IfxEth *eth, const IfxEth_PortPins *portPins);

/** \brief Set up input pins
 * \param eth eth pointer to ETH driver structure
 * \param portPins portPins pin of port to be set
 * \return None
 */
IFX_EXTERN void IfxEth_setupInputPins(IfxEth *eth, const IfxEth_PortPins *portPins);

/** \brief Initialises the driver
 * \param eth ETH driver structure
 * \param config ETH configuration structure
 * \return None
 */
IFX_EXTERN void IfxEth_init(IfxEth *eth, const IfxEth_Config *config);

/** \} */

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

/** \brief receive buffers
 */
IFX_EXTERN uint8              IfxEth_rxBuffer[IFXETH_MAX_RX_BUFFERS][IFXETH_RTX_BUFFER_SIZE];

IFX_EXTERN IfxEth_RxDescrList IfxEth_rxDescr;

/** \brief Transmit buffers
 */
IFX_EXTERN uint8              IfxEth_txBuffer[IFXETH_MAX_TX_BUFFERS][IFXETH_RTX_BUFFER_SIZE];

IFX_EXTERN IfxEth_TxDescrList IfxEth_txDescr;

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void *IfxEth_waitTransmitBuffer(IfxEth *eth)
{
    void *tx;

    do
    {
        tx = IfxEth_getTransmitBuffer(eth);
    } while (tx == NULL_PTR);

    return tx;
}


IFX_INLINE void IfxEth_setLoopbackMode(IfxEth *eth, boolean loopbackMode)
{
    (void)eth;
    ETH_MAC_CONFIGURATION.B.LM = loopbackMode ? 1 : 0;
}


IFX_INLINE void IfxEth_clearTxInterrupt(IfxEth *eth)
{
    (void)eth;
    MODULE_ETH.STATUS.B.TI = 1;
}


IFX_INLINE void IfxEth_clearRxInterrupt(IfxEth *eth)
{
    (void)eth;
    MODULE_ETH.STATUS.B.RI = 1;
}


IFX_INLINE void IfxEth_TxDescr_setBuffer(IfxEth_TxDescr *descr, void *buffer)
{
    descr->TDES2.U = (uint32)buffer;
}


IFX_INLINE boolean IfxEth_TxDescr_isAvailable(IfxEth_TxDescr *descr)
{
    return (descr->TDES0.A.OWN == 0) ? TRUE : FALSE;
}


IFX_INLINE IfxEth_TxDescr *IfxEth_TxDescr_getNext(IfxEth_TxDescr *descr)
{
    return (IfxEth_TxDescr *)(descr->TDES3.U);
}


IFX_INLINE void IfxEth_RxDescr_setBuffer(IfxEth_RxDescr *descr, void *buffer)
{
    descr->RDES2.U = (uint32)buffer;
}


IFX_INLINE IfxEth_RxDescr *IfxEth_RxDescr_getNext(IfxEth_RxDescr *descr)
{
    return (IfxEth_RxDescr *)(descr->RDES3.U);
}


IFX_INLINE IfxEth_RxDescr *IfxEth_getActualRxDescriptor(IfxEth *eth)
{
    return eth->pRxDescr;
}


IFX_INLINE void IfxEth_shuffleTxDescriptor(IfxEth *eth)
{
    eth->pTxDescr = IfxEth_TxDescr_getNext(eth->pTxDescr);
}


IFX_INLINE void IfxEth_shuffleRxDescriptor(IfxEth *eth)
{
    eth->pRxDescr = IfxEth_RxDescr_getNext(eth->pRxDescr);
}


IFX_INLINE boolean IfxEth_isTxInterrupt(IfxEth *eth)
{
    (void)eth;

    return MODULE_ETH.STATUS.B.TI != 0;
}


IFX_INLINE boolean IfxEth_isRxInterrupt(IfxEth *eth)
{
    (void)eth;

    return MODULE_ETH.STATUS.B.RI != 0;
}


IFX_INLINE boolean IfxEth_isRxDataAvailable(IfxEth *eth)
{
    //return (IfxEth_rxDescr[eth->rxIndex][0] & (1U << 31)) == 0);
    return IfxEth_getActualRxDescriptor(eth)->RDES0.A.OWN == 0;
}


IFX_INLINE boolean IfxEth_isRxChecksumError(IfxEth *eth)
{
    IfxEth_RxDescr *descr = IfxEth_getActualRxDescriptor(eth);
    boolean         error = (descr->RDES0.A.IPC != 0);
    descr->RDES0.A.IPC = 0;

    return error;
}


IFX_INLINE boolean IfxEth_isLinkActive(IfxEth *eth)
{
    return eth->config.phyLink() != 0;
}


IFX_INLINE uint16 IfxEth_getRxDataLength(IfxEth *eth)
{
    uint16 length = 0;

    if (IfxEth_isRxDataAvailable(eth) != FALSE)
    {
        length = (uint16)IfxEth_getActualRxDescriptor(eth)->RDES0.A.FL;
    }

    return length;
}


IFX_INLINE void *IfxEth_getMacAddressPointer(IfxEth *eth)
{
    return (void *)eth->config.macAddress;
}


IFX_INLINE boolean IfxEth_getLoopbackMode(IfxEth *eth)
{
    (void)eth;
    return (ETH_MAC_CONFIGURATION.B.LM != 0) ? TRUE : FALSE;
}


IFX_INLINE IfxEth_TxDescr *IfxEth_getBaseTxDescriptor(IfxEth *eth)
{
    return eth->txDescr->items;
}


IFX_INLINE IfxEth_RxDescr *IfxEth_getBaseRxDescriptor(IfxEth *eth)
{
    return eth->rxDescr->items;
}


IFX_INLINE IfxEth_TxDescr *IfxEth_getActualTxDescriptor(IfxEth *eth)
{
    return eth->pTxDescr;
}


IFX_INLINE uint32 IfxEth_getActualRxIndex(IfxEth *eth)
{
    uint32 offset = (uint32)eth->pRxDescr - (uint32)IfxEth_getBaseRxDescriptor(eth);
    return offset / sizeof(IfxEth_RxDescr);
}


IFX_INLINE void IfxEth_TxDescr_release(IfxEth_TxDescr *descr)
{
    descr->TDES0.A.OWN = 1U;
}


IFX_INLINE void IfxEth_RxDescr_release(IfxEth_RxDescr *descr)
{
    descr->RDES0.A.OWN = 1U;
}


IFX_INLINE void IfxEth_TxDescr_setup(IfxEth_TxDescr *descr, uint16 length, boolean firstSegment, boolean lastSegment)
{
    IfxEth_TxDescr0 tdes0;

    tdes0.U        = descr->TDES0.U;
    tdes0.A.FS     = firstSegment;
    tdes0.A.LS     = lastSegment;
    descr->TDES0.U = tdes0.U;
    descr->TDES1.U = length;
}


#endif /* IFXET_H */
