/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_one.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:17:48 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/07 17:29:01 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redirection	*ft_lstnew_red(int red_type, int fd)
{
	t_redirection	*new_red;

	new_red = malloc(sizeof(t_redirection));
	if (!new_red)
		return (NULL);
	new_red->red_type = red_type;
	new_red->red_fd = fd;
	new_red->next = NULL;
	return (new_red);
}

void	ft_lstadd_back_red(t_redirection **red_head, t_redirection *new_red)
{
	t_redirection	*current_red;

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