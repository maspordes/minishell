/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/06 17:14:11 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 创建新的命令结构 */
static t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

/* 创建新的重定向结构 */
static t_redirect	*new_redirect(int type, char *file)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->file = ft_strdup(file);
	redirect->next = NULL;
	return (redirect);
}

/* 添加重定向到命令 */
static void	add_redirect(t_cmd *cmd, t_redirect *redirect)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redirect;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = redirect;
}

/* 处理重定向标记 */
static int	handle_redirect(t_token **token, t_cmd *cmd)
{
	int			type;
	t_token		*file_token;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != T_WORD)
		return (0);
	file_token = *token;
	add_redirect(cmd, new_redirect(type, file_token->value));
	*token = (*token)->next;
	return (1);
}

/* 处理管道标记 */
static t_cmd	*handle_pipe(t_token **token, t_cmd *cmd)
{
	t_cmd	*new;

	*token = (*token)->next;
	new = new_cmd();
	if (!new)
		return (NULL);
	cmd->next = new;
	return (new);
}

/* 添加参数到命令 */
static int	add_arg(t_cmd *cmd, char *arg)
{
	int		i;
	int		size;
	char	**new_args;

	size = 0;
	if (cmd->args)
		while (cmd->args[size])
			size++;
	new_args = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (1);
}

/* 解析命令和参数 */
static t_cmd	*parse_cmd(t_token **token)
{
	t_cmd	*cmd;
	t_cmd	*current;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	current = cmd;
	while (*token)
	{
		if ((*token)->type == T_WORD)
		{
			if (!add_arg(current, (*token)->value))
				return (NULL);
			*token = (*token)->next;
		}
		else if ((*token)->type == T_PIPE)
		{
			current = handle_pipe(token, current);
			if (!current)
				return (NULL);
		}
		else if ((*token)->type >= T_REDIR_IN && (*token)->type <= T_HEREDOC)
		{
			if (!handle_redirect(token, current))
				return (NULL);
		}
		else
			*token = (*token)->next;
	}
	return (cmd);
}

/* 释放命令结构 */
void	free_cmds(t_cmd *cmds)
{
	t_cmd		*tmp_cmd;
	t_redirect	*tmp_redir;
	t_redirect	*next_redir;
	int			i;

	while (cmds)
	{
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
				free(cmds->args[i++]);
			free(cmds->args);
		}
		tmp_redir = cmds->redirects;
		while (tmp_redir)
		{
			next_redir = tmp_redir->next;
			if (tmp_redir->file)
				free(tmp_redir->file);
			free(tmp_redir);
			tmp_redir = next_redir;
		}
		tmp_cmd = cmds;
		cmds = cmds->next;
		free(tmp_cmd);
	}
}

/* 解析器主函数 */
t_cmd	*parser(t_token *tokens)
{
	t_token	*token_ptr;
	t_cmd	*cmd_list;

	token_ptr = tokens;
	
	// 删除或注释下面的调试输出代码
	/*
	printf("Parsed tokens:\n");
	t_token *tmp = tokens;
	while (tmp)
	{
		printf("  Token: '%s', Type: %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
	*/
	
	cmd_list = parse_cmd(&token_ptr);
	return (cmd_list);
} 