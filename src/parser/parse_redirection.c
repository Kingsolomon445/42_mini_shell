/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:41 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/12 19:27:58 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parse_file_dir(t_shell *shell, char *file_name)
{
	char    *dir;
	char    *new_file;

	new_file = ft_strdup(file_name);
	if (new_file[1] && new_file[0] == new_file[1] && ft_strchr("\"'", new_file[0]))
	{
		shell->do_not_run = 1;
		return(print_error(1, NULL, "", NOFILEDIR), ft_free(new_file), set_status(shell, 1), 0); 
	}
	dir = ft_strrchr(new_file, '/');
	if (!dir)
		return (ft_free(new_file), set_status(shell, 12), 1);
	dir = ft_substr(new_file, 0, dir - new_file + 1);
	if (opendir(dir))
		return (1);
	print_error(1, NULL, file_name, NOFILEDIR);
	shell->do_not_run = 1;
	return (ft_free(new_file), ft_free(dir),  set_status(shell, 1), 0);
}

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

int	parse_redirection(t_shell *shell, t_parse_commands_vars *vars,  char *command)
{
	char	red_type;
	char	*file;
	char	*delimeter;
	int		i;
	int		j;
	int		fd;
	int		start;
	t_redirection *red;

	j = 0;
	if (*command == '>' && command[1] && command[1] == '>')
	{
		red_type = RED_APPEND;
		j += 2;
	}
	else if (*command == '<' && command[1] && command[1] == '<')
	{
		red_type = HEREDOC;
		j += 2;
	}
	else if (*command == '>')
	{
		red_type = RED_OUT;
		j++;
	}
	else if (*command == '<')
	{
		red_type = RED_IN;
		j++;
	}
	while(command[j] && ft_strchr(" \t\b\v\n", command[j]))
		j++;
	start = j;
	while(command[j] && !ft_strchr(" \t\b\v\n><|", command[j]))
		j++;
	i = j;
	while(command[i] && ft_strchr(" \t\b\v\n", command[i]))
		i++;
	vars->i += i;
	if (red_type != HEREDOC)
	{
		file = ft_substr(command, start, j - start);
		if (!file)
			return (0);
		if (compare_str("", file))
		{
			shell->do_not_run = 1;
			return (set_status(shell, 258), ft_printf_fd(2, SYNTAXERR), 0);
		}
		if (!parse_file_dir(shell, file))
			return (ft_free(file), 0);
		fd = open_fd(file, red_type);
		if (fd == -1)
		{
			shell->do_not_run = 1;
			print_error(1, NULL, file, NOFILEDIR);
			return (ft_free(file),  set_status(shell, 1),  0);
		}
		red = ft_lstnew_red(red_type, fd);
		if (!red)
			return (ft_free(file), 0);
		ft_lstadd_back_red(&vars->redirection, red);
		return (ft_free(file), 1);
	}
	delimeter = ft_substr(command, start, j - start);
	if (!delimeter)
		return (0);
	if (compare_str("", delimeter))
	{
		shell->do_not_run = 1;
		return (set_status(shell, 258), ft_printf_fd(2, SYNTAXERR), 0);
	}
	fd = run_here_doc(shell, delimeter);
	if (fd == -1)
		return (ft_free(delimeter), 0);
	red = ft_lstnew_red(red_type, fd);
	if (!red)
		return (ft_free(delimeter), 0);
	ft_lstadd_back_red(&vars->redirection, red);
	return (ft_free(delimeter), set_status(shell, 0), 1);
}