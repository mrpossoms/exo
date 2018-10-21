#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

MOD_SRC=$(shell ls *.cpp)
OS_TARGET=$(cat os_target)
TARGET_OBJS=$(shell ls imp/obj/$(TARGET))

#----------------------------------------
#    ___      _
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#
.PHONY: all
all: $(addprefix obj/$(TARGET)/,$(MOD_SRC:.cpp=.o)) exo
	@echo "$(COLOR_GREEN) Built module for $(TARGET) $(COLOR_OFF)"

obj/$(TARGET):
	mkdir -p $@

obj/$(TARGET)/%.o: %.cpp obj/$(TARGET)
	$(CMP) $(FLAGS) $(INC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf obj
