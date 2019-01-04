#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

FLAGS+=
INC+=-Iinc/
SRCS=$(shell ls src | grep .cpp)
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

obj/$(TARGET):
	mkdir -p obj/$(TARGET)

tests/bin:
	mkdir tests/bin

tests/bin/%: tests/%.cpp tests/bin
	$(CMP) $(INC) $(FLAGS) -Itests/ $< -o $@

tests: $(TEST_EXES)
	@echo "$(COLOR_GREEN) Built all tests $(COLOR_OFF)"

test: tests
	./test_runner.py

test-%: tests/bin/%
	./$<

clean:
	rm -rf tests/bin

install: 
	ln -s $(shell pwd)/inc /usr/local/include/exo

.PHONY: all install test 
