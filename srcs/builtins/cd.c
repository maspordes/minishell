/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:46:46 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:46:48 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* cd 命令的实现 */
int	ft_cd(char **args, t_env **env_list)
{
    char	*path;

    // Handle "too many arguments" error
    if (args[1] && args[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return (1);
    }

    // Expand environment variables in the path
    path = args[1] ? expand_variables(args[1], *env_list, 0) : get_env_value(*env_list, "HOME");
    if (!path || chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
        free(path);
        return (1);
    }

    // Update PWD and OLDPWD environment variables
    set_env_value(env_list, "OLDPWD", get_env_value(*env_list, "PWD"));
    set_env_value(env_list, "PWD", getcwd(NULL, 0));
    free(path);
    return (0);
}

char	*expand_variables(char *str, t_env *env_list, int exit_status)
{
    char	*expanded;
    char	*key;
    char	*value;
    int		i;

    expanded = ft_strdup("");
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++;
            key = get_key_from_str(&str[i]); // Extract the key after '$'
            value = ft_strcmp(key, "?") == 0 ? ft_itoa(exit_status) : get_env_value(env_list, key);
            expanded = ft_strjoin_free(expanded, value ? value : "");
            free(key);
            if (value && ft_strcmp(key, "?") != 0)
                free(value);
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
        }
        else
            expanded = ft_strjoin_char(expanded, str[i++]);
    }
    return (expanded);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
    char	*result;

    if (!s1 || !s2)
        return (NULL);
    result = ft_strjoin(s1, s2); // Use your existing ft_strjoin function
    free(s1); // Free the first string
    return (result);
}

char	*ft_strjoin_char(char *s1, char c)
{
    char	*result;
    int		len;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    result = malloc(len + 2); // Allocate space for the string + 1 char + null terminator
    if (!result)
        return (NULL);
    ft_memcpy(result, s1, len);
    result[len] = c;
    result[len + 1] = '\0';
    free(s1); // Free the original string
    return (result);
}

int	is_numeric_argument(const char *str)
{
    int	i;

    if (!str || !*str)
        return (0);

    // Skip leading '+' or '-'
    if (str[0] == '+' || str[0] == '-')
        str++;

    // Ensure all remaining characters are digits
    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
