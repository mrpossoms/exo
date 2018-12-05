#!/bin/bash

exo_root=$1
rc=_file=$2
is_ci_test=false

if [ ! -z $1 ]; then
	if [ $1 = "ci_test" ]; then 
		is_ci_test=true
		exo_root=/usr/share/exo
		rc_file=$HOME/.bashrc
	fi
fi

cd bin
./exo $exo_root $rc_file
cd ..

dot_file_path=$(cat /tmp/$USER.dotfile)
exo_root_path=$(cat /tmp/$USER.exo_root)

if [ -f $dot_file_path ]; then
	if [ $is_ci_test ]; then
		echo "Using dotfile: " $dot_file_path

		echo "[Dotfile contents] ---------------"
		cat $dot_file_path
		
		echo "[Env vars] -----------------------"
		printenv
	else
		source $dot_file_path
		make install
	fi
fi

