#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

MOD_NAME=$(shell basename $(shell pwd))
MOD_SRC=$(shell ls *.cpp)
OS_TARGET=$(exo get os_target)
TARGET_OBJS=$(shell ls $(EXO_ROOT)/imp/obj/$(TARGET)/$(OS_TARGET)_*)
MOD_OBJS=$(addprefix obj/$(TARGET)/,$(MOD_SRC:.cpp=.o))

#----------------------------------------
#    ___      _
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#
.PHONY: all
all: $(MOD_OBJS) exo

	$(if $(OS_TARGET)=='unix',make unix-mod)
	@echo "$(COLOR_GREEN) Built module for $(TARGET) $(COLOR_OFF)"

.PHONY: unix-mod
unix-mod: bin/$(TARGET) $(MOD_OBJS) exo
	$(CMP) $(FLAGS) $(EXO_ROOT)/imp/obj/$(TARGET)/*.o $(MOD_OBJS) -o bin/$(TARGET)/$(MOD_NAME)

bin/$(TARGET):
	mkdir -p $@

obj/$(TARGET):
	mkdir -p $@

obj/$(TARGET)/%.o: %.cpp obj/$(TARGET)
	$(CMP) $(FLAGS) $(INC) -c $< -o $@

.PHONY: target_objs
target_objs:
	@echo $(TARGET_OBJS)

.PHONY: clean
clean:
	rm -rf obj
