/**
 * \file	wDriver_Can.c
 * \brief	WNC CAN Process API
 *
 * \date	2014/12/27
 * \author	Crow CY Liao
 */

#include "wDriver_Can.h"

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
/**
 * @brief
 * Global CAN Node information
 * All CAN setting of system will be declared in this variable
 */
w_MulticanBasic g_MulticanBasic;
IfxMultican_Message gMsg;

/**
 * @function
 * Initiate system CAN interface based on \ref g_MulticanBasic
 * \section Remarks
 * Currently, this function only initiate Node 0 and the pin definition is P20_7 and P20_8.
 * The baudrate of Node0 is \ref NODE0_BAUDRATE and the FIFO size of Tx is \ref NODE0_TX_FIFO_SIZE
 */
void wMultican_init(void)
{
    /* create module config */
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

    /* initialize module */
    canConfig.nodePointer[IfxMultican_SrcId_0].priority = 99;
    canConfig.nodePointer[IfxMultican_SrcId_1].priority = 100;
    IfxMultican_Can_initModule(&g_MulticanBasic.drivers.can, &canConfig);

    /* create CAN node config */
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &g_MulticanBasic.drivers.can);

    g_MulticanBasic.drivers.canNode0FifoTxSize = NODE0_TX_FIFO_SIZE;
    g_MulticanBasic.drivers.canNode0FifoRxSize = NODE0_RX_FIFO_SIZE;
    g_MulticanBasic.drivers.baudrateNode0 = NODE0_BAUDRATE;
   	canNodeConfig.baudrate = g_MulticanBasic.drivers.baudrateNode0;

   	/* Node 0 */
    {
        canNodeConfig.nodeId    = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);
        canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;
        canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
        canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;
        canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

        IfxMultican_Can_Node_init(&g_MulticanBasic.drivers.canNode0, &canNodeConfig);
    }

    /* Node 0 Message object, Tx Base  */
    {
        /* create message object config */
        IfxMultican_Can_MsgObjConfig canMsgObjConfig;
        IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canNode0);

        canMsgObjConfig.msgObjId              = 0;
        canMsgObjConfig.messageId             = 0x7FF;		// Standard ID, the lowest priority.
        canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
        canMsgObjConfig.frame                 = IfxMultican_Frame_transmit;
        canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;
        canMsgObjConfig.control.extendedFrame = FALSE;
        canMsgObjConfig.control.matchingId    = TRUE;
        canMsgObjConfig.msgObjCount			  = g_MulticanBasic.drivers.canNode0FifoTxSize;		//FIFO size

        /* initialize message object */
        IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canNode0FifoTxBase, &canMsgObjConfig);
    }

    /* Node 0 Message object, Rx 0x200 */
    {
        /* create message object config */
        IfxMultican_Can_MsgObjConfig canMsgObjConfig;
        IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canNode0);

        canMsgObjConfig.msgObjId              = 128;
        canMsgObjConfig.messageId             = 0x201;		// Standard ID, the lowest priority.
        canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
        canMsgObjConfig.frame                 = IfxMultican_Frame_receive;
        canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;
        canMsgObjConfig.control.extendedFrame = FALSE;
        canMsgObjConfig.control.matchingId    = TRUE;
        canMsgObjConfig.msgObjCount			  = 1;//g_MulticanBasic.drivers.canNode0FifoRxSize;		//FIFO size
        canMsgObjConfig.rxInterrupt.enabled = TRUE;
        canMsgObjConfig.rxInterrupt.srcId	= IfxMultican_SrcId_1;
        canMsgObjConfig.priority			= IfxMultican_Priority_CAN_ID;

        /* initialize message object */
        IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canNode0Can0x201, &canMsgObjConfig);
    }

    /* Node 0 Message object, Rx 0x201 */
    {
        /* create message object config */
        IfxMultican_Can_MsgObjConfig canMsgObjConfig;
        IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &g_MulticanBasic.drivers.canNode0);

        canMsgObjConfig.msgObjId              = 129;
        canMsgObjConfig.messageId             = 0x200;		// Standard ID, the lowest priority.
        canMsgObjConfig.acceptanceMask        = 0x7FFFFFFFUL;
        canMsgObjConfig.frame                 = IfxMultican_Frame_receive;
        canMsgObjConfig.control.messageLen    = IfxMultican_DataLengthCode_8;
        canMsgObjConfig.control.extendedFrame = FALSE;
        canMsgObjConfig.control.matchingId    = TRUE;
        canMsgObjConfig.msgObjCount			  = 1;//g_MulticanBasic.drivers.canNode0FifoRxSize;		//FIFO size
        canMsgObjConfig.rxInterrupt.enabled = TRUE;
        canMsgObjConfig.rxInterrupt.srcId	= IfxMultican_SrcId_0;
        canMsgObjConfig.priority			= IfxMultican_Priority_CAN_ID;

        /* initialize message object */
        IfxMultican_Can_MsgObj_init(&g_MulticanBasic.drivers.canNode0Can0x200, &canMsgObjConfig);
    }
}

IFX_INTERRUPT(multican_ISR_0x200, 0, 99);
IFX_INTERRUPT(multican_ISR_0x201, 0, 100);

void multican_ISR_0x200(void)
{
	IfxMultican_Can_MsgObj_readMessage(&g_MulticanBasic.drivers.canNode0Can0x200, &gMsg);

	asm(" NOP");

}

void multican_ISR_0x201(void)
{
	IfxMultican_Can_MsgObj_readMessage(&g_MulticanBasic.drivers.canNode0Can0x201, &gMsg);

	asm(" NOP");
}

/**
 * @function
 * @brief
 * Clear the content of CAN_PKT
 * @param[in]	p	A pointer points CAN_PKT
 *
 */
void wMultiCan_ZeroCanPkt(CAN_PKT * const p)
{
	int idx;
	uint8	*ptr;

	if (!p)
		return;

	ptr = (uint8*)p;

	for (idx = 0; idx < sizeof(CAN_PKT); idx++)
		*(ptr + idx) = 0;

}

/**
 *	@function
 *	@brief
 *	This function will put CAN_PKT on TX FIFO of Node 0. Only support standard ID
 *	@param [in]	p	Point to a CAN_PKT
 */
void wMultiCanNode0_send(CAN_PKT const * const p)
{
	IfxMultican_Message msg;

	if (!p)
		return;
	if ((IfxMultican_DataLengthCode)p->dlc < IfxMultican_DataLengthCode_0)
		return;
	if ((IfxMultican_DataLengthCode)p->dlc > IfxMultican_DataLengthCode_8)
		return;
	if (p->id < 1 || p->id > 0x7FF)
		return;

	IfxMultican_Message_init(&msg, p->id, p->MDL.all, p->MDH.all, (IfxMultican_DataLengthCode)p->dlc);
	IfxMultican_Can_MsgObj_sendMessage(&g_MulticanBasic.drivers.canNode0FifoTxBase, &msg);

}

/**
 * @function
 * @brief
 * A demo function of constantly send a CAN message through Node 0
 */
void wMultiCanNode0Demo_run(uint32 dataH, uint32 dataL)
{
	uint32       count   = 0;
	CAN_PKT ecan;

	wMultiCan_ZeroCanPkt(&ecan);

	ecan.id = 0x100;
	ecan.MDH.all = dataH;
	ecan.MDL.all = dataL;
	ecan.dlc = 8;

	/* Demo CAN packet
	 * ID = 0x7FF, DLC = 8
	 * Data = 08 07 06 05 01 02 03 04  (Byte 7 ~ byte 0)
	 */

	wMultiCanNode0_send(&ecan);
}

