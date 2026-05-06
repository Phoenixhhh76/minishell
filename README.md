# 🐚 Minishell

> A minimal UNIX shell written in C — 42 School project

---

## 📖 Overview

**Minishell** is a simplified shell implementation inspired by `bash`. The goal of this project is to understand how a shell works under the hood: parsing user input, building an Abstract Syntax Tree (AST), and executing commands through system calls.

---

## ✨ Features

- Interactive prompt with command history
- Parsing via an **Abstract Syntax Tree (AST)**
- Command execution with `execve`
- Pipes (`|`) between commands
- Input/output redirections (`<`, `>`, `>>`, `<<`)
- Environment variable expansion (`$VAR`, `$?`)
- Quote handling (`'single'` and `"double"`)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

### Built-in commands

| Command | Description |
|---------|-------------|
| `echo` | Display a line of text (`-n` supported) |
| `cd` | Change the current directory |
| `pwd` | Print the current working directory |
| `export` | Set environment variables |
| `unset` | Unset environment variables |
| `env` | Print the current environment |
| `exit` | Exit the shell |

---

## 🏗️ Architecture — AST-based parsing

Rather than using a simple linear parser, this shell uses an **Abstract Syntax Tree (AST)** to represent the structure of a command line.

```
echo hello | cat -e > out.txt
          |
        PIPE
       /    \
    CMD      REDIR (>)
   /   \        \
 echo  hello    CMD
               /   \
             cat   -e
```

### Parsing pipeline

```
Input string
     │
     ▼
  Lexer (tokenization)
     │
     ▼
  Parser (builds AST)
     │
     ▼
  Executor (tree traversal)
     │
     ▼
  System calls (fork, execve, pipe, dup2...)
```

### AST node types

- `CMD` — a simple command with its arguments
- `PIPE` — connects stdout of left child to stdin of right child
- `REDIR_IN` / `REDIR_OUT` / `REDIR_APPEND` / `HEREDOC` — I/O redirections

---

## 🚀 Getting started

### Prerequisites

- GCC
- GNU Make
- `readline` library

On Linux (Ubuntu/Debian):
```bash
sudo apt-get install libreadline-dev
```

On macOS:
```bash
brew install readline
```

### Build

```bash
git clone https://github.com/Phoenixhhh76/minishell.git minishell
cd minishell
make
```

### Run

```bash
./minishell
```
---

## 🛠️ Implementation details

### Lexer

The lexer reads the raw input string and produces a list of tokens, handling:
- Word splitting
- Quote state machine (`'` and `"`)
- Operator detection (`|`, `<`, `>`, `>>`, `<<`)

### Parser

The parser consumes the token list and recursively builds an AST following a simple grammar:

```
pipeline    := command (PIPE command)*
command     := token+ redirection*
redirection := ('<' | '>' | '>>' | '<<') token
```

### Executor

The executor performs a recursive tree traversal:
- `PIPE` nodes → `fork()` + `pipe()` + recursive calls on each branch
- `REDIR` nodes → `open()` + `dup2()` before executing the child
- `CMD` nodes → `fork()` + `execve()` (or built-in dispatch)

---

## 📚 Resources

- [Bash manual](https://www.gnu.org/software/bash/manual/)
---

## 📜 License

This project is part of the 42 School curriculum and is for educational purposes only.
