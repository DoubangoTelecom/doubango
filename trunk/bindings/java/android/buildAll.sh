#!/bin/bash
# Build tinyWRAP for Google Android Systems

#export CFLAGS="-Os"

for project in tinySAK tinyNET tinyIPSec tinySMS tinyHTTP tinySIGCOMP tinySDP tinyRTP tinyMEDIA tinyDAV tinySIP
do
	echo -e building "$project....\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=static all
done

echo building tinyWRAP....
make PROJECT=tinyWRAP clean
make PROJECT=tinyWRAP BT=shared all