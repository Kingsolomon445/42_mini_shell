/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:44:24 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/18 12:24:12 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_signal_act(void)
{
	struct sigaction	act;
	sigset_t			mask;

	sigemptyset(&mask);
	act.sa_flags = SA_RESTART;
	act.sa_mask = mask;
	act.sa_handler = sig_handler;
	sigaction(SIGINT, &act, NULL);
}

void	ignore_signal(void)
{
	struct sigaction	act;
	sigset_t			block_mask;

	sigemptyset(&block_mask);
	act.sa_handler = SIG_IGN;
	act.sa_mask = block_mask;
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
}

void	sig_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		ft_printf_fd(1, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
