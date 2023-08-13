/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:54:25 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 16:19:18 by ofadahun         ###   ########.fr       */
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
	if (!path)
		return (NULL);
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
	if (shell->cmd_head->toks[1] && compare_str("-c", shell->cmd_head->toks[1]))
	{
		ft_free_lst(&shell->hist_head);
		rl_clear_history();
		shell->last_status = 0;
	}
	else
	{
		temp = shell->hist_head;
		while (temp)
		{
			printf("%5d	%s\n", index + 1, (char *)temp->content);
			temp = temp->next;
			index++;
		}
		shell->last_status = 0;
	}
}

int	parse_dir(t_shell *shell, char *current_bin)
{
	struct stat	st;

	shell->do_not_run = 1;
	if (opendir(current_bin))
		return (set_status(shell, 126), print_error(1, NULL, current_bin, "is a directory"), 1);
	if (stat(current_bin, &st) == 0)
	{
		shell->do_not_run = 0;
		return (1);
	}
	print_error(1, NULL, current_bin, NOFILEDIR);
	return (set_status(shell, 127), 0);
}

char	*get_valid_bin(t_shell *shell, char *cmd)
{
	int			i;
	char		*current_bin;
	char		*new_cmd;
	struct stat	st;

	i = -1;
	if (!shell->path)
		return (ft_strdup(cmd));
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	while (shell->path[++i] && !(ft_strchr(cmd, '/')))
	{
		current_bin = ft_strjoin(shell->path[i], new_cmd);
		if (!current_bin)
			return (free(new_cmd), NULL);
		if (stat(current_bin, &st) == 0)
			return (free(new_cmd), current_bin);
		ft_free(current_bin);
	}
	current_bin = ft_strdup(cmd);
	if (ft_strchr(current_bin, '/'))
		parse_dir(shell, current_bin);
	return (ft_free(new_cmd), current_bin);
}

void    set_status(t_shell *shell, int status)
{
    shell->last_status = status;
}

int	ft_putenv(t_shell *shell, char *new_env)
{
	extern char		**environ;
	int				array_len;
	char			**result;
	int				i;

	i = 0;
	array_len = env_len(shell->env);
	result = (char **)malloc((array_len + 2) * sizeof(char *));
	if (!result)
		return (0);
	while (shell->env[i])
	{
		result[i] = shell->env[i];
		i++;
	}
	result[i] = new_env;
	result[++i] = NULL;
	ft_free(environ);
	environ = result;
	shell->env = environ;
	return (1);
}

void	ft_exit_shell(t_shell *shell, long status)
{
	ft_free_shell(shell);
	exit(status & 0xff);
}
