#!/bin/sh
source exo-utils.sh

function help {
    echo "Creates a blank module. Must be used inside a 'mod' directory"
    exit 0
}

function usage {
    echo "mod [module-name]"
    exit 0
}

MOD_DIR=$(get_cfg_val $HOME/.exo template_path)/mod/.mod

if [ $(basename $(pwd)) != "mod" ]; then
    echo "Command should only be run withing a 'mod' directory"
    exit 1
fi

name=$1

invoke help $name
invoke usage $name

if [ -z $name ]; then
	name=$(prompt "Enter a new module name: ")
fi

if [ -d $name ]; then
    echo "Error: '$name' already exists"
    exit 2
else
    cp -r $MOD_DIR $name
    cat $name/MOD_NAME.cpp | sed s/MOD_NAME/$name/ > $name/$name.cpp
    rm $name/MOD_NAME.cpp
fi
