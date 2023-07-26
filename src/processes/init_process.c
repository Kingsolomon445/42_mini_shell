/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:20:12 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/20 11:48:03 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		is_it_builtin(char **builtins, char *cmd)
{
	int	i;

	i = 0;
	while (*(builtins + i))
	{
		if (compare_cmd(*(builtins + i), cmd))
			return (1);
		i++;
	}
	return (0);
}


void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell)
{
	extern char	**environ;
	
	if (cmd->red)
		handle_redirections(cmd);
	if (compare_cmd("cd", cmd->toks[0]))
		change_directory(shell, cmd);
	else if (compare_cmd("export", cmd->toks[0]))
		export_env(shell);
	else if (compare_cmd("history", cmd->toks[0]) == 1)
		print_history(shell);
	else if (compare_cmd("history -c", shell->input) == 1)
	{
		ft_free_lst(&shell->hist_head);
		rl_clear_history();
	}
	else if (compare_cmd("pwd", cmd->toks[0]))
		get_pwd(shell);
	else if (compare_cmd("env", cmd->toks[0]))
		print_env(environ);
	else if (compare_cmd("unset", cmd->toks[0]))
		run_unset(shell, cmd->toks[1]);
	else if (compare_cmd("echo", cmd->toks[0]))
		echo_echo(cmd);
	else if (compare_cmd("exit", cmd->toks[0]))
		ft_exit_shell(shell, EXIT_SUCCESS);
	
}

int ft_dup_red(t_redirection *red)
{
    if (red->red_type == RED_IN)
	{
        if (dup2(red->red_fd, STDIN_FILENO) == -1)
            return 0;
    }
	else
	{
        if (dup2(red->red_fd, STDOUT_FILENO) == -1)
            return 0;
    }
    close(red->red_fd);
    return 1;
}


void	handle_redirections(t_commands *cmd)
{
	t_redirection *current_red;

	current_red = cmd->red;
	while (current_red)
	{
		ft_dup_red(current_red);
		current_red = current_red->next;
	} 
}

void	ft_exec_in_child_process(t_commands *cmd)
{
	extern char	**environ;

	if (cmd->red)
		handle_redirections(cmd);
	execve(cmd->vbin, cmd->toks, environ);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->toks[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	// free_for_next_read(shell);
	exit(127);
}

void	ft_execute_one_cmd(t_commands *cmd, t_shell *shell)
{
	int			pid;
	int			status;

	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		ft_exec_in_child_process(cmd);
	status = 0;
	waitpid(pid, &status, 0);
	shell->last_status = WEXITSTATUS(status);
}


void	run_commands(t_shell *shell)
{
	t_commands	*current_cmd;
	int			original_stdout;

	current_cmd = shell->cmd_head;
	original_stdout = dup(STDOUT_FILENO);
	set_terminal_settings();
	if (shell->no_cmds == 1)
	{
		if (is_it_builtin(shell->builtins, current_cmd->toks[0]))
			ft_execute_one_builtin(current_cmd, shell);
		else
			ft_execute_one_cmd(current_cmd, shell);
	}
	else
		run_processes(shell->cmd_head, shell, shell->cmd_head->fds);
	reset_terminal_settings();
	close(STDOUT_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}