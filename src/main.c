/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:13:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/20 18:42:47 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	checkleaks(void)
{
	system("leaks minishell");
}

void	init_shell(t_shell *shell)
{
	shell->cmd_head = NULL;
	shell->hist_head = NULL;
	shell->input = NULL;
	shell->welcome_str = ft_strjoin(getenv("USER"), "@minishell% ");
	shell->path = get_final_path();
	shell->builtins = ft_split("echo cd pwd export unset env exit history", ' ');
	shell->last_status = 0;
}

void	free_for_next_read(t_shell *shell)
{
	ft_free(shell->input);
	shell->input = NULL;
	free_commands(&(shell->cmd_head));
	shell->cmd_head = NULL;
}

int	main(void)
{
	t_shell			*shell;

	ignore_signal();
	set_signal_act();
	atexit(&checkleaks);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (-1);
	init_shell(shell);
	init_shellenv(shell);
	while (1)
	{
		if (take_input(shell))
			continue ;
		add_history(shell->input);
		parse_commands(shell);
		if (!search_dollar_in_tok(shell))
			ft_exit_shell(shell, EXIT_FAILURE);
		run_commands(shell);
		free_for_next_read(shell);
	}
	free_me_from_this_burden(shell);
	return (0);
}
