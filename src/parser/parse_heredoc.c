/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 17:56:35 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 21:48:47 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int p(void)
{
	return (1);
}
// void	int_handler(int sig_num)
// {
// 	(void) sig_num;
// 	rl_replace_line("\x03", 0);
// 	rl_done = 1;
// }

char	*parse_delimeter(t_shell *shell, char *delim, int *do_not_expand)
{
	char	*new_delim;
	int		i;
	int		j;
	int		size;

	i = 0;
	j = 0;
	size = 20;
	new_delim = malloc(size);
	while(delim[i])
	{
		if (ft_strchr("\"'", delim[i]))
		{
			*do_not_expand = 1;
			parse_quotes(shell, delim, &new_delim, &i, &j, &size, delim[i]);
		}
		else if (delim[i] == '$')
			parse_dollar(shell, &new_delim, delim, &i, &j, &size);
		else
		{
			if (delim[i] == '\\')
				i++;
			new_delim[j++] = delim[i++];
		}
		append_to_new_cmd(&new_delim, &size, &j, '\0');
	}
	ft_free(delim);
	return (new_delim);
}

char	*parse_heredoc_input(t_shell *shell, char *input, int do_not_expand)
{
	char	*new_input;
	int		i;
	int		j;
	int		size;

	i = 0;
	j = 0;
	size = 20;
	new_input = malloc(size);
	while(input[i])
	{
		if (input[i] == '$' && !do_not_expand)
			parse_dollar(shell, &new_input, input, &i, &j, &size);
		else
			new_input[j++] = input[i++];
		append_to_new_cmd(&new_input, &size, &j, '\0');
	}
	ft_free(input);
	return (new_input);
}

int	run_here_doc(t_shell *shell, char *delim)
{
	int		heredoc;
    int     do_not_expand;
	char	*inp;
    char    *parsed_inp;

    do_not_expand = 0;
	heredoc = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
	{
		ft_printf_fd(2, "pipex: %s: %s\n", strerror(errno), "here_doc");
		return (-1);
	}
	// signal(SIGINT, int_handler);
	// rl_event_hook = p;
    delim = parse_delimeter(shell, ft_strdup(delim), &do_not_expand);
	while (1)
	{
		// inp = readline("> ");
		if (isatty(fileno(stdin)))
			inp = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			inp = ft_strtrim(line, "\n");
			free(line);
		}
        parsed_inp = parse_heredoc_input(shell, inp, do_not_expand);
		if (!parsed_inp || compare_str(parsed_inp, delim) || compare_str(parsed_inp, "\x03"))
			break ;
		ft_putstr_fd(parsed_inp, heredoc);
		ft_putstr_fd("\n", heredoc);
	}
	close(heredoc);
	return (open("here_doc", O_RDONLY));
}