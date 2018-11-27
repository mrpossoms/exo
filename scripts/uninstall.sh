#!/bin/bash

# remove the symlink to the exo repo
rm $EXO_ROOT

# find the dot file from settings, replace with backed up repo
dot_file_path=$(cat $HOME/.exo/user_dot_file)
mv $dot_file_path $dot_file_path.exo
mv $dot_file_path.exo.bk $dot_file_path

# remove symlinks to links for include and host library
make uninstall

rm -rf $HOME/.exo
