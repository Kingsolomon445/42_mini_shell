/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:54:25 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/18 17:22:05 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	checkleaks(void)
{
	system("leaks minishell");
}

void	set_terminal_attributes(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

char	**get_final_path(void)
{
	char	**dirs;
	char	*path;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	return (dirs);
}

int	print_history(t_shell *shell)
{
	int		index;
	t_list	*temp;

	index = 0;
	if (!shell->hist_head)
		return (shell->last_status);
	if (shell->cmd_head->toks[1] && compare_str("-c", shell->cmd_head->toks[1]))
	{
		ft_free_lst(&shell->hist_head);
		shell->hist_head = NULL;
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
	return (shell->last_status);
}

void	set_status(t_shell *shell, int status)
{
	shell->last_status = status;
}
