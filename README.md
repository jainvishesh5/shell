# C++ UNIX Shell

A UNIX-style shell built from scratch in C++ as part of my systems programming and backend engineering journey.

This project focuses on understanding how real shells work internally by implementing core shell functionality manually using low-level Linux concepts and system calls.

---

# Features

## Builtins
- `echo`
- `type`
- `exit`
- `pwd`
- `cd`

## Command Execution
- External command execution
- PATH environment variable resolution
- Executable lookup using Linux system APIs

## Shell Functionality
- Interactive REPL loop
- Command parsing
- Argument tokenization
- Error handling
- Command-not-found handling

## Linux/System Programming Concepts
- Processes
- `fork()`
- `execvp()`
- `waitpid()`
- Pipes
- File descriptors
- Redirection
- Environment variables
- Inter-process communication

## Advanced Features
- Piping (`|`)
- Input/output redirection (`>`, `<`)
- Background processes (`&`)
- Signal handling
- Job control

---

# Tech Stack

- C++
- Linux / WSL
- POSIX system calls

---

# Project Structure

```text
src/
└── main.cpp
```

As the project grows, the codebase will be modularized into:
- parser
- executor
- builtins
- process management
- piping/redirection handlers

---

# Build & Run

## Compile

```bash
g++ src/main.cpp -o shell
```

## Run

```bash
./shell
```

---

# Example Usage

```bash
$ echo hello
hello

$ type ls
ls is /usr/bin/ls

$ pwd
/home/user/projects

$ ls | grep cpp

$ cat file.txt > output.txt
```

---

# Concepts Explored

This project explores:
- shell architecture
- Linux internals
- process management
- executable resolution
- command parsing
- system programming
- concurrency fundamentals
- UNIX philosophy

---

# Motivation

Instead of only using developer tools, I want to deeply understand how they work internally by rebuilding them from scratch.

This project is part of a larger systems programming journey that includes building:
- shells
- web servers
- Redis clones
- Git clones
- databases
- distributed systems

---

# Learning Goals

- Become proficient in systems programming
- Understand Linux process management deeply
- Learn low-level networking and IPC
- Improve C++ engineering skills
- Build strong backend/infrastructure fundamentals

---

# Future Improvements

- Better parser architecture
- Autocomplete
- Command history persistence
- Configuration files
- Shell scripting support
- Async job management
- Improved terminal UX
- Plugin system

---

# References

- Operating Systems: Three Easy Pieces
- Beej’s Guide to Network Programming
- Linux man pages
- CodeCrafters
- The Linux Programming Interface