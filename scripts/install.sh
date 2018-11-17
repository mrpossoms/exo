#!/bin/bash

exo_root=$1
rc_file=$2

if [ $1 = "ci-test" ]; then

	exo_root=/usr/share/exo
	rc_file=$HOME/.bashrc
fi

cd bin
./exo $exo_root $rc_file
cd ..

dot_file_path=$(cat /tmp/$USER.dotfile)
exo_root_path=$(cat /tmp/$USER.exo_root)

if [ -f $dot_file_path ]; then
	echo "Using dotfile: " $dot_file_path
	echo "[Dotfile contents] ---------------"
	cat $dot_file_path
	export EXO_ROOT=$(cat $exo_root_path)
	export PATH="$PATH:$EXO_ROOT/bin"
	echo "[Env vars] -----------------------"
	printenv
	make install
fi

