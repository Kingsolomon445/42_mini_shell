/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:44:10 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/05 19:10:44 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	check_valid_identifier(char *env_title)
{
	int	i;

	i = 0;
	while (env_title[i])
	{
		if (!ft_isalnum(env_title[i]) && env_title[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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
	shell->last_status = 0;
	if (!env_title)
		return ;
	if (!check_valid_identifier(env_title))
	{
		ft_printf_fd(2, "minishell: unset: `%s': not a valid identifier\n", env_title);
		shell->last_status = 1;
		return ;
	}
	envlen = env_len(shell->env);
	if (getenv(env_title))
	{
		updated_env = (char **)malloc(envlen * sizeof(char *));
		if (!updated_env)
			return ;
		while (shell->env[j])
		{
			if (!ft_strnstr(shell->env[j], env_title, ft_strlen(env_title)))
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
