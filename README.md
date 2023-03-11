# MINISHELL
> MINISHELL is aiming to create a command-line interface based on the famous bash shell, that allows users to interact with an operating system.

<br>

## Table of Contents
* [General Info](#general-information)
* [Features](#features)
* [Usage](#usage)


## General Information
- Minishell displays a command prompt, which indicates that the user can enter a command. Once the command is entered, minishell interprets it, launches a new process, and executes the appropriate program within the operating system or our own built-ins such as cd, echo, pwd.
-  In addition to the execution of commands, minishell can perform other functions, such as setting environment variables, executing scripts, and managing processes.


## Features
- Handles input/output/append redirection and here doc.
- Handles local and environment variables. 
- Handles crtl-c, crtl-d, crtl-\
- Handles single and double quotes.
- Have a working history.
- Handles the following builtins: cd, echo, pwd, export, unset, env, exit.
- Handles $?

## Usage
`make && ./minishell`

## Contact
Created by [Teriuihi](https://github.com/Teriuihi) && [bmajor]https://github.com/alexdrumi