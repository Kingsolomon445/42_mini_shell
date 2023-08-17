/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:14:05 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/16 20:24:56 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_export(char **env, int equalcount)
{
	int			i;
	int			j;

	i = -1;
	j = -1;
	while (env[++i])
	{
		ft_printf_fd(1, "declare -x ");
		while (env[i][++j])
		{
			ft_printf_fd(1, "%c", env[i][j]);
			if (env[i][j] == '=' && !equalcount)
			{
				equalcount = 1;
				ft_printf_fd(1, "\"");
			}
			if (env[i][j + 1] == '\0' && equalcount)
				ft_printf_fd(1, "\"\n");
			else if (env[i][j + 1] == '\0' && !equalcount)
				ft_printf_fd(1, "\n");
		}
		equalcount = 0;
		j = -1;
	}
}

static int	check_for_error(t_shell *shell, char *env_title, \
char *full_env)
{
	if (!check_valid_identifier(env_title) && ft_isalpha(*env_title))
	{
		if (*env_title == '-')
		{
			shell->last_status = 2;
			invalid_option("export", env_title);
		}
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
			{
				shell->last_status = 2;
				invalid_option("export", full_env);
			}
			else
				shell->last_status = \
				print_error(1, "export", full_env, INVALIDID);
			return (0);
		}
	}
	return (1);
}

static char *get_env_substr(char *full_env, char *env_title, int i)
{
	char *substr;

	if (ft_strlen(full_env) > ft_strlen(env_title) + 1)
		substr = ft_substr(full_env, ft_strlen(env_title) + i, \
		ft_strlen(full_env + ft_strlen(env_title) + i));
	else
		substr = ft_strdup("");
	return (substr);
}

static char *export_apend(t_shell *shell, char **full_env, char **env_title)
{
	char *variable;
	char *temp;
	char *equalsto;

	equalsto = ft_strchr(*full_env, '=');
	temp = NULL;
	if (equalsto && *(equalsto - 1) && *(equalsto - 1) == '+')
	{
		*env_title = ft_substr(*full_env, 0, (equalsto - 1) - *full_env);
		if (!check_for_error(shell, *env_title, *full_env))
			return (NULL);
		temp = get_env_substr(*full_env, *env_title, 2);
		if (getenv(*env_title))
			variable = ft_strjoin(getenv(*env_title), temp);
		else
			variable = ft_strjoin(ft_strdup(""), temp);
		ft_free(*full_env);
		*full_env = ft_strjoin(*env_title, equalsto);
	}
	else
	{
		*env_title = ft_substr(*full_env, 0, equalsto - *full_env);
		variable = get_env_substr(*full_env, *env_title, 1);
	}
	return (ft_free(temp), variable);
}

static int	export_more_envs(t_shell *shell, int i)
{
	char	*full_env;
	char	*env_title;
	char	*variable;

	full_env = ft_strdup(shell->cmd_head->toks[i]);
	env_title = NULL;
	variable = export_apend(shell, &full_env, &env_title);
	if (!variable)
		return (ft_free(env_title), ft_free(full_env), 0);
	if (!check_for_error(shell, env_title, full_env))
		return (ft_free(env_title), ft_free(full_env), 0);
	if (check_env(shell->env, env_title) && \
	full_env[ft_strlen(env_title)] == '=')
			update_env_item(shell, env_title, variable);
	else if (!check_env(shell->env, env_title))
		if (!ft_putenv(shell, full_env))
			return (ft_free(env_title), ft_free(full_env), 0);
	return (1);
}

int	export_env(t_shell *shell)
{
	int	i;

	i = 1;
	if (shell->cmd_head->toks[1] == NULL)
	{
		print_export(shell->env, 0);
		return (0);
	}
	else
	{
		while (shell->cmd_head->toks[i])
		{
			if (*(shell->cmd_head->toks[i]) == '=')
				return (print_error(1, "export", \
				shell->cmd_head->toks[i], INVALIDID));
			else
			{
				if (!export_more_envs(shell, i))
					return (shell->last_status);
			}
			i++;
		}
	}
	return (0);
}
