/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:45 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:54:45 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export_env(t_env *env_list)
{
    while (env_list)
    {
        printf("declare -x %s", env_list->key);
        if (env_list->value)
            printf("=\"%s\"", env_list->value);
        printf("\n");
        env_list = env_list->next;
    }
}

/* Helper function to validate identifiers */
int	is_valid_identifier(const char *str)
{
    int	i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0); // Must start with a letter or '_'

    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0); // Only alphanumeric or '_' allowed before '='
        i++;
    }
    return (1); // Valid identifier
}

/* export 命令的实现 */
int	ft_export(char **args, t_env **env_list)
{
    int		i;
    int		exit_status;
    char	*key;
    char	*value;

    if (!args[1]) // If no arguments, print the environment variables
    {
        print_export_env(*env_list);
        return (0);
    }

    i = 1;
    exit_status = 0;
    while (args[i])
    {
        // Split the argument into key and value
        key = ft_strdup(args[i]);
        value = ft_strchr(key, '=');
        if (value)
        {
            *value = '\0'; // Null-terminate the key
            value++;       // Move to the value part
        }

        // Validate the key only
        if (!is_valid_identifier(key))
        {
            fprintf(stderr, "not a valid identifier\n");
            exit_status = 1; // Set exit status to 1 for invalid identifier
        }
        else
        {
            // Add or update the environment variable
            add_or_update_env(env_list, args[i]);
        }
        free(key);
        i++;
    }
    return (exit_status);
}
