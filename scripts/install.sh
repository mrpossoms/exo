#!/bin/bash

exo_root=$1
rc_file=$2

if [ $1 = "ci-test" ]; then
	is_ci_test=true
	exo_root=/usr/share/exo
	rc_file=/etc/profile
fi

cd bin
./exo $exo_root $rc_file
cd ..

dot_file_path=$(cat /tmp/$USER.dotfile)
exo_root_path=$(cat /tmp/$USER.exo_root)

if [ -f $dot_file_path ]; then
	if [ $is_ci_test = true ]; then
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

