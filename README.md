pnr-prefix-class-ext
====================

Build:

	make PNR_HOST=hostname

Install:

	nrcmd> extension prefix_class_set_class create dex libprefix_class.so prefix_class_set_class init-entry=prefix_class_initentry
	nrcmd> dhcp attachExtension pre-packet-encode prefix_class_set_class
	nrcmd> dhcp reload

