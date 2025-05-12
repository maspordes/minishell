/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:57 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 13:43:17 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 检查字符串是否为有效的整数 */
static int	is_numeric_argument(const char *str)
{
    int	i;
    int	len;

    if (!str || !*str)
        return (0);

    // Handle quoted strings
    len = ft_strlen(str);
    if ((str[0] == '"' || str[0] == '\'') && str[len - 1] == str[0])
    {
        // Remove quotes for validation
        str = ft_substr(str, 1, len - 2);
        if (!str)
            return (0);
    }

    i = 0;
    // Skip leading '+' or '-'
    if (str[i] == '+' || str[i] == '-')
        i++;

    // Must have at least one digit after sign
    if (!str[i])
        return (0);

    // Check remaining characters are digits
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

/* 将字符串转换为长整型 */
long	ft_atol(const char *str)
{
    long	result;
    int		sign;

    result = 0;
    sign = 1;
    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str && ft_isdigit(*str))
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}

/* 提取字符串中的键 */
char	*get_key_from_str(const char *str)
{
    int		i;
    char	*key;

    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    key = ft_substr(str, 0, i); // Extract the key
    return (key);
}

/* exit 命令的实现 */
int	ft_exit(char **args, t_shell *shell)
{
    long	exit_code;
    char	*arg;
    char	*clean_arg;

    // Print "exit" when exiting an interactive shell
    if (shell->cmd_list && !shell->cmd_list->next)
        printf("exit\n");

    // No arguments: exit with the current exit status
    if (!args[1])
        exit(shell->exit_status);

    // Handle quoted arguments
    arg = args[1];
    clean_arg = NULL;
    if (arg[0] == '"' || arg[0] == '\'')
    {
        // Remove quotes
        clean_arg = ft_substr(arg, 1, ft_strlen(arg) - 2);
        if (!clean_arg)
            exit(255);
        arg = clean_arg;
    }

    // Validate the argument
    if (!is_numeric_argument(args[1]))
    {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
        free(clean_arg);
        exit(255);
    }

    // Convert the argument to a long and handle overflow
    exit_code = ft_atol(arg);
    free(clean_arg);

    if (exit_code < 0 || exit_code > 255)
        exit_code %= 256;
    if (exit_code < 0)
        exit_code += 256;

    // Too many arguments: print an error and return
    if (args[2])
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return (1);
    }

    // Exit with the calculated exit code
    exit(exit_code);
}
