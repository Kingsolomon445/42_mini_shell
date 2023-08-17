/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:44:10 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 18:20:56 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_valid_identifier(char *env_title)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(*env_title) || *env_title == '_'))
		return (0);
	while (env_title[i])
	{
		if (!ft_isalnum(env_title[i]) && env_title[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**unset_looper(t_shell *shell, char *env_title, char **updated_env)
{
	int			i;
	int			j;
	char		*new_env_title;

	i = 0;
	j = 0;
	new_env_title = ft_strjoin(env_title, "=");
	if (!new_env_title)
		return (NULL);
	while (shell->env[j])
	{
		if (!ft_strnstr(shell->env[j], new_env_title, ft_strlen(new_env_title)))
		{
			updated_env[i] = shell->env[j];
			i++;
		}
		else
			ft_free(shell->env[j]);
		j++;
	}
	ft_free(new_env_title);
	updated_env[i] = NULL;
	return (updated_env);
}

static int	run_unset(t_shell *shell, char *env_title)
{
	int			envlen;
	char		**updated_env;
	extern char	**environ;

	updated_env = NULL;
	if (!env_title || !shell->env)
		return (1);
	envlen = env_len(shell->env);
	if (check_env(shell->env, env_title))
	{
		updated_env = (char **)malloc(envlen * sizeof(char *));
		if (!updated_env)
			return (0);
		updated_env = unset_looper(shell, env_title, updated_env);
		ft_free(shell->env);
		shell->env = updated_env;
		environ = shell->env;
	}
	return (1);
}

int	unset_var(t_shell *shell)
{
	int	i;

	i = 1;
	while (shell->cmd_head->toks[i])
	{
		if (!check_valid_identifier(shell->cmd_head->toks[i]))
		{
			if (*shell->cmd_head->toks[i] == '-')
			{
				invalid_option("unset", shell->cmd_head->toks[i]);
				return (2);
			}
			else
				return (print_error(1, "unset", \
				shell->cmd_head->toks[i], INVALIDID));
		}
		else
			if (!run_unset(shell, shell->cmd_head->toks[i]))
				return (12);
		i++;
	}
	return (0);
}

int	unset_main(t_shell *shell)
{
	unset_var(shell);
	return (shell->last_status);
}
