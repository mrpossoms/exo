#!/bin/sh

CFG_ROOT=$HOME/.exo

source exo-utils.sh

export EXO_DIR=$(dirname $(which $0))
# export EXO_DIR=$(pwd)
SUB_CMD=$1

# check for defaults folder in user's home directory
# if it doesn't exist, create it.
if [ ! -d $CFG_ROOT ]; then
    answer=$(prompt ".exo doesn't exist, create it? [y/n] ")

    if [ $answer == 'y' ]; then
        create_default_configs
    else
        exit 1
    fi
fi

# check to see if a sub-command has been invoked
if [ $# -lt 1 ]; then
    # TODO show usage help
    exit 0
fi

# try to run the sub command program
# but check to see if it exists
cmd=$EXO_DIR/exo-cmd-$SUB_CMD.sh
which $cmd
if [ $? -eq 0 ]; then
    $EXO_DIR/exo-cmd-$SUB_CMD.sh ${@:2}
else
    echo "'$SUB_CMD' is not a valid command"
fi
