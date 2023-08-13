/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:14:05 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/13 14:04:16 by ofadahun         ###   ########.fr       */
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
char *full_env, int *error)
{
	if (!check_valid_identifier(env_title) && ft_isalpha(*env_title))
	{
		if (*env_title == '-')
			invalid_option(shell, "export", env_title);
		else
			shell->last_status = print_error(1, "export", env_title, INVALIDID);
		*error = 1;
	}
	else if (!ft_isalpha(*env_title))
	{
		if (*env_title != '_')
		{
			if (*env_title == '-')
				invalid_option(shell, "export", full_env);
			else
				shell->last_status = \
				print_error(1, "export", full_env, INVALIDID);
			*error = 1;
		}
	}
	if (*error)
		return (0);
	return (1);
}

static int	export_more_envs(t_shell *shell, int i, int error)
{
	char	*equalsto;
	char	*full_env;
	char	*env_title;
	char	*variable;

	full_env = ft_strdup(shell->cmd_head->toks[i]);
	equalsto = ft_strchr(full_env, '=');
	env_title = ft_substr(full_env, 0, equalsto - full_env);
	variable = ft_substr(full_env, ft_strlen(env_title) + 1, \
	ft_strlen(full_env + ft_strlen(env_title) + 1));
	if (!check_for_error(shell, env_title, full_env, &error))
		return (ft_free(full_env), 0);
	if (check_env(shell->env, env_title) && \
	full_env[ft_strlen(env_title)] == '=')
	{
		if (variable)
			update_env_item(shell, env_title, variable);
		else
			update_env_item(shell, env_title, ft_strdup("\0"));
	}
	else if (!check_env(shell->env, env_title))
		if (!ft_putenv(shell, full_env))
			return (ft_free(full_env), 0);
	return (1);
}

int	export_env(t_shell *shell)
{
	int	i;

	i = 1;
	shell->last_status = 0;
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
				shell->last_status = print_error(1, "export", \
				shell->cmd_head->toks[i], INVALIDID);
			else
				export_more_envs(shell, i, 0);
			i++;
		}
	}
	return (0);
}
