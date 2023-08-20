/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:21:14 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 18:25:21 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	when_cap_cd(t_shell *shell, char **toks)
{
	if (!compare_str("CD", toks[0]))
		return (print_error(127, NULL, toks[0], NOCMDFOUND));
	else
	{
		shell->last_status = 0;
		return (0);
	}
}

int	ft_execute_one_builtin(t_commands *cmd, t_shell *shell)
{
	if (cmd->red)
		handle_redirs(cmd);
	if (compare_str("cd", cmd->toks[0]))
		return (change_directory(shell, cmd));
	else if (compare_str("export", cmd->toks[0]))
		return (export_env(shell));
	else if (compare_str("history", cmd->toks[0]) == 1)
		return (print_history(shell));
	else if (compare_builtin("pwd", cmd->toks[0]))
		return (get_pwd(shell));
	else if (compare_builtin("env", cmd->toks[0]))
		return (env_main(shell, cmd->toks));
	else if (compare_str("unset", cmd->toks[0]))
		return (unset_main(shell));
	else if (compare_builtin("echo", cmd->toks[0]))
		return (echo_echo(cmd));
	else if (compare_str("exit", cmd->toks[0]))
		return (perform_exit(cmd));
	else
		return (when_cap_cd(shell, cmd->toks));
}

int	ft_execute_builtin(t_commands *cur_cmd, t_shell *shell)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
	{
		status = ft_execute_one_builtin(cur_cmd, shell);
		ft_exit_shell(shell, status);
	}
	status = 0;
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

void	when_one_builtin(t_shell *shell, t_commands *cur_cmd)
{
	if (compare_str("exit", cur_cmd->toks[0]))
	{
		shell->last_status = perform_exit(cur_cmd);
		if (!(cur_cmd->toks[1] && ft_isnumber(cur_cmd->toks[1]) \
		&& cur_cmd->toks[2]))
			ft_exit_shell(shell, shell->last_status);
	}
	else if (compare_str("export", cur_cmd->toks[0]))
		shell->last_status = export_env(shell);
	else if (compare_str("unset", cur_cmd->toks[0]))
		shell->last_status = unset_var(shell);
	else if (compare_str("cd", cur_cmd->toks[0]))
		shell->last_status = change_directory(shell, cur_cmd);
	else if (compare_str("history", cur_cmd->toks[0]) == 1)
		shell->last_status = print_history(shell);
	else
		shell->last_status = ft_execute_builtin(cur_cmd, shell);
}

void	print_export(char **env, int equalcount)
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
