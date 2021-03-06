# name of your application
APPLICATION = border_router_awges

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

BOARD_INSUFFICIENT_MEMORY := airfy-beacon msb-430 msb-430h pca10000 pca10005 \
                             nrf51dongle nrf6310 nucleo-f103 nucleo-f334 \
                             spark-core stm32f0discovery telosb \
                             weio wsn430-v1_3b wsn430-v1_4 yunjia-nrf51822 z1

ifeq (,$(SLIP_UART))
  # set default (last available UART)
  SLIP_UART="UART_DEV(UART_NUMOF-1)"
endif
ifeq (,$(SLIP_BAUDRATE))
  # set default
  SLIP_BAUDRATE=115200
endif

GNRC_NETIF_NUMOF := 2
INCLUDES += -I$(CURDIR)
CFLAGS += -DSLIP_UART=$(SLIP_UART)
CFLAGS += -DSLIP_BAUDRATE=$(SLIP_BAUDRATE)

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netif_default
USEMODULE += auto_init_gnrc_netif

USEMODULE += gnrc_rpl
USEMODULE += gnrc_pktdump
# Include SLIP package for IP over Serial communication
USEMODULE += gnrc_slip
# Specify the mandatory networking modules for 6LoWPAN border router
USEMODULE += gnrc_sixlowpan_border_router_default
# Add forwarding table
USEMODULE += fib
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
