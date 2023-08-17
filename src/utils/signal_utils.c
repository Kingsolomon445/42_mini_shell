#include "../../include/minishell.h"

void	heredoc_sig_int_handler(int sig_num)
{
	(void) sig_num;
    g_ctrlc = 1;
    ft_printf_fd(STDOUT_FILENO, "\n");
    rl_on_new_line();
    rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_int_handler_before_exec(int sig_num)
{
	ft_printf_fd(STDOUT_FILENO, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)sig_num;
}

void	sig_int_handler_after_exec(int sig_num)
{
	g_ctrlc = 1;
	(void)sig_num;
}