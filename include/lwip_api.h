#ifndef LWIP_API_H_
#define LWIP_API_H_

#include <sys/socket.h>
#include <sos/fs/devfs.h>
#include "lwip/netif.h"

extern const sos_socket_api_t lwip_api;

typedef struct MCU_PACK {
    u8 hw_addr[6];
    u16 mtu /*! Default value should be 1500 */;
    sysfs_shared_config_t drive_config;
    const void * netif_device_attr;
} lwip_api_config_t;

typedef struct MCU_PACK {
    err_t (*init_function)(struct netif *);
    err_t (*input_function)(struct pbuf *, struct netif *);
} lwip_api_netif_t;

typedef struct {
    const lwip_api_config_t * config;
    sysfs_shared_state_t drive_state;
} lwip_api_netif_state_t;

int lwip_api_initialize(const lwip_api_config_t * config);
int lwip_api_deinitialize();
int lwip_api_add_netif(lwip_api_netif_state_t * state);

#endif /* LWIP_HDR_LWIPOPTS_H__ */
