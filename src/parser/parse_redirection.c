/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:41 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/15 16:14:14 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	set_redir_type(char *command, char *red_type, int *j)
{
	if (*command == '>' && command[1] && command[1] == '>')
	{
		*red_type = RED_APPEND;
		*j += 2;
	}
	else if (*command == '<' && command[1] && command[1] == '<')
	{
		*red_type = HEREDOC;
		(*j) += 2;
	}
	else if (*command == '>')
	{
		*red_type = RED_OUT;
		(*j)++;
	}
	else if (*command == '<')
	{
		*red_type = RED_IN;
		(*j)++;
	}
}

static void	find_delim_or_file_end(char *command, int *j)
{
	char	quote;

	while (command[*j] && !ft_strchr(" \t\b\v\n><|", command[*j]))
	{
		if (ft_strchr("\"'", command[*j]))
		{
			quote = command[(*j)++];
			ft_increment_index(command, &quote, j, 0);
		}
		if (command[*j])
			(*j)++;
	}
}

int	parse_redir(t_shell *shell, char *command, int *idx, t_redir **redir)
{
	char	red_type;
	int		i;
	int		j;
	int		start;

	j = 0;
	set_redir_type(command, &red_type, &j);
	ft_increment_index(command, shell->delims, &j, 1);
	start = j;
	find_delim_or_file_end(command, &j);
	i = j;
	ft_increment_index(command, shell->delims, &i, 1);
	if (*idx != 0 && command[i] && ft_strchr(" \t\b\v\n", command[i - 1]))
		i--;
	(*idx) += i;
	if (red_type != HEREDOC)
		return (parse_in_out_append_redir(shell, redir, \
		ft_substr(command, start, j - start), red_type));
	return (parse_heredoc(shell, redir, \
	ft_substr(command, start, j - start), red_type));
}
