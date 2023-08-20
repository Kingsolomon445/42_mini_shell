/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 17:56:35 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/18 18:27:10 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_prompt(char *prompt)
{
	char	*inp;

	inp = readline(prompt);
	return (inp);
}

int	run_here_doc(t_shell *shell, char *delim)
{
	int		heredoc;
	int		status;
	char	*new_delim;

	heredoc = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
		return (print_error(shell->last_status, \
		NULL, strerror(errno), "here_doc"), -1);
	shell->heredoc_ran = 1;
	new_delim = parse_file_or_delimeter(shell, ft_strdup(delim), 0);
	if (!new_delim)
		return (close(heredoc), -1);
	status = get_next_heredoc_input(shell, new_delim, heredoc);
	if (status == -1)
		return (close(heredoc), ft_free(new_delim), status);
	return (close(heredoc), ft_free(new_delim), open(".here_doc", O_RDONLY));
}

int	parse_heredoc(t_shell *shell, t_redir **redir, \
char	*delimeter, char red_type)
{
	int			fd;
	t_redir		*red;

	if (!delimeter)
		return (0);
	if (compare_str("", delimeter))
	{
		shell->do_not_run = 1;
		return (ft_free(delimeter), \
		set_status(shell, 258), ft_printf_fd(2, SYNTAXERR), 0);
	}
	fd = run_here_doc(shell, delimeter);
	if (fd == -1)
		return (ft_free(delimeter), 0);
	red = ft_lstnew_red(red_type, fd);
	if (!red)
		return (close(fd), ft_free(delimeter), 0);
	ft_lstadd_back_red(redir, red);
	return (ft_free(delimeter), set_status(shell, 0), 1);
}
