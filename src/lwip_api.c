
#include <sos/dev/netif.h>
#include <sos/fs/sysfs.h>

#include "lwip/opt.h"
#include "lwip_api.h"
#include "lwip/sockets.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"

const sos_socket_api_t lwip_api = {
        .accept = lwip_accept,
        .bind = lwip_bind,
        .shutdown = lwip_shutdown,
        .getpeername = lwip_getpeername,
        .getsockname = lwip_getsockname,
        .getsockopt = lwip_getsockopt,
        .setsockopt = lwip_setsockopt,
        .close = lwip_close,
        .connect = lwip_connect,
        .read = lwip_read,
        .listen = lwip_listen,
        .recv = lwip_recv,
        .recvfrom = lwip_recvfrom,
        .send = lwip_send,
        .sendmsg = lwip_sendmsg,
        .sendto = lwip_sendto,
        .socket = lwip_socket,
        .write = lwip_write,
        .writev = lwip_writev,
        .select = lwip_select,
        .ioctl = lwip_ioctl,
        .fcntl = lwip_fcntl
};

static err_t lwip_api_netif_init(struct netif * netif){
    /* set MAC hardware address length */
    const lwip_api_config_t * config;
    lwip_api_netif_state_t * state = netif->state;
    config = state->config;

    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */

    //how to set HW MAC address??
    netif->hwaddr[0]= config->hw_addr[0];
    netif->hwaddr[1]= config->hw_addr[1];
    netif->hwaddr[2]= config->hw_addr[2];
    netif->hwaddr[3]= config->hw_addr[3];
    netif->hwaddr[4]= config->hw_addr[4];
    netif->hwaddr[5]= config->hw_addr[5];


    /* maximum transfer unit */

    //netif->mtu = 1500;
    netif->mtu = config->mtu;


    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

    //need to open and configure the device -- similar to how fatfs opens files

    netif->name[0] = config->drive_config.name[0];
    netif->name[1] = config->drive_config.name[1];
    netif_dev->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    netif_attr_t attr;
    int result;

    if( (result = sysfs_shared_open(&config->drive_config)) < 0 ){
        return result;
    }

    attr.o_flags = NETIF_FLAG_INIT;
    return sysfs_shared_ioctl(&config->drive_config, I_NETIF_SETATTR, &attr);


}

static void lwip_api_netif_input(struct netif *netif){

}

int lwip_api_initialize(lwip_api_netif_state_t * state, int count){
    int i;
    for(i=0; i < count; i++){
        if( lwip_api_add_netif(state[i]) < 0 ){
            return -1;
        }
    }

    return 0;
}

int lwip_api_deinitialize(){

}

int lwip_api_add_netif(lwip_api_netif_state_t * state){

    if( getpid() != 0 ){
        //this can only be called from the kernel task
        return -1;
    }

    netif_add(0, 0, 0, 0, state, lwip_api_netif_init, lwip_api_netif_input);


}
