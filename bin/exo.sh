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

# Perform first time setup if the EXO_ROOT env var is
# missing from the user's environment
if [ -z $(printenv | grep EXO_ROOT) ]; then
    echo "Welcome to EXO!"
    echo "Running this command will do some setup, and perform the following actions."
    echo "* Add the EXO_ROOT environment variable to your shell's rc file"
    echo "* Add the EXO_ROOT path to your PATH for running exo commands in the future"
    echo "* Symlink the EXO_ROOT directory to this repository."
    echo

    # if one argument has been provided, try to use it as the user's dot file
    # and the default exo root
    if [ ! -z $1 ]; then
        exo_root=$1
        dot_file=$2
    else
    # otherwise go through the normal prompts
	exo_root=$(prompt "Specify your EXO_ROOT [default $DEFAULT_ROOT]: ")
	dot_file=$(prompt "Specify the full path to your shell's rc file: ")
    fi

    if [ -z $exo_root ]; then
        exo_root=$DEFAULT_ROOT
    fi

    ln -s $(pwd)/.. $exo_root 
    on_failure "Couldn't create simlink to '$exo_root'"

    cp $dot_file $dot_file.exo.bk
    echo "export EXO_ROOT=$exo_root" >> $dot_file
    on_failure "Couldn't append EXO_ROOT to '$dot_file'"

    echo "export PATH=\"$PATH:$exo_root/bin\"" >> $dot_file
    on_failure "Couldn't append to your PATH variable at '$dot_file'"
    echo $dot_file > /tmp/$USER.dotfile
    echo $exo_root > /tmp/$UESR.exo_root

    create_default_configs $dot_file

    echo "All good, please restart your shell."
    exit 0
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
