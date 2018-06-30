
set(SOURCES
${SOURCES_PREFIX}/netif_dev.c
${SOURCES_PREFIX}/sys_arch.c
${SOURCES_PREFIX}/lwip_api.c

${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/api_lib.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/api_msg.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/err.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/netbuf.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/netdb.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/netifapi.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/sockets.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/api/tcpip.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/def.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/dns.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/inet_chksum.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/init.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ip.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/mem.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/memp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/netif.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/pbuf.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/raw.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/stats.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/sys.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/tcp_in.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/tcp_out.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/tcp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/timeouts.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/udp.c

${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/autoip.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/dhcp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/etharp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/icmp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/igmp.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/ip4_addr.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/ip4_frag.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv4/ip4.c

${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/ethip6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/dhcp6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/icmp6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/ip6_addr.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/ip6_frag.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/ip6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/mld6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/core/ipv6/nd6.c

${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ppp/polarssl/arc4.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ppp/polarssl/des.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ppp/polarssl/md4.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ppp/polarssl/md5.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ppp/polarssl/sha1.c

${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/ethernet.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/lowpan6.c
${SOURCES_PREFIX}/${LWIP_FOLDER}/src/netif/slipif.c

PARENT_SCOPE)
