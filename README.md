# Minishell Project

## Overview
Minishell is a minimalistic shell program implemented in C, designed to mimic some of the basic functionalities of traditional Unix shells taking bash as reference. This project serves as an educational tool to understand the workings of a shell, including process creation and management, environment variable handling, and command parsing and execution.
We got to explore some of the famous parsing algorithms (RDP : recursive decent parser), as well as implementing (AST : the abstract syntax tree)

## Features
- Command execution: Minishell can execute standard Unix commands from the PATH or custom built-in commands.
- Environment variables: Support for setting, unsetting, and displaying environment variables.
- Signal handling: Handles signals like `SIGINT` (Ctrl-C) and `SIGQUIT` (Ctrl-\) gracefully.
- Command history: Basic command history functionality to navigate through previously entered commands.
- Redirection and piping: Support for input and output redirection (`<`, `>`, `>>`) and command piping (`|`).
- AND and OR opearators.
- Wildcard (*) .....

## Built-in Commands
Minishell includes several built-in commands:
- `cd`: Change the current directory.
- `exit`: Exit the shell.
- `env`: List all environment variables.
- `setenv`: Set an environment variable.
- `unsetenv`: Unset an environment variable.
- `echo` : displays a text/string to the standard output with -n option 

## Getting Started

### Prerequisites
- GCC compiler
- Make (optional for build automation)
- export DEBUG=1 for the debug mode

### Installation
1. Clone the repository:
  - git clone https://github.com/Abdox77/minishell.git

2. Navigate to the project directory:
  - cd minishell && ls *

3. Compile and run the project:
 - make && ./minishell
