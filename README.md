# exo
Build: [![CircleCI](https://circleci.com/gh/mrpossoms/exo/tree/master.svg?style=svg)](https://circleci.com/gh/mrpossoms/exo/tree/master)

exo is a loose framework of bash scripts, and C++ source that help to create modular robotics software that can run anywhere. exo aims to facilitate solving of the following problems.

* graceful, and safe failure
* cross-compilation
* simulation environment integration
* testing
* intra module communication
* platform specific common actions

## Concepts
As an author of an exo project, there are two primary components that you will author. Modules, and messages. Modules can be thought of as self contained programs or code that perform only a few tightly related actions. Messages on the other hand are the means by which modules communicate with each-other. 

## Setup
From your shell, simply run
```shell
$ scripts/install.sh
```
You will then be prompted to provide a path for exo to be symlinked into, and a path your your specific shell's rc file (.bashrc, .zshrc, etc...). After that source your rc file or restart your shell.

## Usage
exo is partially comprised of a set of bash scripts that help automate the process of creating modules and messages. Invoking `exo` from the terminal will show you a number of sub commands that can be used. For example `exo project` will create a new exo project from the template files within the exo repository. `exo msg [message name]` will create a new message from within a project's `msg` directory. And so on. If you have questions about a particular subcommand or its' usage, you can run `exo [subcommand name] help` or `exo [subcommand name] usage` respectively.
