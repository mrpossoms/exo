#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

FLAGS+=-std=c++11 -g
INC+=-Iinc/
#PLATFORMS=$(shell ls src/platforms)
SRCS=$(shell ls src | grep .cpp)
OBJS=$(foreach src,$(SRCS:.cpp=.o),obj/$(TARGET)/$(src))
TEST_SRCS=$(shell ls tests | grep .cpp)
TEST_EXES=$(foreach src,$(TEST_SRCS:.cpp=),tests/bin/$(src))

#----------------------------------------
#    ___      _
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#
all: $(OBJS)
	@echo "$(COLOR_GREEN) Built exo $(COLOR_OFF)"

lib:
	mkdir -p lib

lib/libexo.a: lib $(OBJS)
	ar rcs $@ $(OBJS)

static: lib/libexo.a
	@echo "$(COLOR_GREEN) Built $@ $(COLOR_OFF)"

obj/$(TARGET):
	mkdir -p obj/$(TARGET)

#obj/%.o: obj/$(TARGET)/%.o
#	@echo "$(COLOR_GREEN) Built $@ $(COLOR_OFF)"

obj/$(TARGET)/%.o: src/%.cpp obj/$(TARGET)
	$(CMP) $(INC) $(FLAGS) -c $< -o $@
	@echo "$(COLOR_GREEN) Built $@ $(COLOR_OFF)"

tests/bin:
	mkdir tests/bin

tests/bin/%: tests/%.cpp tests/bin
	$(CMP) $(INC) $(FLAGS) -Itests/ $< src/*.cpp -o $@

tests: $(TEST_EXES)
	@echo "$(COLOR_GREEN) Built all tests $(COLOR_OFF)"

test: tests
	./test_runner.py

clean:
	rm -rf obj lib
	rm -rf tests/bin

install: static
	sudo ln -s $(shell pwd)/inc /usr/local/include/exo
	sudo ln -s $(shell pwd)/lib/libexo.a /usr/local/lib/libexo.a

.PHONY: all install test obj/%.o static
