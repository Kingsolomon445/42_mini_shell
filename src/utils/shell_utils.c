/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:54:25 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/07 17:53:47 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	checkleaks(void)
{
	system("leaks minishell");
}

char	**get_final_path()
{
	char	**dirs;
	char	*path;

	path = getenv("PATH");
	dirs = ft_split(path, ':');
	return (dirs);
}
void	print_history(t_shell *shell)
{
	int		index;
	t_list	*temp;

	index = 0;
	if (!shell->hist_head)
		return ;
	temp = shell->hist_head;
	while (temp)
	{
		printf("%5d	%s\n", index + 1, (char *)temp->content);
		temp = temp->next;
		index++;
	}
}

char	*get_valid_bin(t_shell *shell, char *cmd)
{
	int			i;
	char		*current_bin;
	char		*new_cmd;
	struct stat	st;

	i = -1;
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	while (shell->path[++i])
	{
		current_bin = ft_strjoin(shell->path[i], new_cmd);
		if (!current_bin)
			return (free(new_cmd), NULL);
		if (stat(current_bin, &st) == 0)
			return (free(new_cmd), current_bin);
		ft_free(current_bin);
	}
	current_bin = ft_strdup(cmd);
	return (ft_free(new_cmd), current_bin);
}

void    set_status(t_shell *shell, int status)
{
    shell->last_status = status;
}

int	run_here_doc(char *limiter)
{
	int		heredoc;
	char	*str;

	heredoc = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
	{
		ft_printf_fd(2, "pipex: %s: %s\n", strerror(errno), "here_doc");
		return (-1);
	}
	str = readline(">");
	while (str && compare_cmd(str, limiter) != 1)
	{
		ft_putstr_fd(str, heredoc);
		ft_putstr_fd("\n", heredoc);
		ft_free(str);
		str = readline(">");
	}
	ft_free(str);
	return (open("here_doc", O_RDONLY));
}

void	ft_exit_shell(t_shell *shell, long status)
{
	free_me_from_this_burden(shell);
	exit(status & 0xff);
}
