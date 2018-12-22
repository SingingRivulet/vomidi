#!/bin/sh
stpath=$1
comp="g++ -std=c++11"
if [ -z "$stpath" ]; then
    echo "$0 [soundTouch path]"
    exit
fi
stincpath="$stpath/include"
ssincpath="$stpath/source/SoundStretch"
cxxcmd="$comp -I $stincpath -I $ssincpath"
echo ""
echo "soundTouch include:$stincpath"
echo "SoundStreth:$ssincpath"
echo "cxxcommand:$cxxcmd"
echo ""
echo "finding soundTouch..."

if [ ! -d $stincpath ]; then
    echo "no such directory:$stincpath"
    exit;
fi

if [ ! -d $ssincpath ]; then
    echo "no such directory:$ssincpath"
    exit;
fi

echo "OK"
echo "building Makefile"
echo "cxxcmd=\"$cxxcmd\"" > src/Makefile
cat src/Makefile.in >> src/Makefile
echo "all:" > Makefile
echo  "        cd src && make" >> Makefile
