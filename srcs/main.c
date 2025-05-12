/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/12 22:37:26 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

int	g_signal_status = 0;

/* 初始化shell结构 */
static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_env(envp);
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->input = NULL;
	shell->exit_status = 0;
	return (shell);
}

/* 清理当前命令的资源 */
static void	clean_current_command(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmd_list)
	{
		free_cmds(shell->cmd_list);
		shell->cmd_list = NULL;
	}
}

/* 释放shell结构 */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	clean_current_command(shell);
	if (shell->env_list)
		free_env(shell->env_list);
	free(shell);
}

static int	execute_pipeline(t_shell *shell)
{
	t_cmd	*current_cmd;
	int		expansion_failed;

	// fprintf(stderr, "[Debug] execute_pipeline: Initial shell->exit_status = %d\n", shell->exit_status); // DEBUG
	expansion_failed = 0;
	current_cmd = shell->cmd_list;
	while (current_cmd)
	{
		if (expand_command(current_cmd, shell) != 0)
		{
			shell->exit_status = 1;
			expansion_failed = 1;
			// fprintf(stderr, "[Debug] execute_pipeline: expansion_failed for cmd: %s, setting shell->exit_status = 1\n", current_cmd && current_cmd->args && current_cmd->args[0] ? current_cmd->args[0] : "N/A"); // DEBUG
			break ;
		}
		current_cmd = current_cmd->next;
	}
	// fprintf(stderr, "[Debug] execute_pipeline: After expansion loop, expansion_failed = %d, shell->exit_status = %d\n", expansion_failed, shell->exit_status); // DEBUG
	if (!expansion_failed)
	{
		// fprintf(stderr, "[Debug] execute_pipeline: Calling executor. shell->exit_status before executor = %d\n", shell->exit_status); // DEBUG
		shell->exit_status = executor(shell->cmd_list, &(shell->env_list), shell);
		// fprintf(stderr, "[Debug] execute_pipeline: After executor, executor returned, new shell->exit_status = %d\n", shell->exit_status); // DEBUG
	}
	free_cmds(shell->cmd_list);
	shell->cmd_list = NULL;
	// fprintf(stderr, "[Debug] execute_pipeline: returning %d (shell->exit_status = %d)\n", shell->exit_status, shell->exit_status);
	return (shell->exit_status);
}

static int	process_input(t_shell *shell)
{
	if (!shell->input || shell->input[0] == '\0')
		return (0);
	shell->tokens = lexer(shell->input);
	// DEBUG: Print tokens before parser
	/*
	fprintf(stderr, "[Debug] process_input: Tokens before parser:\n");
	t_token *dbg_token = shell->tokens;
	while (dbg_token)
	{
		fprintf(stderr, "  Token: Type=%d, Value='%s'\n", dbg_token->type, dbg_token->value ? dbg_token->value : "(null)");
		dbg_token = dbg_token->next;
	}
	*/
	// End DEBUG
	if (!shell->tokens)
		return (0);
	shell->cmd_list = parser(shell->tokens);
	// fprintf(stderr, "[Debug] process_input: After parser, shell->cmd_list is %p\n", shell->cmd_list);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	if (!shell->cmd_list)
	{
		// fprintf(stderr, "[Debug] process_input: Parser returned NULL, returning 0\n");
		return (0);
	}
	return (execute_pipeline(shell));
}

/* 设置终端属性 */
static void	setup_readline(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	handle_sigint_prompt(void)
{
	if (g_signal_status == 130)
	{
		g_signal_status = 0;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

static char *read_input(void)
{
	char *prompt;

	prompt = readline("minishell> ");
	if (!prompt)
	{
		printf("exit\n");
		return (NULL);
	}
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*current_input;
	int		last_status = 0; // Initialize

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	setup_signals();
	setup_readline(); /* Call unconditionally again */
	while (1)
	{
		handle_sigint_prompt();
		current_input = read_input(); /* Uses readline */
		if (!current_input) /* EOF */
			break ;
		if (current_input[0] != '\0')
		{
			shell->input = ft_strdup(current_input);
			free(current_input);
			if (shell->input)
			{
				add_history(shell->input); /* Add history here again */
				process_input(shell);
				clean_current_command(shell);
			}
		}
		else
		{
			free(current_input);
		}
	}
	last_status = shell->exit_status;
	free_shell(shell);
	// fprintf(stderr, "[Debug] main: returning %d\n", last_status);
	return (last_status);
} 