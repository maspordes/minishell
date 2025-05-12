/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/05 00:37:01 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 创建新标记 */
static t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

/* 添加标记到链表 */
static void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

/* 处理引号内的内容 */
static int	handle_quotes(char *input, int *i, char **word)
{
	char	quote;
	int		start;

	quote = input[*i];
	start = *i;
	(*i)++;
	while (input[*i])
	{
		if (input[*i] == quote)
		{
			(*i)++;
			*word = ft_strjoin(*word, ft_substr(input, start, *i - start));
			return (1);
		}
		(*i)++;
	}
	*word = ft_strjoin(*word, ft_substr(input, start, *i - start));
	return (0);
}

/* 处理特殊字符 */
static int	handle_special_char(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, new_token(ft_strdup("|"), T_PIPE));
		(*i)++;
		return (1);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, new_token(ft_strdup("<<"), T_HEREDOC));
			(*i) += 2;
		}
		else
		{
			add_token(tokens, new_token(ft_strdup("<"), T_REDIR_IN));
			(*i)++;
		}
		return (1);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, new_token(ft_strdup(">>"), T_REDIR_APPEND));
			(*i) += 2;
		}
		else
		{
			add_token(tokens, new_token(ft_strdup(">"), T_REDIR_OUT));
			(*i)++;
		}
		return (1);
	}
	return (0);
}

/* 处理单词 */
static void	handle_word(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*word;
	char	*temp;

	word = ft_strdup("");
	while (input[*i])
	{
		if (input[*i] == ' ' || input[*i] == '\t')
			break;
		else if (input[*i] == '\'' || input[*i] == '\"')
		{
			if (!handle_quotes(input, i, &word))
			{
				temp = word;
				word = ft_strjoin(word, ft_substr(input, *i, ft_strlen(input) - *i));
				free(temp);
				*i = ft_strlen(input);
				break;
			}
		}
		else if (handle_special_char(input, i, tokens))
			break;
		else
		{
			start = *i;
			while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
				&& input[*i] != '\'' && input[*i] != '\"'
				&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
				(*i)++;
			temp = word;
			word = ft_strjoin(word, ft_substr(input, start, *i - start));
			free(temp);
		}
	}
	if (ft_strlen(word) > 0)
		add_token(tokens, new_token(word, T_WORD));
	else
		free(word);
}

/* 词法分析主函数 */
t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (handle_special_char(input, &i, &tokens))
			continue ;
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}

/* 释放标记链表 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
