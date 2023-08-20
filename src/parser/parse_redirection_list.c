/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_list.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:17:48 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/18 12:09:28 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*ft_lstnew_red(int red_type, int fd)
{
	t_redir	*new_red;

	new_red = malloc(sizeof(t_redir));
	if (!new_red)
		return (NULL);
	new_red->red_type = red_type;
	new_red->red_fd = fd;
	new_red->next = NULL;
	return (new_red);
}

void	ft_lstadd_back_red(t_redir **red_head, t_redir *new_red)
{
	t_redir	*current_red;

	if (!new_red)
		return ;
	if (!(*red_head))
	{
		*red_head = new_red;
		return ;
	}
	current_red = *red_head;
	while (current_red->next)
		current_red = current_red->next;
	current_red->next = new_red;
}

void	ft_free_red(t_redir **headref)
{
	t_redir	*current;

	if (!(*headref))
		return ;
	while (*headref)
	{
		current = *headref;
		close(current->red_fd);
		*headref = (*headref)->next;
		ft_free(current);
	}
}
