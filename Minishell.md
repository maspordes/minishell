# As Beautiful as a Shell

### Version: 8.3

---

## Contents
1. **Introduction**  
2. **Common Instructions**  
3. **Mandatory Part**  
4. **Bonus Part**  
5. **Submission and Peer-Evaluation**

---

## Chapter I: Introduction

Shells have existed since the very beginning of IT. Back then, all developers agreed that communicating with a computer via aligned 1/0 switches was extremely frustrating. It was only logical that they came up with the idea of creating software to communicate with a computer using interactive command lines in a language somewhat close to human language.

With Minishell, you’ll travel back in time and experience the challenges developers faced before Windows existed.

---

## Chapter II: Common Instructions

- Your project must be written in **C**.
- Your project must be written in accordance with the Norm. If you have bonus files/functions, they are included in the norm check, and you will receive a 0 if there is a norm error.
- Your functions should not quit unexpectedly (segmentation fault, bus error, double free, etc.) except for undefined behavior. If this occurs, your project will be considered non-functional and will receive a 0 during the evaluation.
- All heap-allocated memory must be properly freed when necessary. Memory leaks will not be tolerated.
- If the subject requires it, you must submit a **Makefile** that compiles your source files to the required output with the flags `-Wall`, `-Wextra`, and `-Werror`, using `cc`. Additionally, your Makefile must not perform unnecessary relinking.
- Your Makefile must contain at least the rules: `$(NAME)`, `all`, `clean`, `fclean`, and `re`.
- To submit bonuses for your project, you must include a bonus rule in your Makefile, which will add all the various headers, libraries, or functions that are not allowed in the main part of the project. Bonuses must be placed in `_bonus.{c/h}` files unless the subject specifies otherwise. The evaluation of mandatory and bonus parts is conducted separately.
- If your project allows you to use your **libft**, you must copy its sources and its associated Makefile into a libft folder. Your project’s Makefile must compile the library by using its Makefile, then compile the project.
- We encourage you to create test programs for your project, even though this work does not need to be submitted and will not be graded. It will give you an opportunity to easily test your work and your peers’ work. You will find these tests especially useful during your defense.
- Submit your work to the assigned Git repository. Only the work in the Git repository will be graded. If Deepthought is assigned to grade your work, it will occur after your peer evaluations. If an error happens in any section of your work during Deepthought’s grading, the evaluation will stop.

---

## Chapter III: Mandatory Part

### Program Name
`minishell`

### Turn-in Files
`Makefile`, `*.h`, `*.c`

### Makefile Rules
`NAME`, `all`, `clean`, `fclean`, `re`

### Arguments
None

### External Functions Allowed
`readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`,  
`printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`,  
`fork`, `wait`, `waitpid`, `wait3`, `wait4`,  
`signal`, `sigaction`, `sigemptyset`, `sigaddset`,  
`kill`, `exit`,  
`getcwd`, `chdir`,  
`stat`, `lstat`, `fstat`,  
`unlink`,  
`execve`,  
`dup`, `dup2`,  
`pipe`,  
`opendir`, `readdir`, `closedir`,  
`strerror`,  
`perror`,  
`isatty`,  
`ttyname`,  
`ttyslot`,
`ioctl`,
`getenv`,
`tcsetattr`,
`tcgetattr`,
`tgetent`,
`tgetflag`,
`tgetnum`,
`tgetstr`,
`tgoto`,
`tputs`.

### Libft Authorized
Yes

### Description
Write a shell that:

1. Displays a prompt when waiting for a new command.
2. Has a working history.
3. Searches and launches executables based on:
   - The PATH variable.
   - A relative or absolute path.
4. Uses at most one global variable to indicate received signals.
5. Handles:
   - Single quotes (`'`) to prevent interpretation of meta-characters.
   - Double quotes (`"`) to prevent interpretation of meta-characters except `$`.
6. Implements redirections:
   - `<`: Redirect input.
   - `>`: Redirect output.
   - `<<`: Read input until a specified delimiter is seen (does not update history).
   - `>>`: Redirect output in append mode.
7. Implements pipes (`|`) where output of one command connects to input of another.
8. Handles environment variables (`$VAR`) and `$?`.
9. Handles:
   - Ctrl-C: Displays a new prompt on a new line.
   - Ctrl-D: Exits the shell.
   - Ctrl-\: Does nothing.
10. Implements built-in commands:
    - **echo** (with option -n)
    - **cd** (only relative or absolute paths)
    - **pwd**
    - **export**
    - **unset**
    - **env**
    - **exit**

Note: The readline() function may cause memory leaks but does not need fixing.

---

## Chapter IV: Bonus Part

Your program must implement:

1. Logical operators:
   - **&&** (AND)
   - **||** (OR) with parentheses for priority.
2. Wildcards (`*`) for current working directory.

**Note:** The bonus part is evaluated only if the mandatory part is perfectly implemented.

---

## Chapter V: Submission and Peer-Evaluation

Submit your assignment in your Git repository as usual. Only the work inside your repository will be evaluated during defense.

Double-check file names to ensure correctness.

---
