/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:53:18 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/15 20:03:51 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_tok_pos	*ft_lstnew_tokenpos(int index)
{
	t_tok_pos	*tok_pos;

	tok_pos = malloc(sizeof(t_tok_pos));
	if (!tok_pos)
		return (NULL);
	tok_pos->index = index;
	tok_pos->next = NULL;
	return (tok_pos);
}

void	ft_lstadd_back_tokenpos(t_tok_pos **tok_pos_head, \
t_tok_pos *new_tok_pos)
{
	t_tok_pos	*cur_tok_pos;

	if (!new_tok_pos)
		return ;
	if (!(*tok_pos_head))
	{
		*tok_pos_head = new_tok_pos;
		return ;
	}
	cur_tok_pos = *tok_pos_head;
	while (cur_tok_pos->next)
		cur_tok_pos = cur_tok_pos->next;
	cur_tok_pos->next = new_tok_pos;
}

void	ft_free_tokenpos(t_tok_pos **tok_pos)
{
	t_tok_pos	*current;

	if (!(*tok_pos))
		return ;
	while (*tok_pos)
	{
		current = *tok_pos;
		*tok_pos = (*tok_pos)->next;
		ft_free(current);
	}
}
