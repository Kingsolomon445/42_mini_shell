/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:14:05 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 16:43:09 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_for_error(t_shell *shell, char *env_title, \
char *full_env)
{
	if (!check_valid_identifier(env_title) && ft_isalpha(*env_title))
	{
		if (*env_title == '-')
			return (set_status(shell, 2), \
			invalid_option("export", env_title), 0);
		else if (env_title[ft_strlen(env_title) - 1] == '+')
			print_error(1, "export", full_env, INVALIDID);
		else
			shell->last_status = print_error(1, "export", env_title, INVALIDID);
		return (0);
	}
	else if (!ft_isalpha(*env_title))
	{
		if (*env_title != '_')
		{
			if (*env_title == '-')
				return (set_status(shell, 2), \
				invalid_option("export", full_env), 0);
			else
				shell->last_status = \
				print_error(1, "export", full_env, INVALIDID);
			return (0);
		}
	}
	return (1);
}

static char	*get_env_substr(char *full_env, char *env_title, int i)
{
	char	*substr;

	if (ft_strlen(full_env) > ft_strlen(env_title) + 1)
		substr = ft_substr(full_env, ft_strlen(env_title) + i, \
		ft_strlen(full_env + ft_strlen(env_title) + i));
	else
		substr = ft_strdup("");
	return (substr);
}

char	*when_plus_in_export(t_shell *shell, char **env_title, \
char **full_env, char *equalsto)
{
	char	*variable;
	char	*temp;

	*env_title = ft_substr(*full_env, 0, (equalsto - 1) - *full_env);
	if (!*env_title)
		return (NULL);
	if (!check_for_error(shell, *env_title, *full_env))
		return (NULL);
	temp = get_env_substr(*full_env, *env_title, 2);
	if (!temp)
		return (NULL);
	if (getenv(*env_title))
		variable = ft_strjoin(getenv(*env_title), temp);
	else
		variable = ft_strjoin("", temp);
	ft_free(temp);
	if (!variable)
		return (NULL);
	temp = ft_strjoin(*env_title, equalsto);
	ft_free(*full_env);
	*full_env = temp;
	if (!*full_env)
		return (ft_free(variable), NULL);
	return (variable);
}

static char	*export_apend(t_shell *shell, char **full_env, char **env_title)
{
	char	*variable;
	char	*equalsto;

	equalsto = ft_strchr(*full_env, '=');
	variable = NULL;
	if (equalsto && *(equalsto - 1) && *(equalsto - 1) == '+')
	{
		variable = when_plus_in_export(shell, env_title, full_env, equalsto);
		if (!variable)
			return (NULL);
	}
	else if (equalsto)
	{
		*env_title = ft_substr(*full_env, 0, equalsto - *full_env);
		if (!*env_title)
			return (NULL);
		variable = get_env_substr(*full_env, *env_title, 1);
		if (!variable)
			return (NULL);
	}
	else
		*env_title = ft_strdup(*full_env);
	return (variable);
}

int	export_more_envs(t_shell *shell, int i)
{
	char	*full_env;
	char	*env_title;
	char	*variable;

	full_env = ft_strdup(shell->cmd_head->toks[i]);
	if (!full_env)
		return (0);
	env_title = NULL;
	variable = export_apend(shell, &full_env, &env_title);
	if (!variable)
		return (ft_free(env_title), ft_free(full_env), 0);
	if (!check_for_error(shell, env_title, full_env))
		return (ft_free(env_title), ft_free(variable), ft_free(full_env), 0);
	if (check_env(shell->env, env_title) && \
	full_env[ft_strlen(env_title)] == '=')
	{
		if (!update_env_item(shell, env_title, variable))
			return (ft_free(env_title), ft_free(variable), \
			ft_free(full_env), 0);
	}
	else if (!check_env(shell->env, env_title))
		if (!ft_putenv(shell, full_env))
			return (ft_free(env_title), ft_free(variable), \
			ft_free(full_env), 0);
	return (ft_free(env_title), ft_free(variable), ft_free(full_env), 1);
}
