#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
MOD_DIR=mod
MSG_DIR=msg

# global to all modules
INC=-I$(EXO_ROOT)/inc
FLAGS=-g -std=c++11 -fPIC

EXO_IMP=imp
HDRS=$(shell exo rls $(EXO_ROOT)/inc | grep "\.hpp")

COLOR_GREEN=\033[0;32m
COLOR_OFF=\033[0m

OS=$(shell uname)

EXO_MAGIC=$(shell exo magic $(HDRS))
FLAGS+= -DEXO_MAGIC=$(EXO_MAGIC)

ifndef CMP
CMP=g++
endif

TARGET=$(shell $(CMP) -dumpmachine)

exo.magic: $(HDRS)
	$(shell exo magic $^ > exo.magic)

