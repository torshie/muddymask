#!/bin/sh

if [ -f /proc/cpuinfo ]; then
	grep cpu /proc/cpuinfo | wc -l
else
	echo 1
fi
