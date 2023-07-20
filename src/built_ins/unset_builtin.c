/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:44:10 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/19 17:44:59 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_unset(t_shell *shell, char *env_title)
{
	int			i;
	int			j;
	int			envlen;
	char		**updated_env;
	extern char	**environ;

	i = 0;
	j = 0;
	updated_env = NULL;
	envlen = env_len(shell->env);
	if (getenv(env_title))
	{
		updated_env = (char **)malloc(envlen * sizeof(char *));
		if (!updated_env)
			return ;
		while (shell->env[j])
		{
			if (!ft_strnstr(shell->env[j], env_title, ft_strlen(shell->env[j])))
			{
				updated_env[i] = shell->env[j];
				i++;
			}
			else
				ft_free(shell->env[j]);
			j++;
		}
		updated_env[j] = NULL;
		free(shell->env);
		shell->env = updated_env;
		environ = shell->env;
	}
}