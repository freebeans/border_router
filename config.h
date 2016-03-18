/* Configurações */
#ifndef _BORDER_ROUTER_CFG_H_
#define _BORDER_ROUTER_CFG_H_


/* Endereço IPv6 do host (endereço do tunslip) [string] */
#define IPV6_HOST_SLIP_INTERFACE_CONFIG "aaaa::1"



/* Endereço IPv6 da interface serial no RIOT [string] */
#define IPV6_WIRED_ADDR_CONFIG "aaaa::2"
#define IPV6_WIRED_ADDR_PREFIX_CONFIG 64
/* Prefixo do endereço IPv6 (tamanho) [número] */



/* Endereço IPv6 da interface sem fio no RIOT [string] */
#define IPV6_WIRELESS_ADDR_CONFIG "aaaa::3"
#define IPV6_WIRELESS_ADDR_PREFIX_CONFIG 64
/* Prefixo do endereço IPv6 (tamanho) [número] */



/* Canal que a interface sem fio deve usar (entre 11 e 26) [número] */
#define WIRELESS_CHANNEL 26
#define PANID 0xBEA1
/* PAN ID que a interface sem fio deve usar [número] */



#endif
