
#include <sos/dev/netif.h>
#include <sos/fs/sysfs.h>
#include <mcu/debug.h>
#include <errno.h>

#include "lwip/opt.h"
#include "lwip_api.h"
#include "lwip/sockets.h"
#include "lwip/dhcp.h"

#include "lwip/tcpip.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
#include "netif/ppp/pppoe.h"


static void lwip_input_thread(void * arg); //monitor each input
static int lwip_api_netif_is_link_up(struct netif * netif);
static err_t lwip_api_netif_init(struct netif * netif);
static err_t lwip_api_netif_input(struct netif *netif);
static err_t lwip_api_netif_output(struct netif *netif, struct pbuf *p);
static int lwip_api_add_netif(struct netif * netif, void * state);

err_t lwip_api_netif_init(struct netif * netif){
    /* set MAC hardware address length */
    const lwip_api_config_t * config;
    lwip_api_state_t * state = netif->state;
    config = state->config;

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "Host name is %s", config->host_name);
    netif->hostname = config->host_name;

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "HW Addr length %d", ETHARP_HWADDR_LEN);
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */

    //how to set HW MAC address??
    netif->hwaddr[0]= config->hw_addr[0];
    netif->hwaddr[1]= config->hw_addr[1];
    netif->hwaddr[2]= config->hw_addr[2];
    netif->hwaddr[3]= config->hw_addr[3];
    netif->hwaddr[4]= config->hw_addr[4];
    netif->hwaddr[5]= config->hw_addr[5];

    MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, 10000);


    /* maximum transfer unit */

    //netif->mtu = 1500;
    netif->mtu = config->mtu;
    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif

    netif->linkoutput = lwip_api_netif_output;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

    //need to open and configure the device -- similar to how fatfs opens files

    netif->name[0] = config->device_config.name[0];
    netif->name[1] = config->device_config.name[1];
    //netif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    netif_attr_t attr;
    int result;

    if( (result = sysfs_shared_open(&config->device_config)) < 0 ){
        mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to open network interface %s (%d, %d)", config->device_config.name, result, errno);
        return result;
    }

    attr.o_flags = NETIF_FLAG_INIT;
    result = sysfs_shared_ioctl(&config->device_config, I_NETIF_SETATTR, &attr);
    if( result < 0 ){
        mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to init network interface (%d, %d)", result, errno);
        return ERR_IF;
    }

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "NETIF Init complete");
    return ERR_OK;
}

int lwip_api_netif_is_link_up(struct netif * netif){
    lwip_api_state_t * state = netif->state;
    const lwip_api_config_t * config = state->config;
    netif_attr_t attr;
    attr.o_flags = NETIF_FLAG_IS_LINK_UP;
    return sysfs_shared_ioctl(&config->device_config, I_NETIF_SETATTR, &attr);
}

err_t lwip_api_netif_input(struct netif *netif){
    const lwip_api_config_t * config;
    lwip_api_state_t * state = netif->state;
    config = state->config;
    struct pbuf *q;
    struct pbuf *p;
    u16_t len;

    u8 packet_buffer[config->max_packet_size];
    config = state->config;
    u16 offset = 0;

    /* Obtain the size of the packet and put it into the "len"
     variable. */
    //len = ioctl(netif_dev->fd, I_NETIF_LEN);

    len = sysfs_shared_read(&config->device_config, 0, packet_buffer, config->max_packet_size);
    if( len <= 0 ){
        return ERR_IF;
    }

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "Got %d bytes", len);


#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);


    if (p != NULL) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif


        /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
        for (q = p; q != NULL; q = q->next) {
            /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable.
       * This does not necessarily have to be a memcpy, you can also preallocate
       * pbufs for a DMA-enabled MAC and after receiving truncate it to the
       * actually received size. In this case, ensure the tot_len member of the
       * pbuf is the sum of the chained pbuf len members.
       */

            //scatter the packet into the pbuf
            memcpy(q->payload, packet_buffer + offset, q->len);
            offset += q->len;

        }
        //acknowledge that packet has been read();

        MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
        if (((u8_t*)p->payload)[0] & 1) {
            /* broadcast or multicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
        } else {
            /* unicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        /* if no packet could be read, silently ignore this */
        if (len > 0) {

            /* pass all packets to ethernet_input, which decides what packets it supports */
            if (netif->input(p, netif) != ERR_OK) {
                LWIP_DEBUGF(NETIF_DEBUG, ("netif_dev_input: IP input error\n"));
                pbuf_free(p);
                p = NULL;
            }
        }

        LINK_STATS_INC(link.recv);
    } else {
        //drop packet();
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
    }



    return ERR_OK;
}

