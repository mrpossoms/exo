#!/bin/bash
source exo-utils.sh

function help {
    echo "Creates a new empty exo project"
    exit 0
}

function usage {
    echo "project [project-name]"
    exit 0
}

PROJ_DIR=$(get_cfg_val $HOME/.exo template_path)
name="$1"

invoke help $name
invoke usage $name

if [ -z $name ]; then
	name=$(prompt "Enter a new project name: ")
fi

if [ -d $PROJ_DIR/$name ]; then
    echo "Error:" $name "already exists"
    exit 1
else
    cp -r $PROJ_DIR $name
fi

os_target=$(prompt "Which OS is being targeted [unix/none]: ")
echo $os_target > $name/os_target
