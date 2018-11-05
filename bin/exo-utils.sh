function prompt {
    prompt_msg=$1
    >&2 printf "%s" "$prompt_msg"
    read -r value
    echo $value
}

function set_cfg_val {
    path=$1
    name=$2
    value=$3

    echo $value > $path/$name
}


function get_cfg_val {
    path=$1
    name=$2

    cat $path/$name
}

function invoke {
    target=$1
    arg=$2

    if [ $arg == $target ]; then
        $target
    fi
}

function create_default_configs {
    mkdir $CFG_ROOT

    set_cfg_val $CFG_ROOT exo_base $EXO_ROOT
    set_cfg_val $CFG_ROOT template_path $EXO_ROOT/.project
}
