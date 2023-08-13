/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:21:14 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/13 14:11:24 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_it_builtin(char **builtins, char *cmd)
{
	int	i;

	i = 0;
	while (*(builtins + i))
	{
		if (compare_builtin(*(builtins + i), cmd))
			return (1);
		i++;
	}
	return (0);
}

static void	when_cap_cd(t_shell *shell, char **toks)
{
	if (!compare_str("CD", toks[0]))
		exit (print_error(127, NULL, toks[0], NOCMDFOUND));
	else
	{
		shell->last_status = 0;
		exit (0);
	}
}

void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell)
{
	if (cmd->red)
		handle_redirections(cmd);
	if (compare_str("cd", cmd->toks[0]))
		exit(change_directory(shell, cmd));
	else if (compare_str("export", cmd->toks[0]))
		exit (export_env(shell));
	else if (compare_str("history", cmd->toks[0]) == 1)
		print_history(shell);
	else if (compare_builtin("pwd", cmd->toks[0]))
		exit(get_pwd(shell));
	else if (compare_builtin("env", cmd->toks[0]))
		env_main(shell, cmd->toks);
	else if (compare_str("unset", cmd->toks[0]))
		unset_main(shell);
	else if (compare_builtin("echo", cmd->toks[0]))
		echo_echo(cmd);
	else if (compare_str("exit", cmd->toks[0]))
		perform_exit(cmd, shell);
	else
		when_cap_cd(shell, cmd->toks);
}

void	ft_execute_builtin(t_commands *cur_cmd, t_shell *shell)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
		ft_execute_one_builtin(cur_cmd, shell);
	status = 0;
	waitpid(pid, &status, 0);
	shell->last_status = WEXITSTATUS(status);
}

void	when_one_builtin(t_shell *shell, t_commands *cur_cmd)
{
	if (compare_str("exit", cur_cmd->toks[0]))
		perform_exit(cur_cmd, shell);
	else if (compare_str("export", cur_cmd->toks[0]))
		export_env(shell);
	else if (compare_str("unset", cur_cmd->toks[0]))
		unset_var(shell);
	else if (compare_str("cd", cur_cmd->toks[0]))
		change_directory(shell, cur_cmd);
	else
		ft_execute_builtin(cur_cmd, shell);
}
