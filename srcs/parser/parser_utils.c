/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:04:07 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*new_cmd(void)
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

t_redirect	*new_redirect(int type, char *file)
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

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

static void	free_redirects(t_redirect *redirects)
{
	t_redirect	*tmp_redir;
	t_redirect	*next_redir;

	tmp_redir = redirects;
	while (tmp_redir)
	{
		next_redir = tmp_redir->next;
		if (tmp_redir->file)
			free(tmp_redir->file);
		free(tmp_redir);
		tmp_redir = next_redir;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;

	while (cmds)
	{
		free_args(cmds->args);
		free_redirects(cmds->redirects);
		tmp_cmd = cmds;
		cmds = cmds->next;
		free(tmp_cmd);
	}
}
