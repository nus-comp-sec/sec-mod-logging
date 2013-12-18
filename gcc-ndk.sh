#!/bin/bash
if [ $# -eq 0 ]
then
    git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6
    git clone https://android.googlesource.com/platform/prebuilts/ndk
elif [ "$1" == "http" ]
then
    echo "Downloanding over HTTP ..."
    git clone http://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6
    git clone http://android.googlesource.com/platform/prebuilts/ndk
elif [ "$1" == "cache" ]
then
    echo "Downloading from cache ..."
    wget http://www.comp.nus.edu.sg/~liangzk/downloads/gcc-ndk.tar.gz
    tar zxvf gcc-ndk.tar.gz > /dev/null
fi
