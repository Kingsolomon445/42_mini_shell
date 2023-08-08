/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:20:12 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/06 16:37:29 by sbhatta          ###   ########.fr       */
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

void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell)
{
	extern char	**environ;

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
	else if (compare_builtin("pwd", cmd->toks[0]))
		get_pwd(shell);
	else if (compare_builtin("env", cmd->toks[0]))
	{
		if (cmd->toks[1])
		{
			printf("env: %s: No such file or directory\n", cmd->toks[1]);
			shell->last_status = 127;
		}
		else
			print_env(environ);
	}
	else if (compare_cmd("unset", cmd->toks[0]))
		run_unset(shell, cmd->toks[1]);
	else if (compare_builtin("echo", cmd->toks[0]))
		echo_echo(cmd);
	else if (compare_cmd("exit", cmd->toks[0]))
	{
		if (cmd->toks[1] && ft_isnumber(cmd->toks[1]) && cmd->toks[2])
		{
			ft_printf_fd(1, "exit\n");
			ft_printf_fd(2, "minishell: exit: too many arguments\n");
			shell->last_status = 1;
		}
		else if (cmd->toks[1])
		{
			printf("exit\n");
			if (!ft_isnumber(cmd->toks[1]))
			{
				ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", cmd->toks[1]);
				ft_exit_shell(shell, 255);
			}
			else
			{
				if (*(cmd->toks[1]) == '-')
				{
					if (ft_atoi_ulong(cmd->toks[1] + 1) > 9223372036854775808UL)
					{
						ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", cmd->toks[1]);
						ft_exit_shell(shell, 255);
					}
					else
						ft_exit_shell(shell, ft_atoi_ulong(cmd->toks[1]));
				}
				else if (ft_atoi_ulong(cmd->toks[1]) > 9223372036854775807)
				{
					ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", cmd->toks[1]);
					ft_exit_shell(shell, 255);
				}
				else
					ft_exit_shell(shell, ft_atoi_ulong(cmd->toks[1]));
			}
		}
		else
		{
			printf("exit\n");
			ft_exit_shell(shell, EXIT_SUCCESS);
		}
	}
	else
	{
		if (!compare_cmd("CD", cmd->toks[0]))
			ft_printf_fd(2, "minishell: %s: command not found\n", cmd->toks[0]);
		shell->last_status = 127;
	}
}


int ft_dup_red(t_redirection *red)
{
    if (red->red_type == RED_IN || red->red_type == HEREDOC)
	{
        if (dup2(red->red_fd, STDIN_FILENO) == -1)
            return 0;
    }
	else if (red->red_type == RED_OUT || red->red_type == RED_APPEND)
	{
        if (dup2(red->red_fd, STDOUT_FILENO) == -1)
            return 0;
    }
    close(red->red_fd);
    return 1;
}


int	handle_redirections(t_commands *cmd)
{
	t_redirection *current_red;

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
		// perror("dup");
		exit (errno);
	}
	execve(cmd->vbin, cmd->toks, environ);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->toks[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	// ft_free_for_next_read(shell);
	exit(errno);
}

void	ft_execute_one_cmd(t_commands *cmd, t_shell *shell)
{
	int			pid;
	int			status;

	pid = fork();
	if (pid < 0)
		exit(errno);
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
	// set_terminal_settings();
	if (shell->no_cmds == 1)
	{
		if (is_it_builtin(shell->builtins, cur_cmd->toks[0]))
			ft_execute_one_builtin(cur_cmd, shell);
		else
			ft_execute_one_cmd(cur_cmd, shell);
	}
	else
		run_processes(shell->cmd_head, shell, shell->cmd_head->fds);
	// reset_terminal_settings();
	close(STDOUT_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdout);
}
