/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:20:12 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/12 14:26:22 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_dup_red(t_redirection *red)
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

int	handle_redirections(t_commands *cmd)
{
	t_redirection	*current_red;

	current_red = cmd->red;
	while (current_red)
	{
		if (!ft_dup_red(current_red))
			return (0);
		current_red = current_red->next;
	}
	return (1);
}

void	ft_exec_in_child_process(t_commands *cmd)
{
	extern char	**environ;
	int			sucess;

	sucess = 1;
	if (cmd->red)
		sucess = handle_redirections(cmd);
	if (!sucess)
	{
		perror("dup");
		exit (1);
	}
	execve(cmd->vbin, cmd->toks, environ);
	print_error(1, NULL, cmd->toks[0], NOCMDFOUND);
	if (errno == 2)
		exit (127);
	else if (errno == 13)
		exit (127);
	else
		exit(1);
}

void	ft_execute_one_cmd(t_commands *cmd, t_shell *shell)
{
	int			pid;
	int			status;

	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
		ft_exec_in_child_process(cmd);
	status = 0;
	waitpid(pid, &status, 0);
	shell->last_status = WEXITSTATUS(status);
}

void	run_commands(t_shell *shell)
{
	t_commands	*cur_cmd;
	int			original_stdout;

	cur_cmd = shell->cmd_head;
	original_stdout = dup(STDOUT_FILENO);
	if (shell->no_cmds == 1)
	{
		if (is_it_builtin(shell->builtins, cur_cmd->toks[0]))
		{
			if (shell->no_cmds == 1)
				when_one_builtin(shell, cur_cmd);
		}
		else
			ft_execute_one_cmd(cur_cmd, shell);
	}
	else
		run_processes(shell->cmd_head, shell, shell->cmd_head->fds);
	close(STDOUT_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}
