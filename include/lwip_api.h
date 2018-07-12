#ifndef LWIP_API_H_
#define LWIP_API_H_

#include <sys/socket.h>
#include <sos/fs/devfs.h>

#include "lwip/netif.h"
#include "lwip/sockets.h"

typedef struct MCU_PACK {
    sysfs_shared_config_t device_config; //make this the first member
    u8 hw_addr[6];
    u16 mtu /*! Default value should be 1500 */;
    const char * host_name;
    struct netif * network_interface_list;
    u16 network_interface_count;
    u16 max_packet_size;
} lwip_api_config_t;

typedef struct MCU_PACK {
    err_t (*init_function)(struct netif *);
    err_t (*input_function)(struct pbuf *, struct netif *);
} lwip_api_netif_t;

typedef struct {
    sysfs_shared_state_t device_state;
    const lwip_api_config_t * config;
} lwip_api_state_t;

int lwip_api_startup(const void * socket_api);
int lwip_api_deinitialize();

#define LWIP_DECLARE_CONFIG_STATE(name_value, network_interface_count_value, device_filesystem_value, device_name_value, host_name_value, mtu_value, hw_addr_0_value, hw_addr_1_value, hw_addr_2_value, hw_addr_3_value, hw_addr_4_value, hw_addr_5_value) \
    lwip_api_netif_state_t name_value##_state; \
    struct netif name_value##_network_interface_list[network_interface_count_value]; \
    lwip_api_config_t name_value##_config = { \
    .device_config.devfs = device_filesystem_value, .device_config.name = device_name_value, .device_config.state = name_value##_state, \
    .hw_addr[0] = hw_addr_0_value, .hw_addr[1] = hw_addr_1_value, .hw_addr[2] = hw_addr_2_value, .hw_addr[3] = hw_addr_3_value, .hw_addr[4] = hw_addr_4_value, .hw_addr[5] = hw_addr_5_value, \
    .mtu = mtu_value, \
    .netif_device_attr = netif_device_attr_value, \
    .host_name = host_name_value, \
    .network_interface_list = name_value##_network_interface_list, \
    .network_interface_count = network_interface_count_value }


#define LWIP_DECLARE_SOCKET_API(api_name, api_config, api_state) \
    const sos_socket_api_t api_name##_api = { \
    .startup = lwip_api_startup, \
    .accept = lwip_accept, \
    .bind = lwip_bind, \
    .shutdown = lwip_shutdown, \
    .getpeername = lwip_getpeername, \
    .getsockname = lwip_getsockname, \
    .getsockopt = lwip_getsockopt, \
    .setsockopt = lwip_setsockopt, \
    .close = lwip_close, \
    .connect = lwip_connect, \
    .read = lwip_read, \
    .listen = lwip_listen, \
    .recv = lwip_recv, \
    .recvfrom = lwip_recvfrom, \
    .send = lwip_send, \
    .sendmsg = lwip_sendmsg, \
    .sendto = lwip_sendto, \
    .socket = lwip_socket, \
    .write = lwip_write, \
    .writev = lwip_writev, \
    .select = lwip_select, \
    .ioctl = lwip_ioctl, \
    .fcntl = lwip_fcntl, \
    .config = api_config, \
    .state = api_state }

#endif /* LWIP_HDR_LWIPOPTS_H__ */
