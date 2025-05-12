/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:46:46 by shutan            #+#    #+#             */
/*   Updated: 2025/05/12 20:29:28 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Helper function to update PWD and OLDPWD */
static int	update_pwd_env(t_env **env_list, char *old_pwd_val)
{
	char	*new_pwd_val;

	new_pwd_val = getcwd(NULL, 0);
	if (!new_pwd_val)
	{
		perror("cd: getcwd error");
		/* OLDPWD might still be useful, but PWD update failed */
		if (old_pwd_val)
			set_env_value(env_list, "OLDPWD", old_pwd_val);
		return (1); /* Indicate failure */
	}
	if (old_pwd_val)
		set_env_value(env_list, "OLDPWD", old_pwd_val);
	set_env_value(env_list, "PWD", new_pwd_val);
	free(new_pwd_val); /* Free the value returned by getcwd */
	return (0); /* Indicate success */
}

/* Helper function to determine the target path for cd */
static char	*get_target_path(char **args, t_env *env_list)
{
	char	*path_arg;
	char	*target_path;
	char	*env_val;

	path_arg = args[1];
	if (!path_arg)
	{
		/* Case: cd */
		env_val = get_env_value(env_list, "HOME");
		if (!env_val)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (NULL);
		}
		target_path = ft_strdup(env_val);
	}
	else if (ft_strcmp(path_arg, "-") == 0)
	{
		/* Case: cd - */
		env_val = get_env_value(env_list, "OLDPWD");
		if (!env_val)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		target_path = ft_strdup(env_val);
		/* Print the directory being changed to, like bash */
		printf("%s\n", target_path);
	}
	else
	{
		/* Case: cd <path> */
		/* Note: Expansion should ideally happen before this point */
		/* Assuming args[1] is the final path for now */
		target_path = ft_strdup(path_arg);
	}
	if (!target_path)
		perror("cd: memory allocation error");
	return (target_path);
}

/* cd 命令的实现 */
int	ft_cd(char **args, t_env **env_list)
{
	char	*target_path;
	char	*old_pwd_val;
	int		result;

	/* Handle "too many arguments" error */
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	/* Get current PWD to set OLDPWD later */
	/* For now, stick to env variable for consistency */
	old_pwd_val = get_env_value(*env_list, "PWD");
	if (old_pwd_val)
		old_pwd_val = ft_strdup(old_pwd_val);
	/* else: OLDPWD won't be set if PWD wasn't set */

	/* Determine the target path (handles NULL, "-", <path>) */
	target_path = get_target_path(args, *env_list);
	if (!target_path)
	{
		free(old_pwd_val); /* Free if strdup'd */
		return (1); /* Error message printed in get_target_path */
	}
	/* Change directory */
	if (chdir(target_path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: ", args[1] ? args[1] : "(HOME)");
		perror(NULL);
		free(target_path);
		free(old_pwd_val);
		return (1);
	}
	/* Update PWD and OLDPWD environment variables */
	result = update_pwd_env(env_list, old_pwd_val);

	/* Cleanup */
	free(target_path);
	free(old_pwd_val);
	return (result);
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
