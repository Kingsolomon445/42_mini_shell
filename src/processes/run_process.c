/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:26:40 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/18 15:05:21 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../include/minishell.h"

int	ft_dup(int fds[2], int in)
{
	if (in)
	{
		close(fds[1]);
		if (dup2(fds[0], STDIN_FILENO) == -1)
			return (0);
		close(fds[0]);
	}
	else
	{
		close(fds[0]);
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			return (0);
		close(fds[1]);
	}
	return (1);
}

int	run_in_child_process(t_commands *cmd, t_shell *shell, int fds[2])
{
	if (g_ctrlc)
		ft_exit_shell(shell, 1);
	if (cmd->cmd_pos == 1)
		shell->sucess = ft_dup(cmd->fds, 0);
	else if (cmd->cmd_pos == shell->no_cmds)
		shell->sucess = ft_dup(fds, 1);
	else
	{
		if (!ft_dup(fds, 1))
			print_error_exit(shell, strerror(errno), 1);
		else
			shell->sucess = ft_dup(cmd->fds, 0);
	}
	if (!shell->sucess)
		print_error_exit(shell, strerror(errno), 1);
	if (cmd->do_not_run)
		ft_exit_shell(shell, 1);
	if (!is_it_builtin(shell->builtins, cmd->toks[0]))
		return (ft_exec_in_child_process(shell, cmd));
	return (ft_execute_one_builtin(cmd, shell));
}

void	close_fds(int fd_in, int fd_out)
{
	close(fd_in);
	close(fd_out);
}

static void	in_child(t_shell *shell, t_commands *cmd, int fds[2], int *status)
{
	*status = run_in_child_process(cmd, shell, fds);
	if (cmd->cmd_pos == shell->no_cmds)
		close_fds(cmd->fds[0], cmd->fds[1]);
	ft_exit_shell(shell, *status);
}

void	run_processes(t_commands *cmd, t_shell *shell, int fds[2])
{
	int			pid;
	int			status;
	t_commands	*prev_cmd;	

	shell->sucess = 0;
	if (pipe(cmd->fds) < 0)
		exit(1);
	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
		in_child(shell, cmd, fds, &status);
	if (cmd->cmd_pos != 1)
		close_fds(fds[0], fds[1]);
	prev_cmd = cmd;
	cmd = cmd->next;
	if (cmd)
		run_processes(cmd, shell, prev_cmd->fds);
	close_fds(prev_cmd->fds[0], prev_cmd->fds[1]);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
}
