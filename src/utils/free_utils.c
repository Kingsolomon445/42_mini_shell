/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:49:41 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/20 11:15:45 by ofadahun         ###   ########.fr       */
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

void	free_split_alloc(char **str)
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

void	ft_free_red(t_redirection **headref)
{
	t_redirection	*current;

	if (!(*headref))
		return ;
	while (*headref)
	{
		current = *headref;
		*headref = (*headref)->next;
		ft_free(current);
	}
}

void	free_commands(t_commands **cmd_head)
{
	t_commands	*current;

	if (!(*cmd_head))
		return ;
	while (*cmd_head)
	{
		current = *cmd_head;
		*cmd_head = (*cmd_head)->next;
		free_split_alloc(current->toks);
		ft_free(current->vbin);
		ft_free(current->cmd_str);
		ft_free_red(&(current->red));
		ft_free(current);
	}
}

void	free_me_from_this_burden(t_shell *shell)
{
	free_split_alloc(shell->path);
	free_split_alloc(shell->env);
	free_split_alloc(shell->builtins);
	ft_free(shell->welcome_str);
	ft_free(shell->input);
	free_commands(&(shell->cmd_head));
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

