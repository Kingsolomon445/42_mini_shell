/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:19:32 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/19 13:06:44 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_env_item(t_shell *shell, char *env_title, char *new_env)
{
	extern char		**environ;
	int				i;
	char			*temp;

	i = 0;
	temp = NULL;
	if (getenv(env_title))
	{
		while (!ft_strnstr(environ[i], env_title, \
		ft_strlen(environ[i])) && environ[i])
			i++;
		if (i)
		{
			ft_free(shell->env[i]);
			temp = ft_strjoin(env_title, "=");
			shell->env[i] = ft_strjoin(temp, new_env);
			free(temp);
			environ = shell->env;
		}
	}
}

void	change_directory(t_shell *shell, t_commands *cmd)
{
	char	*current_dir;
	char	*old_dir;

	current_dir = NULL;
	old_dir = NULL;
	old_dir = getcwd(old_dir, 0);
	if (chdir(cmd->toks[1]) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s", cmd->toks[1]);
		perror(" ");
		shell->last_status = 1;
	}
	else
	{
		current_dir = getcwd(current_dir, 0);
		update_env_item(shell, "PWD", current_dir);
		ft_free(current_dir);
		update_env_item(shell, "OLDPWD", old_dir);
		shell->last_status = 0;
	}
	ft_free(old_dir);
}
//need to change env PWD and OLDPWD when directory is changed.
//cat Makefile | grep m | head -c 5
//cat | ls needs to print ls first and cat should wait for one