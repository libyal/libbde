#
# Regular cron jobs for the libbde package
#
0 4	* * *	root	[ -x /usr/bin/libbde_maintenance ] && /usr/bin/libbde_maintenance
