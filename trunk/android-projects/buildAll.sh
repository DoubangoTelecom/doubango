#!/bin/bash
# Build tinyDEMO for Google Android Systems

for project in tinySAK tinyNET tinyIPSec tinySMS tinySIGCOMP tinyHTTP tinySDP tinyRTP tinyMEDIA tinySIP tinyDAV tinyDEMO
do
	echo -e building "$project....\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=$1 all
done
