#!/bin/sh

source exo-utils.sh

MOD_DIR=$(get_cfg_val $HOME/.exo template_path)/mod/.mod

if [ $(basename $(pwd)) != "msg" ]; then
    echo "Command should only be run withing a 'msg' directory"
    exit 1
fi

name=$1
if [ -z $name ]; then
    name=$(prompt "Enter a new module name: ")
fi

if [ -d $name ]; then
    echo "Error: '$name' already exists"
    exit 2
else
    upper_case_name=$(echo $name | awk '{print toupper($0)}')
    cat .msg.hpp | sed s/MSG_NAME_MAGIC/$(echo $upper_case_name)_MAGIC/ | sed s/MSG_NAME/$name/ > $name.hpp
fi
