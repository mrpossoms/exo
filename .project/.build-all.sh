#!/bin/sh

for mod in $(ls mod); do
    make -C mod/$mod
done
