#!/bin/sh

source exo-utils.sh

MOD_DIR=$(get_cfg_val $HOME/.exo template_path)/mod/.mod

if [ $(basename $(pwd)) != "mod" ]; then
    echo "Command should only be run withing a 'mod' directory"
    exit 1
fi

echo "Enter a new module name"
read -r name
if [ -d $name ]; then
    echo "Error: '$name' already exists"
    exit 2
else
    cp -r $MOD_DIR $name
fi
