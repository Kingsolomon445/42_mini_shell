/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 15:32:18 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/20 10:45:31 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/libft.h"

void	ft_lstfree(t_list **list)
{
	t_list	*temp;

	temp = *list;
	if (!*list)
		return ;
	while (*list)
	{
		*list = (*list)->next;
		free(temp);
		temp = *list;
	}
}

