#!/bin/sh

source exo-utils.sh

PROJ_DIR=$(get_cfg_val $HOME/.exo template_path)

echo "Enter a new project name"
read -r name
if [ -d $PROJ_DIR/$name ]; then
    echo "Error:" $name "already exists"
    exit 1
else
    cp -r $PROJ_DIR $name
fi
