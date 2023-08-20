/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 16:55:50 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 16:25:44 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_putenv(t_shell *shell, char *new_env)
{
	extern char		**environ;
	int				array_len;
	char			**result;
	int				i;

	i = 0;
	if (!new_env)
		return (0);
	array_len = env_len(shell->env);
	result = (char **)malloc((array_len + 2) * sizeof(char *));
	if (!result)
		return (0);
	while (shell->env[i])
	{
		result[i] = shell->env[i];
		i++;
	}
	result[i] = ft_strdup(new_env);
	if (!result[i])
		return (ft_free_split(result), 0);
	result[++i] = NULL;
	ft_free(shell->env);
	shell->env = result;
	environ = result;
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
