/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:41 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 19:50:53 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	open_fd(char *file_name, char red_type)
{
	int	fd;

	fd = -1;
	if (red_type == RED_IN)
		fd = open(file_name, O_RDONLY);
	else if (red_type == RED_OUT)
		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (red_type == RED_APPEND)
		fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

void	parse_redirection(t_redirection **redirection,  int *i, char *input)
{
	char	red_type;
	char	*file;
	int		j;
	int		fd;
	int		start;
	t_redirection *red;

	j = 0;
	if (*input == '>' && input[1] && input[1] == '>')
		red_type = RED_APPEND;
	else if (*input == '<' && input[1] && input[1] == '<')
		red_type = HEREDOC;
	else if (*input == '>')
		red_type = RED_OUT;
	else if (*input == '<')
		red_type = RED_IN;
	while(ft_strchr(">< \t\b\v\n", input[j]))
		j++;
	start = j;
	while(!ft_strchr(" \t\b\v\n", input[j]))
		j++;
	(*i) += j;
	file = ft_substr(input, start, j - start);
	// printf("input after this is: %s\n", input + j);
	// printf("file here == %s\n", file);
	fd = open_fd(file, red_type);
	red = ft_lstnew_red(red_type, fd);
	ft_lstadd_back_red(redirection, red);
	// printf("fd value here is %d\n", fd);
}