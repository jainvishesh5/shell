
# Custom C++ Linux Shell Implementation

A lightweight, POSIX-compliant custom shell written in C++ from scratch. This project implements standard shell features by interacting directly with Linux system calls. By putting the terminal into **raw mode**, it bypasses canonical input processing to handle custom tab completion, manual backspace manipulations, and seamless multi-stage command piping.


## 🚀 Features

* **Interactive Raw Terminal Mode:** Configures the terminal profile (`termios`) to catch keystrokes instantly by disabling `ICANON` and `ECHO` flags.
* **Smart Tab Completion:**
  * Pressing `Tab` once auto-completes unique builtins or system binaries found across the dynamic `PATH` directory vector.
  * Dynamically evaluates partial strings to complete arguments up to their longest common prefix.
  * Pressing `Tab` twice breaks the line to print all current matching commands out in an organized sub-grid.
* **Multi-Stage Pipelines (`|`):** Supports chained external program executions by managing sequential anonymous descriptor arrays with `pipe()` and `dup2()`.
* **Advanced Tokenizer Engine:** Handles single quotes (`'...'`), double quotes (`"..."`), and backslash (`\`) escape characters accurately.
* **Output & Error File Redirection:**
  * `>` or `1>`: Overwrite standard output file streams.
  * `>>` or `1>>`: Append data to standard output file streams.
  * `2>`: Overwrite standard error log streams.
  * `2>>`: Append data to standard error log streams.
* **Native Builtins:** Built-in support for `echo`, `cd` (with complete `~` path translation mapping), `pwd`, `type`, and `exit`.


## 🛠️ Architecture Workflow

The application relies on a continuous Read-Evaluate-Print Loop (**REPL**) that manually controls stream buffers:



[Raw Input Loop] ──> [Tokenizer Engine] ──> [Redirection Parser] ──> [Execution Engine]
(Reads chars          (Handles quotes/        (Pulls '>', '2>>'        (Forks processes
one-by-one)            escape arrays)          modifies argv)           or runs builtin)


1. **Read Stream (Raw Input):** Characters are pulled from `STDIN_FILENO` using a single-byte read buffer. Signals like Backspace (`127`) or Tab (`\t`) bypass default rendering to execute structural UI screen updates (`\b \b` or `\r$ ` rewrites).
2. **Tokenizer:** Splits raw context strings into independent execution blocks while isolating nested quotes or literal string flags.
3. **Redirection Isolation:** Scans instructions backward to dynamically catch structural routing parameters (like `2>>`), mapping target paths into internal configurations while safely truncating them out of the vector argument array.
4. **Execution Dispatcher:** * **Builtins:** Handled instantly inside the parent memory process, swapping standard IO parameters temporarily using duplicate file configurations when file logs are defined.
   * **Pipelines / External Binaries:** Clones tracking boundaries via `fork()`. Elements in the sequence establish automated communication via structured system descriptors prior to executing targets using `execvp()`.


## 💻 Prerequisites

This shell relies heavily on core POSIX Linux system APIs (`<unistd.h>`, `<sys/wait.h>`, `<fcntl.h>`, `<termios.h>`). It must be compiled and run inside a native Linux environment or via Windows Subsystem for Linux (WSL).

* **Compiler:** Any GCC or Clang toolchain supporting C++17 or later.
* **Build Tools:** `g++` command-line utility.


## 📦 Getting Started

### 1. Clone the Repository
bash
git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
cd your-repo-name


### 2. Compile the Source Code

Compile the consolidated `main.cpp` entry file:

bash
g++ -std=c++17 main.cpp -o myshell


### 3. Launch the Shell
bash
./myshell


## 🧪 Usage Examples

### Standard Native Routing

bash
$ pwd
/home/developer/projects
$ cd ~
$ pwd
/home/developer


### File Stream Redirection
bash
# Overwrite standard outputs smoothly
$ echo "Hello World" > output.txt

# Append custom standard error traces from invalid system queries
$ ls non_existent_folder 2>> fallback_errors.log


### Multi-Stage Processing Pipelines

bash
# Line up separate external filters into one continuous workspace string
$ cat /etc/passwd | grep root | type


### Interactive Tab Completion UI
bash
# Single match completing an engine trigger
$ec[TAB] -> transforms automatically into ->$ echo 

# Multi-match previewing (Press Tab Twice)
$ short[TAB][TAB]
shortcut shortpath shortlink 
$ short


## 📂 Project Structure Explained

* `tokenize()`: Main parser evaluating input bounds to create command arguments while strictly validating unmatched quote bounds.
* `parse_redirection()`: Modifies parameter arrays by extracting explicit targets and identifying append flags.
* `run_pipeline()`: Manages anonymous file piping arrays to thread process outputs sequentially into adjacent standard input streams.
* `main()`: Stores active terminal setups, transitions the interface into unbuffered raw state routines, and handles resetting your terminal interface variables on loop termination.

---
