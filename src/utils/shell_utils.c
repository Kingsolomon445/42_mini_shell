/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:54:25 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/20 11:18:02 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**get_final_path()
{
	char	**dirs;
	char	*path;

	path = getenv("PATH");
	dirs = ft_split(path, ':');
	return (dirs);
}

void	is_here_doc(t_shell *shell, char *limiter)
{
	int		heredoc;
	char	*str;

	heredoc = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
	{
		free_for_next_read(shell);
		ft_printf_fd(2, "pipex: %s: %s\n", strerror(errno), "here_doc");
		exit (127);
	}
	write(1, "pipe heredoc> ", 14);
	str = get_next_line(STDIN_FILENO);
	ft_putstr_fd(str, heredoc);
	while (compare_cmd(str, limiter) != 1)
	{
		free(str);
		write(1, "pipe heredoc> ", 14);
		str = get_next_line(STDIN_FILENO);
		if (compare_cmd(str, limiter) != 1)
		{
			ft_putstr_fd(str, heredoc);
			ft_putstr_fd("\n", heredoc);
		}
	}
	free(str);
}

void	ft_exit_shell(t_shell *shell, int status)
{
	free_me_from_this_burden(shell);
	exit(status);
}
