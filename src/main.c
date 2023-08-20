/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:13:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 18:35:05 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_ctrlc = 0;

static int	take_input(t_shell *shell)
{
	char	*content;
	t_list	*new;

	if (!shell->last_status)
		shell->welcome_str = shell->success_prompt;
	else
		shell->welcome_str = shell->failed_prompt;
	shell->input = get_prompt(shell->welcome_str);
	if (!shell->input)
		return (1);
	if (*shell->input && shell->input[0] != '#')
	{	
		content = ft_strdup(shell->input);
		if (!content)
			return (-1);
		new = ft_lstnew(content);
		if (!new)
			return (ft_free(content), -1);
		ft_lstadd_back(&(shell->hist_head), new);
		return (0);
	}
	return (ft_free(shell->input), -1);
}

static void	printf_welcome(void)
{
	ft_printf_fd(1, \
	"*********************************************************************\n");
	ft_printf_fd(1, "\t\t\t🌟Welcome to 42 MiniShell🌟\n");
	ft_printf_fd(1, "\t\t    Developed by: @ofadahun & @sbhatta\n");
	if (getenv("SHLVL"))
		ft_printf_fd(1, "\t\t\t💡Current Shell Level: %s\n", getenv("SHLVL"));
	if (getenv("USER"))
		ft_printf_fd(1, "\t\t\t 💁Current User: %s\n", getenv("USER"));
	else
		ft_printf_fd(1, "\t\t\t 💁Current User: %s\n", "guest");
	ft_printf_fd(1, \
	"*********************************************************************\n");
}

int	start_shell(t_shell *shell)
{
	int	status;

	g_ctrlc = 0;
	signal(SIGINT, sig_int_handler_before_exec);
	status = take_input(shell);
	if (status == 1)
		ft_exit_shell(shell, shell->last_status);
	else if (status == -1)
		start_shell(shell);
	add_history(shell->input);
	signal(SIGINT, sig_int_handler_after_exec);
	if (parse_shell(shell))
		run_commands(shell);
	ft_free_for_next_read(shell);
	init_shell_for_next_read(shell);
	if (status == 0)
		start_shell(shell);
	return (shell->last_status);
}

int	main(void)
{
	t_shell			*shell;
	int				status;

	status = 0;
	signal(SIGQUIT, SIG_IGN);
	set_terminal_attributes();
	shell = malloc(sizeof(t_shell));
	if (!shell)
		exit(12);
	shell->hist_head = NULL;
	if (!init_shell(shell))
		return (ft_free_shell(shell), status);
	printf_welcome();
	status = start_shell(shell);
	return (ft_free_shell(shell), status);
}
