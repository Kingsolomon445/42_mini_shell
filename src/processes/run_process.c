/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:26:40 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/20 12:07:59 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    ft_dup(int fds[2], int in)
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


void    run_in_child_process(t_commands *cmd, t_shell *shell, int fds[2])
{
    extern char	**environ;

    if (cmd->cmd_pos == 1)
        shell->sucess = ft_dup(cmd->fds, 0);
    else if (cmd->cmd_pos == shell->no_cmds)
        shell->sucess = ft_dup(fds, 1);
    else
    {
       shell->sucess = ft_dup(fds, 1);
		if (!shell->sucess)
			exit(EXIT_FAILURE);
		else
        	shell->sucess = ft_dup(cmd->fds, 0);
    }
	if (!shell->sucess)
		exit(EXIT_FAILURE);
    if (!is_it_builtin(shell->builtins, cmd->toks[0]))
        ft_exec_in_child_process(cmd);
    else
       ft_execute_one_builtin(cmd, shell);
    exit(EXIT_SUCCESS);
}

void	run_processes(t_commands *cmd, t_shell *shell, int fds[2])
{
	int	pid;
	t_commands *prev_cmd;	
	int		status;

	shell->sucess = 1;
	if (pipe(cmd->fds) < 0)
		ft_exit_shell(shell, EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		ft_exit_shell(shell, EXIT_FAILURE);
	else if (pid == 0)
        run_in_child_process(cmd, shell, fds);
	if (cmd->cmd_pos != 1)
	{
		close(fds[0]);
		close(fds[1]);
	}
	prev_cmd = cmd;
	cmd = cmd->next;
	if (cmd)
		run_processes(cmd, shell, prev_cmd->fds);
	waitpid(pid, &status, 0);
	if (!shell->sucess)
		ft_exit_shell(shell, EXIT_FAILURE);
	shell->last_status = WIFEXITED(status);
}
