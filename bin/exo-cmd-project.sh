#!/bin/sh

source exo-utils.sh

PROJ_DIR=$(get_cfg_val $HOME/.exo template_path)

name=$(prompt "Enter a new project name: ")
if [ -d $PROJ_DIR/$name ]; then
    echo "Error:" $name "already exists"
    exit 1
else
    cp -r $PROJ_DIR $name
fi

os_target=$(prompt "Which OS is being targeted [unix/none]: ")
echo $os_target >> $name/os_target
