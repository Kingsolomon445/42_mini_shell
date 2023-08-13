/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:19:32 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/13 14:20:49 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_env_item(t_shell *shell, char *env_title, char *new_env)
{
	extern char		**environ;
	int				i;
	char			*temp;

	i = 0;
	temp = NULL;
	if (*env_title != '\0')
	{
		temp = ft_strjoin(env_title, "=");
		if (!temp)
			return (0);
	}
	if (check_env(shell->env, env_title))
	{
		while (shell->env[i] && !ft_strnstr(shell->env[i], temp, \
		ft_strlen(temp)))
			i++;
		if (shell->env[i])
		{
			ft_free(shell->env[i]);
			shell->env[i] = ft_strjoin(temp, new_env);
			if (!shell->env[i])
				return (ft_free(temp), 0);
		}
	}
	if (temp)
		ft_free(temp);
	environ = shell->env;
	return (1);
}

char	*eval_cd_args(t_shell *shell, t_commands *cmd, char *dir)
{
	if (!cmd->toks[1])
		return (dir);
	if (compare_str("-", cmd->toks[1]))
	{
		dir = getenv("OLDPWD");
		if (dir)
			ft_printf_fd(1, "%s\n", dir);
		else
		{
			shell->last_status = print_error(1, NULL, "cd", OLDPWDNOTSET);
			return (NULL);
		}
	}
	if (compare_str("~", cmd->toks[1]) || compare_str("--", cmd->toks[1]))
		dir = getenv("HOME");
	return (dir);
}

static int	update_pwdvars(t_shell *shell, char *dir_type, char *pwdtype)
{
	if (getenv(pwdtype))
	{
		if (!update_env_item(shell, pwdtype, dir_type))
			return (0);
	}
	else
	{
		if (!ft_putenv(shell, ft_strjoin(pwdtype, "=")))
			return (0);
		if (!update_env_item(shell, pwdtype, dir_type))
			return (0);
	}
	return (1);
}

static int	update_pwd(t_shell *shell, char *old_dir)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (ft_free(old_dir), 1);
	if (!update_pwdvars(shell, current_dir, "PWD"))
		return (1);
	ft_free(current_dir);
	if (!update_pwdvars(shell, old_dir, "OLDPWD"))
		return (1);
	return (0);
}

int	change_directory(t_shell *shell, t_commands *cmd)
{
	char	*old_dir;
	char	*dir;
	int		result;

	result = 1;
	if (!cmd->toks[1])
		dir = getenv("HOME");
	else if (compare_str(cmd->toks[1], ""))
		return (set_status(shell, 0), 0);
	else
		dir = cmd->toks[1];
	old_dir = getcwd(NULL, 0);
	dir = eval_cd_args(shell, cmd, dir);
	if (!dir)
		return (0);
	if (chdir(dir) == -1)
		result = print_error(1, "cd", dir, NOFILEDIR);
	else
		result = update_pwd(shell, old_dir);
	if (result == 0)
		shell->last_status = 0;
	else
		shell->last_status = 1;
	return (ft_free(old_dir), result);
}
//need to change env PWD and OLDPWD when directory is changed.
//cat Makefile | grep m | head -c 5
//cat | ls needs to print ls first and cat should wait for one