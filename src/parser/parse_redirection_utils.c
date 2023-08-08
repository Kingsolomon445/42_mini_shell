/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:19:53 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/08 13:05:14 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	search_if_dir(char *file_name)
{
	char	*dir;
	char	*new_file;

	new_file = ft_strdup(file_name);
	dir = ft_strrchr(new_file, '/');
	if (!dir)
		return (ft_free(new_file), 1);
	dir = ft_substr(new_file, 0, dir - new_file + 1);
	// printf("dir here is %s\n", dir);
	if (opendir(dir))
		return (1);
	ft_printf_fd(2, "minishell: %s: No such file or directory\n", file_name);
	return (ft_free(file_name), ft_free(new_file), ft_free(dir), 0);
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

int	extract_file(t_shell *shell, char *cmd_tmp, char red_type, int *fname_len)
{
	char	*file_name;
	int		fd;

	*fname_len = 0;
	while (cmd_tmp[*fname_len] && !ft_strchr(" ><\t\b\r\v", cmd_tmp[*fname_len]))
		(*fname_len)++;
	file_name = ft_substr(cmd_tmp, 0, *fname_len);
	if (!*file_name)
	{
		shell->sucess = -2;
		ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n");
		return (ft_free(file_name), 0);

	}
	if (!file_name)
	{
		shell->sucess = 0;
		return (ft_free(file_name), 0);
	}
	if (!search_if_dir(file_name))
	{
		shell->sucess = -1;
		return (0);
	}
	fd = open_fd(file_name, red_type);
	if (fd == -1)
	{
		ft_printf_fd(2, "minishell: %s: No such file or directory\n", file_name);
		shell->sucess = 0;
		return (ft_free(file_name), 0);
	}
	return (ft_free(file_name), fd);
}

int	extract_delimiter(t_shell *shell, char *cmd_tmp, int *delim_len)
{
	char	*delimeter;
	int		fd;

	*delim_len = 0;
	while (cmd_tmp[*delim_len] && !ft_strchr(" ><\t\b\r\v", cmd_tmp[*delim_len]))
		(*delim_len)++;
	delimeter = ft_substr(cmd_tmp, 0, *delim_len);
	if (!*delimeter)
	{
		shell->sucess = -2;
		ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n");
		return (ft_free(delimeter), 0);
	}
	if (!delimeter)
	{
		shell->sucess = 0;
		return (ft_free(delimeter), 0);
	}
	fd = run_here_doc(delimeter);
	if (fd == -1)
	{
		shell->sucess = 0;
		return (ft_free(delimeter), 0);
	}
	return (ft_free(delimeter), fd);
}