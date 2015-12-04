/**
 * \file IfxMultican_Can.h
 * \brief MULTICAN CAN details
 * \ingroup IfxLld_Multican
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
 * \defgroup IfxLld_Multican_Can_Usage How to use the CAN Interface driver?
 * \ingroup IfxLld_Multican
 *
 * The CAN interface driver provides a default configuration for various modes.
 *
 * In the following sections it will be described, how to integrate the driver into the application framework.
 *
 * \section IfxLld_Multican_Can_Preparation Preparation
 * \subsection IfxLld_Multican_Can_Include Include Files
 *
 * Include following header file into your C code:
 * \code
 *     #include <Multican/Can/IfxMultican_Can.h>
 * \endcode
 *
 * \subsection IfxLld_Multican_Can_Variables Variables
 *
 * Declare the CAN handles as global variables in your C code:
 * \code
 *     // CAN handle
 *     IfxMultican_Can can;
 *
 *     // Nodes handles
 *     IfxMultican_Can_Node canSrcNode;
 *     IfxMultican_Can_Node canDstNode;
 *
 *     // Message Object handles
 *     IfxMultican_Can_MsgObj canSrcMsgObj;
 *     IfxMultican_Can_MsgObj canDstMsgObj;
 *
 *     const unsigned id = 0x100;
 * \endcode
 *
 * \subsection IfxLld_Multican_Can_Init Module Initialisation
 *
 * The module initialisation can be done as followed:
 * \code
 *     // create configuration
 *     IfxMultican_Can_Config canConfig;
 *     IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
 *
 *     // initialize module
 *     // IfxMultican_Can can; // defined globally
 *     IfxMultican_Can_initModule(&can, &canConfig);
 * \endcode
 *
 * \subsection IfxLld_Multican_Can_InitNode Node Initialisation
 *
 * The Can nodes initialisation can be done as followed:
 *
 * \code
 *     // create CAN node config
 *     IfxMultican_Can_NodeConfig canNodeConfig;
 *     IfxMultican_Can_Node_initConfig(&canNodeConfig, &can);
 *
 *     canNodeConfig.baudrate = 1000000; // 1 MBaud
 *
 *     // Source Node
 *     // IfxMultican_Can_Node canSrcNode; // defined globally
 *     {
 *         canNodeConfig.nodeId = IfxMultican_NodeId_0;
 *         canNodeConfig.rxPin = &IIfxMultican_RXD0B_P20_7_IN;
 *         canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
 *         canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
 *         canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;
 *
 *         // initialise the node
 *         IfxMultican_Can_Node_init(&canSrcNode, &canNodeConfig);
 *     }
 *
 *     // Destination Node
 *     // IfxMultican_Can_Node canDstNode; // defined globally
 *     {
 *         canNodeConfig.nodeId = IfxMultican_NodeId_1;
 *         canNodeConfig.rxPin = &IfxMultican_RXD1B_P14_1_IN;
 *         canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
 *         canNodeConfig.txPin = &IfxMultican_TXD1_P14_0_OUT;
 *         canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;
 *
 *         // initialise the node
 *         IfxMultican_Can_Node_init(&canDstNode, &canNodeConfig);
 *     }
 * \endcode
 *
 * \subsection IfxLld_Multican_Can_InitMessageObject Message Object Initialisation
 *
 * The Can message objects initialisation can be done as followed:
 *
 * \code
 *     // IfxMultican_Can_MsgObj canSrcMsgObj; // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *  // assigned message object:
 *         canMsgObjConfig.msgObjId = 0;
 *
 *         canMsgObjConfig.messageId = id; // 'id' is defined globally
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // IfxMultican_Can_MsgObj canDstMsgObj; // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *  // assigned message object:
 *         canMsgObjConfig.msgObjId = 1;
 *
 *         canMsgObjConfig.messageId = id;
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
 *     }
 * \endcode
 *
 * The MULTICAN is ready for use now!
 *
 *
 * \section IfxLld_Multican_Can_StandardDataTransfers Single Data Transfers
 *
 * The CAN driver provides simple to use transfer functions
 *
 * Data can be sent by the following way:
 * \code
 *     const unsigned dataLow = 0xC0CAC01A;
 *     const unsigned dataHigh = 0xBA5EBA11;
 *
 *     // Initialise the message strcture
 *     IfxMultican_Message txMsg;
 *     IfxMultican_Message_init(&txMsg, id, dataLow, dataHigh, IfxMultican_DataLengthCode_8);
 *
 *     // Transmit Data
 *     while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy );
 * \endcode
 *
 * Data can be received by the following way:
 * \code
 *     // Receiving Data
 *
 *     // Initialise the message structure with dummy values, will be replaced by the received values
 *     IfxMultican_Message rxMsg;
 *     IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values
 *
 *     // wait until Multican received a new message
 *     while( !IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj) );
 *
 *     // read message
 *     IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
 *
 *     if( readStatus != IfxMultican_Status_noError ) {
 *         clib_ver_printf("ERROR: IfxMultican_Can_MsgObj_readMessage returned 0x%04x\n", readStatus);
 *         result |= 1;
 *     }
 *
 *     // data now available at rxMsg.data[0] and rxMsg.data[1]
 * \endcode
 *
 *
 * \section IfxLld_Multican_Can_FIFOBasedTransfers FIFO based Transfers
 *
 * A transmit and receive FIFO can be enabled during the node configuration by specifing the number of allocated message objects with the canMsgObjConfig.msgObjCount item.
 * and specifying the message object number of first slave object with the canMsgObjConfig.firstSlaveObjId item.
 *
 * Message objects will be allocated to the FIFO in ascending order.
 *
 * Here a configuration example:
 * \code
 * //add the following defines to your code globally
 * #define FIFO_SIZE 16
 * #define FIFO_SIZE 8
 *
 *
 *     // IfxMultican_Can_MsgObj canSrcMsgObj; // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *  // FIFO MsgObj allocation:
 *         canMsgObjConfig.msgObjId = 0; // will allocate MsgObj 0
 *         canMsgObjConfig.msgObjCount = FIFO_SIZE/2;
 *  canMsgObjConfig.firstSlaveObjId = 1;
 *
 *         canMsgObjConfig.messageId = id; // 'id' defined gloabally
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // IfxMultican_Can_MsgObj canDstMsgObj; // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *  // FIFO MsgObj allocation:
 *         canMsgObjConfig.msgObjId = FIFO_SIZE + 1; // avoid clashing with transmit FIFO message objects
 *         canMsgObjConfig.msgObjCount = FIFO_SIZE;
 *
 *  canMsgObjConfig.firstSlaveObjId = FIFO_SIZE + 2;
 *
 *         canMsgObjConfig.messageId = id; // 'id' defined gloabally
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
 *     }
 * \endcode
 *
 * Data can now be sent and received with the IfxMultican_Can_MsgObj_sendMessage and IfxMultican_Can_MsgObj_readMessage functions as usual (see above).
 *
 * here is a transmit example
 * \code
 *     IfxMultican_Message txMsg;
 *     for (i = 0; i < FIFO_SIZE; ++i)
 *     {
 *         // Transmit Data from the source message object //
 *
 *         IfxMultican_Message_init(&txMsg, id, dataLow + i, dataHigh + i, IfxMultican_DataLengthCode_8);
 *
 *         while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy );
 *     }
 * \endcode
 *
 * data can be read by the following way
 * \code
 *     IfxMultican_Message rxMsg;
 *     for (i = 0; i < FIFO_SIZE; ++i)
 *     {
 *         // Receiving Data, read the data from the destination receive Fifo //
 *         // wait until MCAN received the frame
 *         // wait until Multican received a new message
 *         while( !IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj) );
 *
 *
 *         IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values
 *
 *         IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
 *     }
 * \endcode
 *
 *
 * \section IfxLld_Multican_Can_GatewayTransfers Gateway Transfers
 * After initialising th emodule and nodes, the gateway message objects can be initialised in the following way
 *
 * A Gateway transfers can be enabled during the message object configuration by specifing with the canMsgObjConfig.gatewayTransfersEnable item.
 *
 * Here a configuration example:
 * \code
 *     // source message object
 *     // IfxMultican_Can_MsgObj canSrcMsgObj;  // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *         canMsgObjConfig.msgObjId = 0;
 *         canMsgObjConfig.messageId = id; // 'id' defined globally
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *  canMsgObjConfig.gatewayTransfers    = FALSE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // gateway source message object //
 *     // data will be received into this object from SrcObj, and then copied into gateway destination object //
 *     IfxMultican_Can_MsgObj canGatewaySrcMsgObj;
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *         canMsgObjConfig.msgObjId = 1;
 *         canMsgObjConfig.messageId = id;
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *  canMsgObjConfig.gatewayTransfers                 = TRUE;
 *         canMsgObjConfig.gatewayConfig.copyDataLengthCode = TRUE;
 *         canMsgObjConfig.gatewayConfig.copyData           = TRUE;
 *         canMsgObjConfig.gatewayConfig.copyId             = FALSE;
 *         canMsgObjConfig.gatewayConfig.enableTransmit     = TRUE;  // if this is not choosen, then no need to initialise canDstMsgObj to read the final data
 *  canMsgObjConfig.gatewayConfig.gatewayDstObjId     = 3;    // specify the destination object number
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canGatewaySrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // gateway destination object //
 *     // data, id , datlength code will be copied from GatewaySrcObj into this object through gateway transfers //
 *     // and then sent onto the bus for sending the message to destination message object //
 *     IfxMultican_Can_MsgObj canGatewayDstMsgObj;
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *         canMsgObjConfig.msgObjId = 3;
 *         canMsgObjConfig.messageId = 0x200;
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *  canMsgObjConfig.gatewayTransfers   = FALSE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canGatewayDstMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // destination message object, not needed if  enableTransmit is not chosen in gateway source object, or you don't want to read the data//
 *     // data will be received from GatewayDstObj into this obj //
 *     // IfxMultican_Can_MsgObj canDstMsgObj;      // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *         canMsgObjConfig.msgObjId = 10;
 *         canMsgObjConfig.messageId = 0x200;
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = FALSE;
 *  canMsgObjConfig.gatewayTransfers                 = FALSE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
 *     }
 * \endcode
 *
 * Data can now be sent and received with the IfxMultican_Can_MsgObj_sendMessage and IfxMultican_Can_MsgObj_readMessage functions as usual (see above).
 *
 * The data flow is as followed,
 *
 * Data will be sent from the source object (canSrcMsgObj), it will be received by the gateway source object (canGatewaySrcMsgObj).
 * and then gets copied into the gateway destination object (canGatewayDstMsgObj) without CPU intervention,
 *
 * If GDFS is selected in gateway source object (canGatewaySrcMsgObj)then,
 * the data will be transmitted from gateway destination object(canGatewayDstMsgObj) to the destination object (canDstMsgObj)
 *
 * \section IfxLld_Multican_Can_Gateway_Fifo_Transfers Gateway FIFO based Transfers
 * After initialising the module and nodes,
 *
 * A gateway FIFO transfers can be enabled during the node configuration by specifing the number of allocated message objects with the canMsgObjConfig.msgObjCount item.
 * and enbling the gateway transfers with the canMsgObjConfig.gatewayTransfersEnable item. and also by selecting the start object of the FIFO with the
 * canMsgObjConfig.firstSlaveObjId item.
 *
 * Message objects will be allocated to the gateway FIFO in ascending order.
 * Here a configuration example:
 * \code
 *     // source message object, you can even make it as a Tx FIFO
 *     // IfxMultican_Can_MsgObj canSrcMsgObj;    // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *         canMsgObjConfig.msgObjId = 0;
 *         canMsgObjConfig.messageId = id;
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *  canMsgObjConfig.gatewayTransfers   = FALSE;
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     // gateway source Fifo //
 *     // data will be received into this object from SrcObj, and then copied into gateway Fifo objects //
 *     IfxMultican_Can_MsgObj canGatewaySrcMsgObj;
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *         canMsgObjConfig.msgObjId = 1;
 *         canMsgObjConfig.messageId = id;
 *  canMsgObjConfig.msgObjCount = 4;    // FIFO
 *  canMsgObjConfig.firstSlaveObjId = 2;  // will allocate MsgObj 2..5 for the gateway FIFO
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = TRUE;
 *  canMsgObjConfig.gatewayTransfers                 = TRUE;  // gateway FIFO
 *         canMsgObjConfig.gatewayConfig.copyDataLengthCode = TRUE;
 *         canMsgObjConfig.gatewayConfig.copyData           = TRUE;
 *         canMsgObjConfig.gatewayConfig.copyId             = FALSE;
 *         canMsgObjConfig.gatewayConfig.enableTransmit     = TRUE;
 *
 *
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canGatewaySrcMsgObj, &canMsgObjConfig);
 *     }
 *
 *     int i  = 0;
 *     for (i = 0; i < 4 ; ++i)
 *     {
 *  // gateway destination objects //
 *  // data, id , datlength code will be copied from GatewaySrcObj into this object through gateway transfers //
 *      // and then sent onto the bus for sending the message to destination receive Fifo message objects //
 *         IfxMultican_Can_MsgObj canGatewayDstMsgObj;
 *         {
 *             // create message object config
 *             IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *             IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
 *
 *             canMsgObjConfig.msgObjId = 2 + i;
 *             canMsgObjConfig.messageId = 0x200;
 *             canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *             canMsgObjConfig.frame = IfxMultican_Frame_transmit;
 *             canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *             canMsgObjConfig.control.extendedFrame = FALSE;
 *             canMsgObjConfig.control.matchingId = FALSE;
 *          canMsgObjConfig.gatewayTransfers   = FALSE;
 *
 *             // initialize message object
 *             IfxMultican_Can_MsgObj_init(&canGatewayDstMsgObj, &canMsgObjConfig);
 *         }
 *     }
 *     // destination receive Fifo,
 *     // data will be received from GatewayDstObj into this Fifo //
 *     // IfxMultican_Can_MsgObj canDstMsgObj;    // defined globally
 *     {
 *         // create message object config
 *         IfxMultican_Can_MsgObjConfig canMsgObjConfig;
 *         IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
 *
 *         canMsgObjConfig.msgObjId = 10;
 *         canMsgObjConfig.messageId = 0x200;
 *  canMsgObjConfig.msgObjCount = 4;   // receive FIFO
 *  canMsgObjConfig.firstSlaveObjId  = 11;   // will allocate MsgObj 11..14 for the receive FIFO
 *         canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
 *         canMsgObjConfig.frame = IfxMultican_Frame_receive;
 *         canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
 *         canMsgObjConfig.control.extendedFrame = FALSE;
 *         canMsgObjConfig.control.matchingId = FALSE;
 *  canMsgObjConfig.gatewayTransfers  = FALSE;
 *  canMsgObjConfig.firstSlaveObjId  = 11;
 *
 *
 *         // initialize message object
 *         IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
 *     }
 * \endcode
 *
 * Data can now be sent and received with the IfxMultican_Can_MsgObj_sendMessage and IfxMultican_Can_MsgObj_readMessage functions as usual (see above).
 * here is a transmit example
 * \code
 *     for (i = 0; i < 4; ++i)
 *     {
 *         // Transmit Data from the source message object //
 *         IfxMultican_Message txMsg;
 *         IfxMultican_Message_init(&txMsg, id, dataLow + i, dataHigh + i, IfxMultican_DataLengthCode_8);
 *
 *         while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy );
 *     }
 * \endcode
 *
 * The data flow is as followed,
 *
 * data will be sent from the source object (canSrcMsgObj) or source TX FIFO, it will be received by the gateway source FIFO (canGatewaySrcMsgObj).
 * and then gets copied into the gateway destination objects (canGatewayDstMsgObj) without CPU intervention,
 *
 * If GDFS is selected in gateway source object (FIFO) (canGatewaySrcMsgObj)then,
 * the data will be transmitted from gateway destination objects (canGatewayDstMsgObj) to the destination objects or receive FIFO (canDstMsgObj)
 *
 * data can be read by the following way
 * \code
 *     for (i = 0; i < 4; ++i)
 *     {
 *         // Receiving Data, read the data from the destination receive Fifo //
 *         // wait until MCAN received the frame
 *         // wait until Multican received a new message
 *         while( !IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj) );
 *
 *         IfxMultican_Message rxMsg;
 *         IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values
 *
 *         IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
 *         if( readStatus != IfxMultican_Status_noError ) {
 *             clib_ver_printf("ERROR: IfxMultican_Can_MsgObj_readMessage returned 0x%04x\n", readStatus);
 *             result |= 1;
 *             }
 *     }
 * \endcode
 *
 * \defgroup IfxLld_Multican_Can Can
 * \ingroup IfxLld_Multican
 * \defgroup IfxLld_Multican_Can_Data_Structures Data structures
 * \ingroup IfxLld_Multican_Can
 * \defgroup IfxLld_Multican_Can_General General functions
 * \ingroup IfxLld_Multican_Can
 * \defgroup IfxLld_Multican_Can_Node CAN Nodes
 * \ingroup IfxLld_Multican_Can
 * \defgroup IfxLld_Multican_Can_Message_Objects Message Objects
 * \ingroup IfxLld_Multican_Can
 * \defgroup IfxLld_Multican_Can_Interrupts Interrupts
 * \ingroup IfxLld_Multican_Can
 */

