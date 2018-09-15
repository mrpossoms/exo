#----------------------------------------
#   __   __        _      _    _        
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#                                       
include ../../vars.mk

MOD_SRC=$(shell ls *.c)

#----------------------------------------
#    ___      _        
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#             
.PHONY: all
all: $(addprefix obj/$(TARGET)/,$(MOD_SRC:.c=.o))
	@echo "$(COLOR_GREEN) Built module for $(TARGET) $(COLOR_OFF)"
         
obj/$(TARGET):
	mkdir -p $@

obj/$(TARGET)/%.o: %.c obj/$(TARGET)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf obj
