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

void update_env_item(t_shell *shell, char *env_title, char *new_env)
{
	int		i;
	char	*temp;
	char	*env_var;
	extern char	**environ;

	i = 0;
	temp = NULL;
	env_var = getenv(env_title);
	if (env_var)
	{
		while (shell->env[i])
		{
			if (ft_strnstr(shell->env[i], env_title, ft_strlen(env_title)))
			{
				ft_free(shell->env[i]);
				temp = ft_strjoin(env_title, "=");
				shell->env[i] = ft_strjoin(temp, new_env);
				free(temp);
				environ = shell->env;
				break;
			}
			i++;
		}
	}
}

void change_directory(t_shell *shell, t_commands *cmd)
{
	char	*old_dir;
	char	*current_dir;
	
	old_dir = getcwd(NULL, 0);
	if (!old_dir)
	{
		perror("minishell: cd");
		shell->last_status = 1;
		return;
	}

	if (chdir(cmd->toks[1]) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", cmd->toks[1]);
		perror("");
		shell->last_status = 1;
	}
	else
	{
		current_dir = getcwd(NULL, 0);
		if (!current_dir)
		{
			perror("minishell: cd");
			ft_free(old_dir);
			shell->last_status = 1;
			return;
		}

		update_env_item(shell, "PWD", current_dir);
		update_env_item(shell, "OLDPWD", old_dir);
		ft_free(current_dir);
		shell->last_status = 0;
	}

	ft_free(old_dir);
}