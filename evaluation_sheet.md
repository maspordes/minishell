## Minishell Evaluation Sheet (Markdown Version)

### Introduction

- Remain polite, courteous, respectful, and constructive throughout the evaluation process. The well-being of the community depends on it.
- Identify with the student or group whose work is evaluated the possible dysfunctions in their project. Take the time to discuss and debate the problems that may have been identified.
- Consider that there might be differences in how your peers understood the instructions and the scope of functionalities. Grade honestly and keep an open mind.
- Pedagogy is useful only if the peer-evaluation is done seriously.

### Guidelines

- Only grade the work that was turned in the Git repository of the evaluated student or group.
- Double-check that the Git repository belongs to the student(s). Ensure that the project is the one expected. Also, check that 'git clone' is used in an empty folder.
- Check carefully that no malicious aliases were used to fool you and make you evaluate something that is not the content of the official repository.
- If applicable, review together any scripts used to facilitate grading (scripts for testing or automation).
- If you have not completed the assignment you are evaluating, read the entire subject before starting the evaluation process.
- Use available flags to report:
  - Empty repository
  - Non-functioning program
  - Norm error
  - Cheating
  - Crashes
- In these cases, the evaluation ends and the final grade is 0 (or -42 in case of cheating). Except for cheating, students are encouraged to review the work together to learn from mistakes.
- During the defense, **no segfaults or other unexpected terminations** are allowed; otherwise, the final grade is 0.
- Only edit the configuration file if needed. If you need to edit any file, explain the reasons to the student and ensure both agree.
- **Verify the absence of memory leaks.** Any memory allocated on the heap must be properly freed before the end of execution.
- You may use tools like `leaks`, `valgrind`, or `e_fence`. In case of memory leaks, tick the appropriate flag.

---

## Mandatory Part

### Compile

- Use `make -n` to check if compilation uses `-Wall -Wextra -Werror`. If not, select the "invalid compilation" flag.
- Minishell must compile without errors. If not, select the flag.
- The Makefile must not re-link. If not, select the flag.

### Simple Command & Global Variables

- Execute a simple command with an absolute path (e.g., `/bin/ls`) or any other command without options.
- Ask: How many global variables are used? Why? The student should give a concrete example of why it is mandatory or logical.
- Test an empty command.
- Test only spaces or tabs.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### Arguments

- Execute a simple command with an absolute path (e.g., `/bin/ls`) or any other command with arguments but without quotes.
- Repeat with different commands and arguments.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### echo

- Execute the `echo` command with or without arguments, or the `-n` option.
- Repeat with different arguments.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### exit

- Execute `exit` with or without arguments.
- Repeat with different arguments.
- Don't forget to relaunch the minishell.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### Return Value of a Process

- Execute a simple command (e.g., `/bin/ls`), then execute `echo $?`. Check the printed value; compare with bash.
- Repeat with different commands and arguments. Try wrong commands like `/bin/ls filethatdoesntexist`.
- Try expressions like `expr $? + $?`.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### Signals

- `ctrl-C` in an empty prompt: should display a new line with a new prompt.
- `ctrl-\` in an empty prompt: should do nothing.
- `ctrl-D` in an empty prompt: should quit minishell (relaunch after).
- `ctrl-C` in a prompt after typing: should display a new line with a new prompt. The buffer should be clean; press "Enter" to ensure nothing from the previous line is executed.
- `ctrl-D` in a prompt after typing: should do nothing.
- `ctrl-\` in a prompt after typing: should do nothing.
- Try `ctrl-C`, `ctrl-\`, and `ctrl-D` after running a blocking command like `cat` or `grep`.
- Repeat with different commands.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### Double Quotes

- Execute a command with arguments using double quotes (include whitespaces).
- Try commands like: `echo "cat lol.c | cat > lol.c"`
- Try anything except `$`.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.

### Single Quotes

- Execute commands with single quotes as arguments.
- Try empty arguments.
- Try environment variables, whitespaces, pipes, redirection in single quotes. Example: `echo '$USER'` must print `$USER`.
- Nothing should be interpreted inside single quotes.

### env

- Check if `env` shows the current environment variables.

### export

- Export environment variables, create new ones, and replace old ones.
- Check the result with `env`.

### unset

- Remove environment variables with `unset`.
- Check the result with `env`.

### cd

- Use `cd` to move the working directory; check with `/bin/ls`.
- Repeat with working and non-working directories.
- Try `.` and `..` as arguments.

### pwd

- Use the `pwd` command.
- Repeat in different directories.

### Relative Path

- Execute commands using a relative path.
- Repeat in different directories with complex relative paths (many `..`).

### Environment Path

- Execute commands without any path (e.g., `ls`, `wc`, `awk`).
- Unset `$PATH` and ensure commands do not work.
- Set `$PATH` to multiple directories (e.g., `directory1:directory2`) and ensure directories are checked in order.

### Redirection

- Execute commands with redirections ``.
- Repeat with different commands and arguments; sometimes change `>` with `>>`.
- Check if multiple tries of the same redirections fail.
- Test `<<` redirection (does not need to update history).

### Pipes

- Execute commands with pipes (e.g., `cat file | grep bla | more`).
- Repeat with different commands and arguments.
- Try wrong commands like `ls filethatdoesntexist | grep bla | more`.
- Try mixing pipes and redirections.

### Go Crazy and History

- Type a command line, then use `ctrl-C` and press "Enter". The buffer should be clean.
- Can you navigate through history using Up and Down? Can you retry some command?
- Execute commands that should not work (e.g., `dsbksdgbksdghsd`). Ensure minishell doesn't crash and prints an error.
- `cat | cat | ls` should behave normally.
- Try executing a long command with many arguments.
- Have fun with your minishell!

### Environment Variables

- Execute `echo` with environment variables (`$variable`) as arguments.
- Check that `$` is interpreted as an environment variable.
- Double quotes should interpolate `$`.
- Check that `USER` exists. If not, set it.
- `echo "$USER"` should print the value of the `USER` variable.

---

## Bonus

- Evaluate the bonus part only if the mandatory part is entirely and perfectly done, and error management is flawless.
- If all mandatory points were not passed, ignore bonus points.

### Bonus Features

- Use `&&`, `||`, and parenthesis with commands; ensure minishell behaves like bash.
- Use wildcards in arguments in the current working directory.
- Surprise! (e.g., set the `USER` environment variable, test `echo "'$USER'"` and `echo '"$USER"'`).

---

## Ratings

- **Ok**
- **Outstanding project**
- **Empty work**
- **Incomplete work**
- **Invalid compilation**
- **Norme**
- **Cheat**
- **Crash**
- **Incomplete group**
- **Concerning situation**
- **Leaks**
- **Forbidden function**

---

## Conclusion

- Leave a comment on this evaluation.

---

**End of Evaluation Sheet**
[Content adapted from the official evaluation sheet][1]
