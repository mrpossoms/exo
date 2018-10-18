#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

FLAGS+=-std=c++11
INC+=-Iinc/
SRCS=$(shell ls src)
OBJS=$(foreach src,$(SRCS:.cpp=.o),obj/$(TARGET)/$(src))

#----------------------------------------
#    ___      _
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#
all: $(OBJS)
	@echo "$(COLOR_GREEN) Built exo $(COLOR_OFF)"

obj/$(TARGET):
	mkdir -p obj/$(TARGET)

obj/%.o: obj/$(TARGET)/%.o
	@echo "$(COLOR_GREEN) Built $@ $(COLOR_OFF)"

obj/$(TARGET)/%.o: src/%.cpp obj/$(TARGET)
	$(CMP) $(INC) $(FLAGS) -c $< -o $@
	@echo "$(COLOR_GREEN) Built $@ $(COLOR_OFF)"

tests/bin:
	mkdir tests/bin

tests/bin/%: tests/%.cpp tests/bin
	$(CMP) $(INC) $(FLAGS) -Itests/ src/*.cpp $< -o $@

test:
	echo ""

clean:
	rm -rf obj
	rm -rf tests/bin

.PHONY: all test obj/%.o
