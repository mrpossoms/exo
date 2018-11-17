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

dot_file_path=/tmp/$USER.dotfile
if [ -f $dot_file_path ]; then
	path=$(cat $dot_file_path)
	echo "Using dotfile: " $path
	echo "[Dotfile contents]"
	cat $path
	source $path
	make install
fi

