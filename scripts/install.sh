#!/bin/sh

exo_root=$1
rc_file=$2

if [ $1 = "ci-test" ]; then

	exo_root=/usr/share/exo
	rc_file=$HOME/.bashrc
fi

cd bin
./exo $exo_root $rc_file
cd ..

dot_file_path=/tmp/$USER
if [ -f /tmp/$USER ]; then
	path=$(cat /tmp/$USER)
	make install
fi

