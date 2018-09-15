#----------------------------------------
#   __   __        _      _    _        
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#                                       
MOD_DIR=mod
MSG_DIR=msg

# global to all modules
INC=
FLAGS=

COLOR_GREEN=\033[0;32m
COLOR_OFF=\033[0m

ifndef CC
CC=gcc
endif

TARGET=$(shell $(CC) -dumpmachine)


