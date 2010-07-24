#!/bin/bash
# Build tinyWRAP for Google Android Systems

for project in tinySAK tinyNET tinyHTTP tinyXCAP tinyIPSec tinySMS tinySIGCOMP tinySDP tinyRTP tinyMEDIA tinyDAV tinySIP
#for project in tinyNET
do
	echo -e building "$project....\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=static all
done

echo building tinyWRAP....
make PROJECT=tinyWRAP clean
make PROJECT=tinyWRAP BT=shared all