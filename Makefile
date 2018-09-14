.editor-pref:
	./.setcfg.sh ./ "Enter a full path to your preferred editor" $@  

.PHONY: project
project: .editor-pref
	@./.mkproject.sh .
