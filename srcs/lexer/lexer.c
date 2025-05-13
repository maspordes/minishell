/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/13 16:00:57 by marrey           ###   ########.fr       */
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

/* Commenting out unused function handle_quotes */
/*
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
*/

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

/* 处理单词或引用 */
static void	handle_word(char *input, int *i, t_token **tokens)
{
	char	*final_word_value;
	char	*segment;
	int		segment_start_index;
	char	quote_char;

	final_word_value = ft_strdup("");
	if (!final_word_value)
		return; /* Malloc error */
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && \
			input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		char *temp;
		segment_start_index = *i;
		segment = NULL;
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote_char = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote_char)
				(*i)++;
			if (input[*i] == quote_char)
				(*i)++;
			segment = ft_substr(input, segment_start_index, *i - segment_start_index);
		}
		else /* Unquoted segment */
		{
			while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && \
					input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && \
					input[*i] != '\'' && input[*i] != '\"')
				(*i)++;
			segment = ft_substr(input, segment_start_index, *i - segment_start_index);
		}
		if (segment)
		{
			temp = final_word_value;
			final_word_value = ft_strjoin(temp, segment);
			free(temp);
			free(segment);
			if (!final_word_value)
				return; /* Malloc error */
		}
		else /* ft_substr likely failed or segment was empty */
		{
			break;
		}
	}
	if (ft_strlen(final_word_value) > 0)
		add_token(tokens, new_token(final_word_value, T_WORD));
	else
		free(final_word_value);
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
		/* Check for special operator tokens first */
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			handle_special_char(input, &i, &tokens); /* This advances i */
		}
		else /* Must be a word or start of a quoted segment */
		{
			handle_word(input, &i, &tokens); /* This should advance i */
		}
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
