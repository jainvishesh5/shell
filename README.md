# Unix Shell in C++

A Unix-like command line shell built from scratch in C++ that supports command execution, built-in commands, I/O redirection, pipelines, quoting, escaping, and interactive tab completion.

This project demonstrates core operating system and systems programming concepts including process creation, program execution, inter-process communication, file descriptor management, and terminal control.



## Features

### Command Execution

* Execute external programs using `fork()` and `execvp()`
* Search executables through the system `PATH`
* Support arbitrary command arguments

Example:

bash
ls -la
grep main shell.cpp



### Built-in Commands

Implemented shell builtins:

| Command | Description                       |
| ------- | --------------------------------- |
| `cd`    | Change current directory          |
| `pwd`   | Print working directory           |
| `echo`  | Print arguments                   |
| `type`  | Identify builtins and executables |
| `exit`  | Exit the shell                    |

Examples:

bash
cd ~/projects
pwd
type ls
echo Hello World



### Quoting and Escaping

Supports:

* Single quotes `'...'`
* Double quotes `"..."`

Examples:

bash
echo "hello world"
echo 'multiple words'
echo file\ name.txt


The tokenizer correctly preserves quoted strings and escaped characters.



### Output Redirection

Supported redirections:

bash
command > file
command >> file
command 1> file
command 1>> file
command 2> file
command 2>> file


Examples:

bash
ls > files.txt
echo hello >> output.txt
grep main shell.cpp 2> errors.txt




### Pipelines

Supports multi-stage pipelines using `|`.

Examples:

bash
ls | grep cpp
cat file.txt | grep error | sort


Pipeline implementation uses:

* `pipe()`
* `fork()`
* `dup2()`
* `waitpid()`

allowing data to flow between processes exactly like a Unix shell.



### Interactive Tab Completion

Custom completion system implemented without external libraries.

Features:

* Built-in command completion
* Executable discovery from `PATH`
* Longest common prefix expansion
* Double-TAB to display all matches
* Bell notification when no completion exists

Examples:

bash
ec<TAB>


becomes

bash
echo


and

bash
p<TAB>


expands to the longest common prefix of matching commands.



### Terminal Handling

Uses raw terminal mode via `termios` to provide interactive input handling.

Implemented:

* Character-by-character input
* Backspace support
* TAB completion
* Custom command line rendering



## Architecture


User Input
     │
     ▼
 Tokenizer
     │
     ▼
 Command Parser
     │
     ├── Builtin Execution
     │
     ├── External Execution
     │       │
     │       ├── fork()
     │       └── execvp()
     │
     ├── Redirection Handler
     │
     └── Pipeline Executor




## System Calls Used

This project makes extensive use of Unix system calls:

| System Call | Purpose                     |
| ----------- | --------------------------- |
| `fork()`    | Create child processes      |
| `execvp()`  | Execute programs            |
| `waitpid()` | Process synchronization     |
| `pipe()`    | Inter-process communication |
| `dup2()`    | File descriptor redirection |
| `open()`    | File handling               |
| `access()`  | Executable lookup           |
| `getcwd()`  | Current directory retrieval |
| `chdir()`   | Directory changes           |
| `termios`   | Raw terminal control        |



## Build

### Requirements

* Linux
* GCC/G++
* C++17 or newer

### Compile

bash
g++ -std=c++17 shell.cpp -o shell


### Run

bash
./shell




## Example Session

bash
$ pwd
/home/user

$ echo Hello World
Hello World

$ type ls
ls is /usr/bin/ls

$ ls | grep cpp
shell.cpp

$ echo test > output.txt

$ cat output.txt
test




## Concepts Demonstrated

* Systems Programming
* Operating Systems
* Process Management
* Inter-Process Communication
* File Descriptor Manipulation
* Terminal Programming
* Command Parsing
* Unix Process Model
* Shell Design



## Future Improvements

* Command history
* Arrow-key navigation
* Environment variable expansion
* Job control (`jobs`, `fg`, `bg`)
* Signal handling (`SIGINT`, `SIGTSTP`)
* Command substitution
* Wildcard expansion (`*`)
* Argument auto-completion
* Background execution (`&`)



## Author

Vishesh Jain

Built as a systems programming project to explore how Unix shells manage processes, pipes, redirection, and terminal interaction.
