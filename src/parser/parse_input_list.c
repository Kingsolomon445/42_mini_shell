/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:26:11 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/08 17:51:22 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_dollar *ft_lstnew_dollar(int index, char *value)
{
	t_dollar *dollar;

	dollar = malloc(sizeof(t_dollar));
	if (!dollar)
		return (NULL);
	dollar->value = value;
	dollar->index = index;
	dollar->next = NULL;
	return (dollar);
}

void ft_lstadd_back_dollar(t_dollar **dollar_head, t_dollar *new_dollar)
{
	t_dollar	*current_dollar;

	if (!new_dollar)
		return ;
	if (!(*dollar_head))
	{
		*dollar_head = new_dollar;
		return ;
	}
	current_dollar = *dollar_head;
	while (current_dollar->next)
		current_dollar = current_dollar->next;
	current_dollar->next = new_dollar;
}

void	ft_free_dollar(t_dollar **headref)
{
	t_dollar	*current;

	if (!(*headref))
		return ;
	while (*headref)
	{
		current = *headref;
		*headref = (*headref)->next;
		ft_free(current->value);
		ft_free(current);
	}
}