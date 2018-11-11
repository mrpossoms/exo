#!/bin/bash

CFG_ROOT=$HOME/.exo
DEFAULT_ROOT=/usr/share/exo

if [ -z $EXO_ROOT ]; then
	source $EXO_ROOT ./exo-utils.sh
else
	source $EXO_ROOT/bin/exo-utils.sh
fi

export EXO_DIR=$(dirname $(which $0))
# export EXO_DIR=$(pwd)
SUB_CMD=$1

if [ -z $(printenv | grep EXO_ROOT) ]; then
    echo "Welcome to EXO!"
    echo "Running this command will do some setup, and perform the following actions."
    echo "* Add the EXO_ROOT environment variable to your shell's rc file"
    echo "* Add the EXO_ROOT path to your PATH for running exo commands in the future"
    echo "* Symlink the EXO_ROOT directory to this repository."
    echo
    exo_root=$(prompt "Specify your EXO_ROOT [default $DEFAULT_ROOT]: ")
    dot_file=$(prompt "Specify the full path to your shell's rc file: ")

    if [ -z $exo_root ]; then
        exo_root=$DEFAULT_ROOT
    fi

    sudo ln -s $(pwd)/.. $exo_root 
    on_failure "Couldn't create simlink to '$exo_root'"

    echo "export EXO_ROOT=$exo_root" >> $dot_file
    on_failure "Couldn't append EXO_ROOT to '$dot_file'"

    echo "export PATH=\"$PATH:$exo_root/bin\"" >> $dot_file
    on_failure "Couldn't append to your PATH variable at '$dot_file'"

    echo "All good, please restart your shell."
    exit 0
fi

# check for defaults folder in user's home directory
# if it doesn't exist, create it.
if [ ! -d $CFG_ROOT ]; then
    answer=$(prompt ".exo doesn't exist, create it? [y/n]: ")

    if [ $answer == 'y' ]; then
        create_default_configs
    else
        exit 1
    fi
fi

# check to see if a sub-command has been invoked
if [ $# -lt 1 ]; then
    echo "EXO Commands"
    echo "--------------"
    cmds=$(ls $EXO_ROOT/bin/exo-cmd-*.sh | sed "s|$EXO_ROOT/bin/exo-cmd-||" | sed "s|.sh||")
    for cmd in $cmds; do
        echo "exo $cmd -" $(exo $cmd help)
    done
    exit 0
fi

# try to run the sub command program
# but check to see if it exists
cmd=$EXO_DIR/exo-cmd-$SUB_CMD.sh
which $cmd > /dev/null
if [ $? -eq 0 ]; then
    $EXO_DIR/exo-cmd-$SUB_CMD.sh ${@:2}
else
    echo "'$SUB_CMD' is not a valid command"
fi
