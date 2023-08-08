/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:13:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/08 17:52:04 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// We need to handle an occasion whereby redirection/heredoc doesnt have a file or an argument. ===== done
// We still need a welcome string or welcome ascii art
// Need to handle echo $$
// Need to escape backslash
// Unset needs to handle multiple arguments  ===== done
// need to handle when pipe is inside quoted strings
//ls > tmp/file gives malloc error

static int	take_input(t_shell *shell)
{
	char	*content;
	t_list	*new;

	// shell->input = readline(shell->welcome_str);
	if (isatty(fileno(stdin)))
		shell->input = readline(shell->welcome_str);
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		shell->input = ft_strtrim(line, "\n");
		free(line);
	}
	if (!shell->input)
		ft_exit_shell(shell, EXIT_FAILURE);
	if (*shell->input)
	{	
		content = ft_strdup(shell->input);
		if (!content)
			ft_exit_shell(shell, EXIT_FAILURE);
		new = ft_lstnew(content);
		if (!new)
		{
			ft_free(content);
			ft_exit_shell(shell, EXIT_FAILURE);
		}
		if (!shell->hist_head)
			shell->hist_head = new;
		else
			ft_lstadd_back(&(shell->hist_head), new);
		return (0);
	}
	return (ft_free(shell->input), 1);
}

static int	init_shell(t_shell *shell)
{
	char	*new_level;
	int		level;

	shell->cmd_head = NULL;
	shell->hist_head = NULL;
	shell->input = NULL;
	shell->welcome_str = ft_strjoin(getenv("USER"), "@minishell$ ");
	shell->path = get_final_path();
	shell->builtins = \
	ft_split("echo cd pwd export unset env exit history", ' ');
	shell->last_status = 0;
	init_shellenv(shell);
	if (!getenv("SHLVL"))
	{
		level = 1;
		ft_putenv(shell, ft_strdup("SHLVL=1"));
	}
	else
		level = ft_atoi(getenv("SHLVL")) + 1;
	new_level = ft_itoa(level);
	return (update_env_item(shell, "SHLVL", new_level), ft_free(new_level), 1);
}

int	parse_commands(t_shell *shell)
{
	int			cmd_pos;
	char		**commands;
	t_commands	*new;

	cmd_pos = 0;
	commands = ft_split_commands(shell->input, '|', shell);
	if (!commands)
		return (0);
	shell->no_cmds = count_commands(commands);
	while (*(commands + cmd_pos))
	{
		new = ft_lstnew_cmd(*(commands + cmd_pos), shell, cmd_pos + 1);
		if (shell->red_status < 0)
		{
			shell->last_status = 258;
			if (shell->red_status == -1)
				shell->last_status = 127;
			return (ft_free(new), free_split_alloc(commands), 0);
		}
		if (!new)
			return (free_split_alloc(commands), 0);
		if (!shell->cmd_head)
			shell->cmd_head = new;
		else
			ft_lstadd_back_cmd(&(shell->cmd_head), new);
		cmd_pos++;
	}
	return (free_split_alloc(commands), 1);
}

int	main(void)
{
	t_shell			*shell;

	ignore_signal();
	set_signal_act();
	// atexit(&checkleaks);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (-1);
	shell->red_status = 1;
	init_shell(shell);
	shell->dollar_head = NULL;
	while (1)
	{
		if (take_input(shell))
			continue ;
		add_history(shell->input);
		parse_input(shell);
		if (parse_commands(shell))
		{
			run_commands(shell);
		}
		ft_free_dollar(&shell->dollar_head);
		shell->dollar_head = NULL;
		free_for_next_read(shell);
	}
	free_me_from_this_burden(shell);
	return (0);
}
