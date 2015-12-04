/*
 * vars.h
 *
 *  Created on: 2015/12/4
 *      Author: 10105002
 */

#ifndef VARS_H_
#define VARS_H_


struct stat_report {
	uint32	phy_link: 1;
	uint32	ethRam: 1;
	uint32	position: 8;
	uint32	dummy: 6;
	uint32	mdio_stat:16;
};

extern volatile struct stat_report report;

#endif /* VARS_H_ */
