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


function create_default_configs {
    mkdir $CFG_ROOT

    set_cfg_val $CFG_ROOT exo_base /etc/exo
    set_cfg_val $CFG_ROOT template_path /etc/exo/.project
}