err_t lwip_api_netif_output(struct netif *netif, struct pbuf *p){
    const lwip_api_config_t * config;
    lwip_api_state_t * state = netif->state;
    config = state->config;
    struct pbuf *q;

    //initiate transfer();

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "write interface");

    for (q = p; q != NULL; q = q->next) {
        /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
        //send data from(q->payload, q->len);
        if( sysfs_shared_write(&config->device_config, 0, q->payload, q->len) != q->len ){
            return ERR_IF;
        }
    }

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
        /* broadcast or multicast packet*/
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    } else {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    return ERR_OK;
}

int lwip_api_startup(const void * socket_api){

    const lwip_api_config_t * config = ((const sos_socket_api_t*)socket_api)->config;
    lwip_api_state_t * state = ((const sos_socket_api_t*)socket_api)->state;
    u32 i;

    sys_init();

    if( config->network_interface_count == 0 ){
        mcu_debug_log_error(MCU_DEBUG_SOCKET, "No network interfaces");
        return -1;
    } else {

        mcu_debug_log_info(MCU_DEBUG_SOCKET, "TCPIP Init");
        tcpip_init(0, 0);

        usleep(100*1000);

        //state is passed around as part of netif, it needs to link back to config
        state->config = config;

        mcu_debug_log_info(MCU_DEBUG_SOCKET, "0x%lX 0x%lX", (u32)config, (u32)state);
        for(i = 0; i < config->network_interface_count; i++){
            mcu_debug_log_info(MCU_DEBUG_SOCKET, "Add NEFIF %d of %d", i+1, config->network_interface_count);
            if( lwip_api_add_netif(config->network_interface_list + i, state) < 0 ){
                mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to add interface %d of %d", i+1, config->network_interface_count);
                return -1;
            }
        }

        mcu_debug_log_info(MCU_DEBUG_SOCKET, "Set default network interface");

        netif_set_default(config->network_interface_list);

        //allow NETIF to come up
        usleep(250*1000);


        mcu_debug_log_info(MCU_DEBUG_SOCKET, "Start DHCP 0x%lX", config->network_interface_list);
        dhcp_start(config->network_interface_list);


    }


    return 0;
}

int lwip_api_deinitialize(){
    return 0;
}

int lwip_api_add_netif(struct netif * netif, void * state){

    if( getpid() != 0 ){
        //this can only be called from the kernel task
        mcu_debug_log_error(MCU_DEBUG_SOCKET, "Cannot add netif except from kernel");
        return -1;
    }

    netif_add(netif,
          #if LWIP_IPV4
              (ip4_addr_t*)IP4_ADDR_ANY, //ip addr
              (ip4_addr_t*)IP4_ADDR_ANY, //ip netmask
              (ip4_addr_t*)IP4_ADDR_ANY, //gw
          #endif
              state,
              lwip_api_netif_init,
              tcpip_input);

    //create a thread to monitor the new interface - for up/down, etc
    sys_thread_new("netif", lwip_input_thread, netif, 4096, 0);


    mcu_debug_log_info(MCU_DEBUG_SOCKET, "Added netif %s", netif->hostname);
    return 0;
}

void lwip_input_thread(void * arg){
    //this thread monitors each network interface for incoming traffic
    struct netif * netif = (struct netif*)arg;

    mcu_debug_log_info(MCU_DEBUG_SOCKET, "Start thread %s", netif->hostname);

    if( lwip_api_netif_is_link_up(netif) > 0 ){
        mcu_debug_log_info(MCU_DEBUG_SOCKET, "%s is up", netif->hostname);
        netif_set_link_up(netif);
        netif_set_up(netif);
        mcu_debug_log_info(MCU_DEBUG_SOCKET, "Net if 0x%lX is up %d", (u32)netif, netif_is_up(netif));
    } else {
        mcu_debug_log_info(MCU_DEBUG_SOCKET, "%s is down", netif->hostname);
    }

    while(1){
#if 1
        if( netif_is_up(netif) ){
            lwip_api_netif_input(netif);

            //if interface goes up -- call netif_set_link_up()
            //if interface goes down -- call netif_set_link_down()

            if( lwip_api_netif_is_link_up(netif) <= 0 ){
                mcu_debug_log_warning(MCU_DEBUG_SOCKET, "%s link is down", netif->hostname);
                //netif_set_link_down(netif);
            } else {
                //netif_set_link_up(netif);
            }
        }
#endif
        usleep(20*1000);
    }

}
