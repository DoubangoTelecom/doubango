#!/bin/bash
# Build webRTC for Google Android Systems
# Last version known to work: 2425 (4:56:50 PM, Tuesday, June 19, 2012)

for IS_NEON in no yes
do

for project in audio_opensles
do
	if [ $project = "audio_opensles" ]; then \
	   export PLATFORM=android-9; \
	else \
	   export PLATFORM=android-3; \
	fi \
	
	echo -e building "plugin_$project ($PLATFORM)....\n"
	cd $project
	make PROJECT=plugin_$project clean
	make PROJECT=plugin_$project ANDROID_PLATFORM=$PLATFORM BT=shared NEON=$IS_NEON all
	cd ..
done

done
