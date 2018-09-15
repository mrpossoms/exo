#!/bin/sh

MOD_DIR=$1
echo "Enter a new module name"
read -r name
if [ -d $MOD_DIR/$name ]; then
	echo "Error:" $name "already exists"
	exit 1
else
	cp -r $MOD_DIR/.mod $MOD_DIR/$name
	mkdir $MOD_DIR/$name/msgs/rx
	mkdir $MOD_DIR/$name/msgs/tx
	$(cat ../.editor-pref) $MOD_DIR/$name/$name.c
fi
