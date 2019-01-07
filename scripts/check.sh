#!/bin/bash

# Checks for the core components of a valid
# exo installation

if [ -z $EXO_ROOT ]; then
	exit 1
fi

if [ ! -d /usr/local/include/exo ]; then
	exit 2
fi

if [ ! -d $HOME/.exo ]; then
	exit 4
fi

if [ -z $(which exo) ]; then
	exit 5
fi
