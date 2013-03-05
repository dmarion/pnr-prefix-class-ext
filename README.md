pnr-prefix-class-ext
====================

Build:

	make PNR_HOST=hostname

Install:

	nrcmd> extension prefix_class_set_class create dex libprefix_class.so prefix_class_set_class init-entry=prefix_class_initentry
	nrcmd> dhcp attachExtension pre-packet-encode prefix_class_set_class
	nrcmd> dhcp reload

Sample config:

	prefix NA create 2001:ffff:ffff::/64 dhcp-type=dhcp link=link1
	prefix PD1 create 2001:ffff:1111::/48 dhcp-type=prefix-delegation link=link1
	prefix PD2 create 2001:ffff:2222::/48 dhcp-type=prefix-delegation link=link1
	prefix PD3 create 2001:ffff:3333::/48 dhcp-type=prefix-delegation link=link1
	prefix PD1 set allocation-group=prefix-class:11
	prefix PD2 set allocation-group=prefix-class:22
	prefix PD3 set allocation-group=prefix-class:33
	dhcp reload
