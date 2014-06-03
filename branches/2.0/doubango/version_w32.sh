###
# Copyright (C) 2013 Mamadou DIOP
###
VERSION_MAJOR=2
VERSION_MINOR=0
VERSION_MICRO=0
SVN_VERSION=$(svnversion -n)

export IFS=":"
i=0
for word in $SVN_VERSION; do
  SVN_REVISION[i++]="$word"
done

# path
UpdateVersion()
{
    echo "Processing: $1"
    sed -i "s/[0-9]\+\,[0-9]\+\,[0-9]\+\,[0-9]\+/$VERSION_MAJOR\,$VERSION_MINOR\,$VERSION_MICRO\,${SVN_REVISION[0]}/g" $1
    sed -i "s/[0-9]\+\.[0-9]\+\.[0-9]\+\.[0-9]\+/$VERSION_MAJOR\.$VERSION_MINOR\.$VERSION_MICRO\.${SVN_REVISION[0]}/g" $1
    
    #sed -i "s/2.0.0.1113M/2.0.0.1113/g" $1
    #sed -i "s/2,0,0,1113M/2,0,0,1113/g" $1
}

UpdateVersion tinyBFCP/version.rc
UpdateVersion tinyDAV/version.rc
UpdateVersion tinyHTTP/version.rc
UpdateVersion tinyIPSec/version.rc
UpdateVersion tinyMEDIA/version.rc
UpdateVersion tinyMSRP/version.rc
UpdateVersion tinyNET/version.rc
UpdateVersion tinyRTP/version.rc
UpdateVersion tinySAK/version.rc
UpdateVersion tinySDP/version.rc
UpdateVersion tinySIGCOMP/version.rc
UpdateVersion tinySIP/version.rc
UpdateVersion tinySMS/version.rc
UpdateVersion tinyXCAP/version.rc
UpdateVersion bindings/version.rc

UpdateVersion plugins/pluginCUDA/version.rc
UpdateVersion plugins/pluginDirectShow/version.rc
UpdateVersion plugins/pluginWASAPI/version.rc
UpdateVersion plugins/pluginWinAudioDSP/version.rc
UpdateVersion plugins/pluginWinMF/version.rc
UpdateVersion plugins/pluginWinIPSecVista/version.rc
