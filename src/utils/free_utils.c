/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:49:41 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 19:57:25 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_lst(t_list **headref)
{
	t_list	*current;

	if (!(*headref))
		return ;
	while (*headref)
	{
		current = *headref;
		*headref = (*headref)->next;
		ft_free(current->content);
		ft_free(current);
	}
}

void	ft_free_split(char **str)
{
	int		i;

	i = 0;
	if (str)
	{
		while (str[i])
		{
			free(str[i]);
			str[i] = NULL;
			i++;
		}
		free(str);
		str = NULL;
	}
}



void	ft_free_shell(t_shell *shell)
{
	ft_free_split(shell->path);
	ft_free_split(shell->env);
	ft_free_split(shell->builtins);
	ft_free(shell->welcome_str);
	ft_free(shell->input);
	ft_free_cmds(&(shell->cmd_head));
	ft_free_lst(&(shell->hist_head));
	ft_free(shell);
}

void	ft_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	ft_free_for_next_read(t_shell *shell)
{
	ft_free(shell->input);
	shell->input = NULL;
	ft_free_cmds(&shell->cmd_head);
	shell->cmd_head = NULL;
}

