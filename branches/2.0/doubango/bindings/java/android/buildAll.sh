#!/bin/bash
# Build tinyWRAP for Google Android Systems

for project in tinySAK tinyNET tinyHTTP tinyXCAP tinyIPSec tinySMS tinySIGCOMP tinySDP tinyMSRP tinyMEDIA tinyRTP tinyDAV tinySIP
#for project in tinyDAV
do
	echo -e building "$project with parameters=$@\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=static $@ all
done

echo building tinyWRAP....
make PROJECT=tinyWRAP clean
make PROJECT=tinyWRAP BT=shared LT=static $@ all