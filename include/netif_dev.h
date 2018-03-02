/*
 * netif_dev.h
 *
 *  Created on: Apr 17, 2017
 *      Author: tgil
 */

#ifndef LWIP_NETIF_DEV_H_
#define LWIP_NETIF_DEV_H_


#include <mcu/types.h>

typedef struct MCU_PACK {
	u8 hw_addr[6];
	u16 mtu /*! Default value should be 1500 */;
	const char * dev /*! Path to network interface device */;
	const char * host_name /*! Host name */;
} netif_dev_config_t;



#endif /* LWIP_NETIF_DEV_H_ */
