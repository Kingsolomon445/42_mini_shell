/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:20:12 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/17 17:05:50 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_dup_red(t_redir *red)
{
	if (red->red_type == RED_IN || red->red_type == HEREDOC)
	{
		if (dup2(red->red_fd, STDIN_FILENO) == -1)
			return (0);
	}
	else if (red->red_type == RED_OUT || red->red_type == RED_APPEND)
	{
		if (dup2(red->red_fd, STDOUT_FILENO) == -1)
			return (0);
	}
	close(red->red_fd);
	return (1);
}

int	handle_redirs(t_commands *cmd)
{
	t_redir	*current_red;

	current_red = cmd->red;
	while (current_red)
	{
		if (!ft_dup_red(current_red))
			return (0);
		current_red = current_red->next;
	}
	return (1);
}

int	ft_exec_in_child_process(t_shell *shell, t_commands *cmd)
{
	int			sucess;

	sucess = 1;
	if (cmd->red)
		sucess = handle_redirs(cmd);
	if (!sucess)
	{
		perror("dup");
		return (1);
	}
	execve(cmd->vbin, cmd->toks, shell->env);
	print_error(1, NULL, cmd->toks[0], NOCMDFOUND);
	if (errno == 2)
		return (127);
	else if (errno == 13)
		return (127);
	return (1);
}

void	ft_execute_one_cmd(t_commands *cmd, t_shell *shell)
{
	int			pid;
	int			status;

	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		status = ft_exec_in_child_process(shell, cmd);
		ft_exit_shell(shell, status);
	}
	status = 0;
	waitpid(pid, &status, 0);
	shell->last_status = WEXITSTATUS(status);
}

void	run_commands(t_shell *shell)
{
	t_commands	*cur_cmd;

	cur_cmd = shell->cmd_head;
	if (g_ctrlc)
	{
		ft_printf_fd(1, "\n");
		set_status(shell, 130);
		return ;
	}
	if (shell->no_cmds == 1 && !cur_cmd->do_not_run)
	{
		if (is_it_builtin(shell->builtins, cur_cmd->toks[0]))
			when_one_builtin(shell, cur_cmd);
		else
			ft_execute_one_cmd(cur_cmd, shell);
	}
	else if (shell->no_cmds != 1)
		run_processes(shell->cmd_head, shell, shell->cmd_head->fds);
	if (g_ctrlc)
		ft_printf_fd(1, "\n");
}
