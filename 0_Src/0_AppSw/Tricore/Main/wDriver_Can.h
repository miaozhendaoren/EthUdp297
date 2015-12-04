
/** \file	wDriver_Can.h
 *
 *	\date	2014/12/27
 *	\author	Crow CY Liao
 */

#ifndef WDRIVER_CAN_H_
#define WDRIVER_CAN_H_

#include <Ifx_Types.h>
#include <Multican/Can/IfxMultican_Can.h>

/** \brief Definition of node 0 TX fifo size */
#define	NODE0_TX_FIFO_SIZE	8
#define NODE0_RX_FIFO_SIZE	8

/** \brief Baudrate of Node 0 */
#define NODE0_BAUDRATE		500000

/** \brief Structure of CAN Information */
typedef struct
{
    struct
    {
		uint32					baudrateNode0;			/**< \brief Node 0 baudrate */
        IfxMultican_Can			can;					/**< \brief CAN driver handle */
        IfxMultican_Can_Node	canNode0;				/**< \brief CAN Node 0 */
        IfxMultican_Can_MsgObj	canNode0FifoTxBase;		/**< \brief CAN Node 0 Tx FiFo Message object */
        unsigned int			canNode0FifoTxSize;		/**< \brief TX FIFO size of Node 0 */
        IfxMultican_Can_MsgObj	canNode0FifoRxBase;		/**< \brief CAN Node 0 Tx FiFo Message object */
        unsigned int			canNode0FifoRxSize;		/**< \brief TX FIFO size of Node 0 */
        IfxMultican_Can_MsgObj	canNode0Can0x200;			/**< \brief CAN Node 0 Tx FiFo Message object */
        IfxMultican_Can_MsgObj	canNode0Can0x201;			/**< \brief CAN Node 0 Tx FiFo Message object */
    } drivers;

} w_MulticanBasic;

struct CANDL_BYTE {
	uint8	byte0;
	uint8	byte1;
	uint8	byte2;
	uint8	byte3;
};

struct CANDH_BYTE {
	uint8	byte4;
	uint8	byte5;
	uint8	byte6;
	uint8	byte7;
};

/**
 * \brief
 * High word of CAN data, byte 4 ~ 7
 */
union CANMDH_REG {
	uint32	all;
	struct	CANDH_BYTE	byte;
};

/**
 * \brief
 * Low word of CAN data, byte 0 ~ 3
 */
union CANMDL_REG {
	uint32	all;
	struct CANDL_BYTE byte;
} ;

/**
 * \brief
 * CAK packet structure
 */
typedef struct {
	uint32	id;				/**< CAN Msg ID */
	uint8	dlc;			/**< CAN Msg DLC */
	union CANMDL_REG MDL;	/**< Data byte 0 - 3 */
	union CANMDH_REG MDH;	/**< Data byte 4 - 7 */
} CAN_PKT;


void wMultican_init(void);
void wMultiCanNode0Demo_run(uint32, uint32);

void wMultiCanNode0_send(CAN_PKT const * const p);
void wMultiCan_ZeroCanPkt(CAN_PKT * const p);

#endif /* WDRIVER_CAN_H_ */
