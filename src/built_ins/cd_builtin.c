/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:19:32 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/06 20:23:11 by sbhatta          ###   ########.fr       */
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
	if (check_env(shell->env, env_title))
	{
		while (environ[i] && !ft_strnstr(environ[i], env_title, \
		ft_strlen(env_title)))
			i++;
		if (environ[i])
		{
			ft_free(shell->env[i]);
			temp = ft_strjoin(env_title, "=");
			shell->env[i] = ft_strjoin(temp, new_env);
			free(temp);
			environ = shell->env;
		}
	}
}

char	*eval_cd_args(t_shell *shell, t_commands *cmd, char *dir)
{
	if (compare_cmd("-", cmd->toks[1]))
	{
		dir = getenv("OLDPWD");
		if (dir)
			printf("%s\n", dir);
		else
		{
			ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
			shell->last_status = 1;
			return (NULL);
		}
	}
	if (compare_cmd("~", cmd->toks[1]))
		dir = getenv("HOME");
	return (dir);
}

int	update_pwd(t_shell *shell, char *old_dir)
{
	char	*current_dir;
	
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ft_free(old_dir), 0);
	if (getenv("PWD"))
		update_env_item(shell, "PWD", current_dir);
	else
	{
		ft_putenv(shell, ft_strdup("PWD="));
		update_env_item(shell, "PWD", current_dir);
	}
	ft_free(current_dir);
	if (getenv("OLDPWD"))
		update_env_item(shell, "OLDPWD", old_dir);
	else
	{
		ft_putenv(shell, ft_strdup("OLDPWD="));
		update_env_item(shell, "OLDPWD", old_dir);
	}
	shell->last_status = 0;
	return(1);
}

int	change_directory(t_shell *shell, t_commands *cmd)
{
	char	*old_dir;
	char 	*dir;
	int		result;

	result = 1;
	if (!cmd->toks[1])
	{
		shell->last_status = 0;
		return (1);
	}
	dir = cmd->toks[1];
	old_dir = getcwd(NULL, 0);
	dir = eval_cd_args(shell, cmd, dir);
	if (!dir)
		return (1);
	if (chdir(dir) == -1)
	{
		ft_printf_fd(2, "minishell: cd: %s: ", cmd->toks[1]);
		perror("");
		shell->last_status = 1;
	}
	else
		result = update_pwd(shell, old_dir);
	return (ft_free(old_dir), result);
}
//need to change env PWD and OLDPWD when directory is changed.
//cat Makefile | grep m | head -c 5
//cat | ls needs to print ls first and cat should wait for one