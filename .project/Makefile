#----------------------------------------
#   __   __        _      _    _
#   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
#    \ V / _` | '_| / _` | '_ \ / -_|_-<
#     \_/\__,_|_| |_\__,_|_.__/_\___/__/
#
include $(EXO_ROOT)/vars.mk

#----------------------------------------
#    ___      _
#   | _ \_  _| |___ ___
#   |   / || | / -_|_-<
#   |_|_\\_,_|_\___/__/
#
.PHONY: setup
setup:
	mkdir $(MSG_DIR)

.PHONY: mod
mod:
	@./.mkmod.sh $(MOD_DIR)

.PHONY: msg
msg:
	@./.mkmsg.sh $(MSG_DIR)

bin/$(TARGET): bin
	mkdir -p bin/$(TARGET)

$(MOD_DIR)/%/obj:
	mkdir $@

$(MOD_DIR)//obj/%.o: $(MOD_DIR)/%.c
	$(CMP) $^ -o $@
