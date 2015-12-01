#!/bin/bash
# Build Android Utils for Google Android Systems

for IS_NEON in no yes
do

for project in utils
do
	echo -e building "$project....\n"
	make PROJECT=$project clean
	make PROJECT=$project BT=shared NEON=$IS_NEON all
done

done
