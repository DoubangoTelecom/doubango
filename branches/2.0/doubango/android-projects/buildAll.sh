#!/bin/bash
# Build tinyDEMO for Google Android Systems

for project in tinySAK tinyNET tinyHTTP tinyXCAP tinyIPSec tinySMS tinySIGCOMP tinySDP tinyRTP tinyMSRP tinyMEDIA tinyDAV tinySIP
do
	echo -e building "$project....\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=$1 all
done
