function on_failure {
    if [ $? != 0 ]; then
        echo "Error: $1"
	exit 1	
    fi
}

function prompt {
    prompt_msg="$1"
    >&2 printf "%s" "$prompt_msg"
    read -r value
    echo $value
}

function set_cfg_val {
    path="$1"
    name="$2"
    value="$3"

    echo $value > $path/$name
}

function get_cfg_val {
    path="$1"
    name="$2"

    cat $path/$name
}

function invoke {
    target="$1"
    arg="$2"

    if [ -z $arg ] || [ -z $target ]; then
        exit 0
    fi
    
    if [ $arg = $target ]; then
        exit 1
    fi
}

function create_default_configs {
    dot_file="$1"

    if [ -z $dot_file ]; then
        echo "Error: no dotfile path provided"
        exit 1
    fi

    CFG_ROOT=$HOME/.exo
    mkdir $CFG_ROOT

    set_cfg_val $CFG_ROOT exo_base $EXO_ROOT
    set_cfg_val $CFG_ROOT user_dot_file $dot_file
}