#ifndef IFXMULTICAN_CAN_H
#define IFXMULTICAN_CAN_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Multican/Std/IfxMultican.h"
#include "Scu/Std/IfxScuCcu.h"
#include "_Reg/IfxScu_regdef.h"

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Multican_Can_Data_Structures
 * \{ */
/** \brief Structure for gateway configuration
 */
typedef struct
{
    uint32               copyDataLengthCode : 1;   /**< \brief Specifies the choice for copying data length code */
    uint32               copyData : 1;             /**< \brief Specifies the choice for copying data (data low and data high) */
    uint32               copyId : 1;               /**< \brief Specifies the choice for copying id of the message */
    uint32               enableTransmit : 1;       /**< \brief Specifies the enable choice of TXRQ in the destination gateway object (GDFS of source gateway object) */
    IfxMultican_MsgObjId gatewayDstObjId;          /**< \brief Message object number of first slave object (bottom pointer) */
} IfxMultican_Can_GatewayConfig;

/** \brief Structure for interrupt configuration
 */
typedef struct
{
    uint16     priority;          /**< \brief interrupt priority */
    IfxSrc_Tos typeOfService;     /**< \brief type of interrupt service */
} IfxMultican_Can_InterruptConfig;

/** \brief Structure for interrupt source
 */
typedef struct
{
    boolean           enabled;     /**< \brief If true, enables the interrupt generation */
    IfxMultican_SrcId srcId;       /**< \brief interrupt node pointer used */
} IfxMultican_Can_InterruptSource;

/** \brief Message object control
 */
typedef struct
{
    uint32                     singleDataTransfer : 1;    /**< \brief Specifies the single data transfer option. If 1, single data transfer is selected */
    uint32                     singleTransmitTrial : 1;   /**< \brief Specifies the single transmit trial option. If 1, single transmit trial is selected */
    IfxMultican_DataLengthCode messageLen;                /**< \brief Specifies the length of the transmited data (number of bytes). This value is ignored for receive object */
    uint32                     extendedFrame : 1;         /**< \brief Specifies the standard / extended frame mode. 0: standard frame 11 bit ID; 1: extended frame 29 bit ID */
    uint32                     matchingId : 1;            /**< \brief Specifies the acceptance mask. 0: standard & extended frame (11 & 29 bit); 1: only frames with maching IDE */
} IfxMultican_Can_MsgObjControl;

/** \brief CAN node handle data structure
 */
typedef struct
{
    Ifx_CAN           *mcan;       /**< \brief Specifies the pointer to the MULTICAN module registers */
    Ifx_CAN_N         *node;       /**< \brief Specifies the pointer to the MULTICAN node registers */
    IfxMultican_NodeId nodeId;     /**< \brief Specifies the node Id */
} IfxMultican_Can_Node;

/** \} */

/** \addtogroup IfxLld_Multican_Can_Data_Structures
 * \{ */
/** \brief CAN handle data structure
 */
typedef struct
{
    Ifx_CAN *mcan;     /**< \brief Specifies the pointer to the MULTICAN module registers */
} IfxMultican_Can;

/** \brief CAN module configuration
 */
typedef struct
{
    Ifx_CAN                        *module;                               /**< \brief pointer to MULTICAN module */
    IfxMultican_ClockSelect         clockSelect;                          /**< \brief Selected module input clock */
    float32                         moduleFreq;                           /**< \brief Required module frequency in Hertz */
    IfxMultican_Can_InterruptConfig nodePointer[IFXMULTICAN_NUM_SRC];     /**< \brief Node pointer configuration */
} IfxMultican_Can_Config;

/** \brief CAN message object handle data structure
 */
typedef struct
{
    IfxMultican_Can_Node *node;            /**< \brief Specifies the pointer to the node handle */
    IfxMultican_MsgObjId  msgObjId;        /**< \brief Specifies the message object ID */
    uint16                msgObjCount;     /**< \brief Number of message object sto be initialised (1 for standard Msg Obj and no. of objects including base object for FIFO transfers) */
    IfxMultican_MsgObjId  fifoPointer;     /**< \brief Pointer for FIFO based transfers */
} IfxMultican_Can_MsgObj;

/** \brief CAN message object configuration
 */
typedef struct
{
    IfxMultican_Can_Node           *node;                   /**< \brief Specifies the pointer to the node handle */
    IfxMultican_MsgObjId            msgObjId;               /**< \brief Specifies the message object ID */
    uint16                          msgObjCount;            /**< \brief Number of message object sto be initialised (1 for standard Msg Obj and no. of objects including base object for FIFO transfers) */
    IfxMultican_Can_MsgObjControl   control;                /**< \brief Message object control */
    IfxMultican_Frame               frame;                  /**< \brief Specifies the frame type */
    uint32                          acceptanceMask;         /**< \brief Specifies the acceptance mask */
    uint32                          messageId;              /**< \brief Specifies the message ID */
    IfxMultican_Priority            priority;               /**< \brief Specifies the message object priority */
    IfxMultican_Can_InterruptSource rxInterrupt;            /**< \brief Rx Interrupt configuration */
    IfxMultican_Can_InterruptSource txInterrupt;            /**< \brief Tx Interrupt configuration */
    uint32                          gatewayTransfers : 1;   /**< \brief Specifies the gateway source object (gateway transfres enable / disable choice) */
    IfxMultican_Can_GatewayConfig   gatewayConfig;          /**< \brief Structure for gateway configuration */
    IfxMultican_MsgObjId            firstSlaveObjId;        /**< \brief Message object number of first slave object (bottom pointer) */
} IfxMultican_Can_MsgObjConfig;

/** \brief CAN Node configuration
 */
typedef struct
{
    Ifx_CAN                        *module;                     /**< \brief pointer to MULTICAN module */
    IfxMultican_NodeId              nodeId;                     /**< \brief Specifies the node Id */
    boolean                         analyzerMode;               /**< \brief Specifies the analizer mode. If TRUE then the CAN Node works in analizer mode */
    boolean                         loopBackMode;               /**< \brief Specifies the loop back mode. If TRUE then the CAN Node works in loop back mode */
    uint32                          baudrate;                   /**< \brief Specifies the baudrate */
    uint16                          samplePoint;                /**< \brief Specifies the sample point. Range = [0, 10000] resp. [0%, 100%] of the total bit time */
    uint16                          synchJumpWidth;             /**< \brief Specifies the resynchronisation jump width. Range = [0, 10000] resp. [0%, 100%] of the total bit time */
    IfxMultican_Rxd_In             *rxPin;                      /**< \brief Specifies the receive pin */
    IfxPort_InputMode               rxPinMode;                  /**< \brief Specifies the receive pin as input mode */
    IfxMultican_Txd_Out            *txPin;                      /**< \brief Specifies the transmit pin */
    IfxPort_OutputMode              txPinMode;                  /**< \brief Specifies the transmit pin output mode */
    uint8                           errorWarningLevel;          /**< \brief Specifies the error warinig level */
    IfxMultican_Can_InterruptSource transferInterrupt;          /**< \brief Transfer interrupt */
    IfxMultican_Can_InterruptSource lastErrorCodeInterrupt;     /**< \brief Last error code interrupt */
    IfxMultican_Can_InterruptSource alertInterrupt;             /**< \brief Alert interrupt */
    IfxMultican_Can_InterruptSource frameCounterInterrupt;      /**< \brief Frame counter interrupt */
    IfxMultican_Can_InterruptSource timerInterrupt;             /**< \brief Timer Interrupt */
} IfxMultican_Can_NodeConfig;

/** \} */

/** \addtogroup IfxLld_Multican_Can_General
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Reset the CAN module.\n
 * Reset and disable the CAN module, inclusive message object and node registers.
 * \param mcan pointer to the CAN handle
 * \return None
 */
IFX_INLINE void IfxMultican_Can_deinit(IfxMultican_Can *mcan);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Return the actual CAN module configuration
 * \param mcan pointer to the CAN handle
 * \param config Pointer to the configuration structure, will be filled by this function
 * \return None
 */
IFX_EXTERN void IfxMultican_Can_getConfig(IfxMultican_Can *mcan, IfxMultican_Can_Config *config);

/** \brief Get the module frequency
 * \param mcan pointer to the CAN handle
 * \return Frequency Value
 */
IFX_EXTERN float32 IfxMultican_Can_getModuleFrequency(IfxMultican_Can *mcan);

/** \brief Initialize the CAN module\n
 * The following configuration is used:\n
 * - The CAN module is stopped during sleep mode\n
 * - The normal divider mode is selected\n
 * - The CAN module clock is the system clock
 * \param mcan pointer to the CAN handle
 * \param config Specifies pointer to the CAN module configuration
 * \return TRUE: Returns TRUE if the operation was successful\n
 * FALSE: Returns FALSE if the operation was errorneous
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN IfxMultican_Status IfxMultican_Can_initModule(IfxMultican_Can *mcan, const IfxMultican_Can_Config *config);

/** \brief Return the default MULTICAN configuration
 * \param config Default configuration filled by this function
 * \param mcan base address of the MULTICAN register space
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN void IfxMultican_Can_initModuleConfig(IfxMultican_Can_Config *config, Ifx_CAN *mcan);

/** \} */

/** \addtogroup IfxLld_Multican_Can_Node
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Activate the CAN Node. Participate in the CAN bus activities
 * \param node Specifies the CAN node handle to be configured
 * \return None
 */
IFX_INLINE void IfxMultican_Can_Node_activate(IfxMultican_Can_Node *node);

/** \brief Deactivate the CAN Node. Take out from participation in the CAN bus activities
 * \param node Specifies the CAN node handle to be configured
 * \return None
 */
IFX_INLINE void IfxMultican_Can_Node_deactivate(IfxMultican_Can_Node *node);

/** \brief Reset the CAN node
 * \param node Specifies the CAN node handle to be configured
 * \return None
 */
IFX_INLINE void IfxMultican_Can_Node_deinit(IfxMultican_Can_Node *node);

/** \brief Recovers the CAN node from bus off
 * \param node Specifies the CAN node handle to be configured
 * \return Status
 *
 * IfxMultican_Status status = IfxMultican_Status_busOff;
 *
 * while (status != IfxMultican_Status_noError)
 * {
 *     status = IfxMultican_Can_Node_recoverBusOff(&canNode);
 * }
 *
 */
IFX_INLINE IfxMultican_Status IfxMultican_Can_Node_recoverBusOff(IfxMultican_Can_Node *node);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get the actual message object configuration
 * \param node Specifies the CAN node handle to be configured
 * \param config Specifies the CAN node configuration
 * \return None
 */
IFX_EXTERN void IfxMultican_Can_Node_getConfig(IfxMultican_Can_Node *node, IfxMultican_Can_NodeConfig *config);

/** \brief Initialize the CAN node
 * \param node Specifies the CAN node handle to be configured
 * \param config Specifies the CAN node configuration
 * \return TRUE: Returns TRUE if the operation was successful\n
 * FALSE: Returns FALSE if the operation was errorneous
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN IfxMultican_Status IfxMultican_Can_Node_init(IfxMultican_Can_Node *node, IfxMultican_Can_NodeConfig *config);

/** \brief Get default CAN node configuration
 * \param config Specifies the CAN node configuration
 * \param mcan pointer to the CAN handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN void IfxMultican_Can_Node_initConfig(IfxMultican_Can_NodeConfig *config, IfxMultican_Can *mcan);

/** \brief Recovers the CAN node from bus off
 * \param node Specifies the CAN node handle to be configured
 * \return None
 *
 * IfxMultican_Status status = IfxMultican_Status_busOff;
 *
 * while (status != IfxMultican_Status_noError)
 * {
 *     status = IfxMultican_Can_Node_recoverBusOff(&canNode);
 * }
 *
 */
IFX_EXTERN void IfxMultican_Can_Node_sendToBusOff(IfxMultican_Can_Node *node);

/** \} */

/** \addtogroup IfxLld_Multican_Can_Message_Objects
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Cancel pending TX request by invalidating the request\n
 * Only when frame transmission has not been started
 * \param msgObj pointer to the CAN message object handle
 * \return TRUE if cancellation was successfully executed
 */
IFX_INLINE boolean IfxMultican_Can_MsgObj_cancelSend(IfxMultican_Can_MsgObj *msgObj);

/** \brief Clear the RX pending flag of a message object
 * \param msgObj pointer to the CAN message object handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_INLINE void IfxMultican_Can_MsgObj_clearRxPending(IfxMultican_Can_MsgObj *msgObj);

/** \brief Clear the TX pending flag of a message object
 * \param msgObj pointer to the CAN message object handle
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_INLINE void IfxMultican_Can_MsgObj_clearTxPending(IfxMultican_Can_MsgObj *msgObj);

/** \brief Reset the message object\n
 * Append the message object to the end of idle list and reset message object registers
 * \param msgObj pointer to the CAN message object handle
 * \return None
 */
IFX_INLINE void IfxMultican_Can_MsgObj_deinit(IfxMultican_Can_MsgObj *msgObj);

/** \brief Get message object ID which has TX/RX pending flag from a message object group
 * \param msgObj pointer to the CAN message object handle
 * \param msgObjGroup Message object group
 * \return Message Object Id
 */
IFX_INLINE IfxMultican_MsgObjId IfxMultican_Can_MsgObj_getPendingId(IfxMultican_Can_MsgObj *msgObj, IfxMultican_MsgObjGroup msgObjGroup);

/** \brief Get the message object status
 * \param msgObj pointer to the CAN message object handle
 * \return \ref IfxMultican_MsgObjStat bitfield
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_INLINE IfxMultican_MsgObjStat IfxMultican_Can_MsgObj_getStatus(IfxMultican_Can_MsgObj *msgObj);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Get the actual message object configuration
 * \param msgObj pointer to the CAN message object handle
 * \param config Pointer to the RAM buffer. Filled by this function
 * \return None
 */
IFX_EXTERN void IfxMultican_Can_MsgObj_getConfig(IfxMultican_Can_MsgObj *msgObj, IfxMultican_Can_MsgObjConfig *config);

/** \brief Initialize the message object
 * \param msgObj pointer to the CAN message object handle
 * \param config pointer to the CAN message object configuration
 * \return Status
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN IfxMultican_Status IfxMultican_Can_MsgObj_init(IfxMultican_Can_MsgObj *msgObj, const IfxMultican_Can_MsgObjConfig *config);

/** \brief Initialise message config with default values and the given parameters
 * \param config pointer to the CAN message object configuration
 * \param node pointer to the CAN node handle to which the message object should be assigned
 * \return None
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN void IfxMultican_Can_MsgObj_initConfig(IfxMultican_Can_MsgObjConfig *config, IfxMultican_Can_Node *node);

/** \brief Returns the RX pending flag of a message object.
 * \param msgObj pointer to the CAN message object handle
 * \return TRUE of the RX pending flag of a message object is set
 */
IFX_EXTERN boolean IfxMultican_Can_MsgObj_isRxPending(IfxMultican_Can_MsgObj *msgObj);

/** \brief Returns the TX pending flag of a message object.
 * \param msgObj pointer to the CAN message object handle
 * \return TRUE of the TX pending flag of a message object is set
 */
IFX_EXTERN boolean IfxMultican_Can_MsgObj_isTxPending(IfxMultican_Can_MsgObj *msgObj);

/** \brief Read a received CAN message
 * \param msgObj pointer to the CAN message object handle
 * \param msg This parameter is filled in by the function with the received message. Also when reading is not successful
 * \return IfxMultican_Status_noError: if the operation was successful\n
 * IfxMultican_Status_messageLost: if the message lost and new data is not yet ready\n
 * IfxMultican_Status_newDataButMessageLost: if the one message lost and last new data is retrieved successfully
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN IfxMultican_Status IfxMultican_Can_MsgObj_readMessage(IfxMultican_Can_MsgObj *msgObj, IfxMultican_Message *msg);

/** \brief Send a CAN message
 * \param msgObj pointer to the CAN message object handle
 * \param msg Specifies the msg to be send
 * \return IfxMultican_Status_noError: if the operation was successful\n
 * IfxMultican_Status_notSentBusy: if the operation was unsuccessful due to hardware is busy
 *
 * A coding example can be found in \ref IfxLld_Multican_Can_Usage
 *
 */
IFX_EXTERN IfxMultican_Status IfxMultican_Can_MsgObj_sendMessage(IfxMultican_Can_MsgObj *msgObj, const IfxMultican_Message *msg);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxMultican_Can_deinit(IfxMultican_Can *mcan)
{
    IfxMultican_deinit(mcan->mcan);
}


IFX_INLINE void IfxMultican_Can_Node_activate(IfxMultican_Can_Node *node)
{
    Ifx_CAN_N *hwNode = IfxMultican_Node_getPointer(node->mcan, node->nodeId);

    IfxMultican_Node_activate(hwNode);
}


IFX_INLINE void IfxMultican_Can_Node_deactivate(IfxMultican_Can_Node *node)
{
    Ifx_CAN_N *hwNode = IfxMultican_Node_getPointer(node->mcan, node->nodeId);
    IfxMultican_Node_deactivate(hwNode);
}


IFX_INLINE void IfxMultican_Can_Node_deinit(IfxMultican_Can_Node *node)
{
    Ifx_CAN_N *hwNode = IfxMultican_Node_getPointer(node->mcan, node->nodeId);

    IfxMultican_Node_deinit(hwNode);
}


IFX_INLINE IfxMultican_Status IfxMultican_Can_Node_recoverBusOff(IfxMultican_Can_Node *node)
{
    Ifx_CAN_N *hwNode = IfxMultican_Node_getPointer(node->mcan, node->nodeId);

    return IfxMultican_Node_recoverBusOff(hwNode);
}


IFX_INLINE boolean IfxMultican_Can_MsgObj_cancelSend(IfxMultican_Can_MsgObj *msgObj)
{
    Ifx_CAN_MO *hwObj = IfxMultican_MsgObj_getPointer(msgObj->node->mcan, msgObj->msgObjId);

    return IfxMultican_MsgObj_cancelSend(hwObj);
}


IFX_INLINE void IfxMultican_Can_MsgObj_clearRxPending(IfxMultican_Can_MsgObj *msgObj)
{
    Ifx_CAN_MO *hwObj = IfxMultican_MsgObj_getPointer(msgObj->node->mcan, msgObj->msgObjId);

    IfxMultican_MsgObj_clearRxPending(hwObj);
}


IFX_INLINE void IfxMultican_Can_MsgObj_clearTxPending(IfxMultican_Can_MsgObj *msgObj)
{
    Ifx_CAN_MO *hwObj = IfxMultican_MsgObj_getPointer(msgObj->node->mcan, msgObj->msgObjId);

    IfxMultican_MsgObj_clearTxPending(hwObj);
}


IFX_INLINE void IfxMultican_Can_MsgObj_deinit(IfxMultican_Can_MsgObj *msgObj)
{
    IfxMultican_MsgObj_deinit(msgObj->node->mcan, msgObj->msgObjId);
}


IFX_INLINE IfxMultican_MsgObjId IfxMultican_Can_MsgObj_getPendingId(IfxMultican_Can_MsgObj *msgObj, IfxMultican_MsgObjGroup msgObjGroup)
{
    return IfxMultican_MsgObj_getPendingId(msgObj->node->mcan, msgObjGroup);
}


IFX_INLINE IfxMultican_MsgObjStat IfxMultican_Can_MsgObj_getStatus(IfxMultican_Can_MsgObj *msgObj)
{
    Ifx_CAN_MO *hwObj = IfxMultican_MsgObj_getPointer(msgObj->node->mcan, msgObj->msgObjId);

    return IfxMultican_MsgObj_getStatus(hwObj);
}


#endif /* IFXMULTICAN_CAN_H */
