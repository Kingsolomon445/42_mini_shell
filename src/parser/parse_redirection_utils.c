/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:50:03 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 19:50:05 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	open_fd(char *file_name, char red_type)
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

static char	*parse_file_dir_helper(t_shell *shell, char *file_name)
{
	char	*new_file;

	new_file = ft_strdup(file_name);
	if (!new_file)
		return (set_status(shell, 12), NULL);
	if (new_file[1] && new_file[0] == \
	new_file[1] && ft_strchr("\"'", new_file[0]))
	{
		shell->do_not_run = 1;
		return (print_error(1, NULL, "", NOFILEDIR), \
		ft_free(new_file), set_status(shell, 1), NULL);
	}
	return (new_file);
}

static int	parse_file_dir(t_shell *shell, char *file_name)
{
	char	*dir;
	char	*new_file;
	DIR		*temp_dir;

	temp_dir = opendir(file_name);
	if (temp_dir)
	{
		shell->do_not_run = 1;
		return (closedir(temp_dir), set_status(shell, 1), \
		print_error(1, NULL, file_name, "is a directory"), 0);
	}
	new_file = parse_file_dir_helper(shell, file_name);
	if (!new_file)
		return (0);
	if (!ft_strrchr(new_file, '/'))
		return (ft_free(new_file), 1);
	dir = ft_substr(new_file, 0, ft_strrchr(new_file, '/') - new_file + 1);
	if (!dir)
		return (ft_free(new_file), set_status(shell, 12), 0);
	temp_dir = opendir(dir);
	if (temp_dir)
		return (closedir(temp_dir), ft_free(new_file), ft_free(dir), 1);
	print_error(1, NULL, file_name, NOFILEDIR);
	shell->do_not_run = 1;
	return (ft_free(new_file), ft_free(dir), set_status(shell, 1), 0);
}

static void	close_r(t_shell *shell, t_redir **redir, char *file, int do_not_run)
{
	close_all_fds(redir);
	ft_free_red(redir);
	ft_free(file);
	if (do_not_run != -1)
		shell->do_not_run = 1;
}

int	parse_in_out_append_redir(t_shell *shell, t_redir **redir, \
char *file, char red_type)
{
	int			fd;
	t_redir		*red;

	file = parse_file_or_delimeter(shell, file, 1);
	if (!file)
		return (0);
	if (compare_str("", file))
		return (set_status(shell, 258), ft_printf_fd(2, SYNTAXERR), \
		close_r(shell, redir, file, 1), 0);
	if (!parse_file_dir(shell, file))
		return (close_all_fds(redir), ft_free(file), ft_free_red(redir), 0);
	if (red_type == RED_IN && access(file, F_OK) == -1)
		return (print_error(1, NULL, file, strerror(errno)), \
		close_r(shell, redir, file, 1), set_status(shell, 1), 0);
	fd = open_fd(file, red_type);
	if (fd == -1)
		return (close_r(shell, redir, file, -1), set_status(shell, 1), 0);
	red = ft_lstnew_red(red_type, fd);
	if (!red)
		return (close(fd), close_r(shell, redir, file, -1), 0);
	return (ft_free(file), ft_lstadd_back_red(redir, red), 1);
}
