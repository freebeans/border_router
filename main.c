#include <stdio.h>
#include "shell.h"
#include "msg.h"
#include "config.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/ipv6/nc.h"
#include "net/gnrc/rpl.h"

void detecta_interfaces(kernel_pid_t *);
void configura_interfaces(kernel_pid_t *);

enum{WIRED=0, WIRELESS=1, ifaceCount}; /* facilita compreensão (ifaceCount é a quantidade de interfaces) */

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];


int main(void)
{
	msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

	puts("Border Router AWGES");

	{	/* configura interfaces de rede */
		kernel_pid_t interfaces[ifaceCount];
		detecta_interfaces(interfaces);
		configura_interfaces(interfaces);
	}


	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);


	return 0;
}

void detecta_interfaces(kernel_pid_t * out_ifaces){
	// recebe a lista de interfaces de rede
	kernel_pid_t get_ifaces[GNRC_NETIF_NUMOF];
    size_t numof = gnrc_netif_get(get_ifaces);

	for(size_t i=0; i<numof; i++){	// itera sobre as interfaces retornadas
		gnrc_ipv6_netif_t *entry = gnrc_ipv6_netif_get(get_ifaces[i]);	// recebe estrutura relativa à interface
		if(entry->flags & GNRC_IPV6_NETIF_FLAGS_IS_WIRED)			// testa se a interface é cabeada
			out_ifaces[0] = get_ifaces[i];
		else
			out_ifaces[1] = get_ifaces[i];
	}
}

void configura_interfaces(kernel_pid_t * interface){
	
	ipv6_addr_t addr;
	
	{	/* ifconfig [WIRED] add aaaa::2 */
		
		printf("Adicionando %s `a interface SLIP\n", IPV6_WIRED_ADDR_CONFIG);
		if( ipv6_addr_from_str(&addr, IPV6_WIRED_ADDR_CONFIG) == NULL )
			puts("Erro ao converter IPv6 Wireless @ configura_interfaces() / ifconfig WIRED\n");
			
		int flags = GNRC_IPV6_NETIF_ADDR_FLAGS_NDP_AUTO | GNRC_IPV6_NETIF_ADDR_FLAGS_UNICAST;
		
		if( gnrc_ipv6_netif_add_addr(interface[WIRED], &addr, IPV6_WIRED_ADDR_PREFIX_CONFIG, flags) == NULL )
			puts("Erro ao adicionar endereco IPv6 @ configura_interfaces() / ifconfig WIRED\n");
	}
	
	{	/* ifconfig [WIRELESS] set channel WIRELESS_CHANNEL */
		unsigned int res = WIRELESS_CHANNEL;
		gnrc_netapi_set(interface[WIRELESS], NETOPT_CHANNEL, 0, (uint16_t *)&res, sizeof(uint16_t));
	}
	
	{	/* ifconfig [WIRELESS] set pan_id PANID */
		unsigned int res = PANID;
		gnrc_netapi_set(interface[WIRELESS], NETOPT_NID, 0, (uint16_t *)&res, sizeof(uint16_t));
	}
	
	{	/* ifconfig [WIRELESS] add aaaa::3 */
		
		printf("Adicionando %s `a interface WIRELESS\n", IPV6_WIRELESS_ADDR_CONFIG);
		if( ipv6_addr_from_str(&addr, IPV6_WIRELESS_ADDR_CONFIG) == NULL)
			puts("Erro ao converter IPv6 Wireless @ configura_interfaces() / ifconfig WIRELESS\n");
			
		int flags = GNRC_IPV6_NETIF_ADDR_FLAGS_NDP_AUTO | GNRC_IPV6_NETIF_ADDR_FLAGS_UNICAST;
		
		if( gnrc_ipv6_netif_add_addr(interface[WIRELESS], &addr, IPV6_WIRELESS_ADDR_PREFIX_CONFIG, flags) == NULL )
			puts("Erro ao adicionar endereco IPv6 @ configura_interfaces() / ifconfig WIRELESS\n");
	}
	
	{	/* ncache add [WIRED] aaaa::1 */
		
		printf("Adicionando %s ao neighbor cache da interface SLIP\n", IPV6_HOST_SLIP_INTERFACE_CONFIG);
		if( ipv6_addr_from_str(&addr, IPV6_HOST_SLIP_INTERFACE_CONFIG) == NULL )
			puts("Erro ao converter IPv6 Wireless @ configura_interfaces() / ncache\n");
			
		if( gnrc_ipv6_nc_add(interface[WIRED], &addr, NULL, 0, 0) == NULL)
			puts("Erro ao adicionar endereco ao neighbor cache @ configura_interfaces() / ncache\n");
	}
	
	{	/* rpl init [WIRELESS] */
		puts("Iniciando RPL na interface sem fio");
		gnrc_rpl_init(interface[WIRELESS]);
	}
	
	{	/* rpl root [WIRELESS] aaaa::3 */
		puts("Adicionando DODAG `a intstancia RPL");
		if( ipv6_addr_from_str(&addr, IPV6_WIRELESS_ADDR_CONFIG) == NULL )
			puts("Erro ao converter IPv6 Wireless @ configura_interfaces()\n");
			
		if( gnrc_rpl_root_init(interface[WIRELESS], &addr, false, false) == NULL )
			puts("Erro ao adicionar DODAG `a instancia RPL @ configura_interfaces()\n");
	}
	
	
	
}




