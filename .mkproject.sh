#!/bin/sh

PROJ_DIR=$1
echo "Enter a new project name"
read -r name
if [ -d $PROJ_DIR/$name ]; then
	echo "Error:" $name "already exists"
	exit 1
else
	cp -r $PROJ_DIR/.project $PROJ_DIR/$name
fi
