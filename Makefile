PNR_HOST	= lab-pnr
DEX_DIR		= /opt/nwreg2/local/extensions/dhcp/dex
INCLUDE_DIR	= /opt/nwreg2/local/examples/dhcp/dex
CARGS		= -g -fpic -MD -Wall -D_REENTRANT -shared -lgcc -I $(INCLUDE_DIR)

all:
	scp prefix_class.c $(PNR_HOST):/${DEX_DIR}/prefix_class.c
	ssh $(PNR_HOST) 'gcc $(CARGS) ${DEX_DIR}/prefix_class.c -o ${DEX_DIR}/libprefix_class.so '