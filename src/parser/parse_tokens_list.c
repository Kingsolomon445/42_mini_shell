/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:53:18 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 20:24:44 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token_pos  *ft_lstnew_tokenpos(int index)
{
	t_token_pos *token_pos;

	token_pos = malloc(sizeof(t_token_pos));
	token_pos->index = index;
	token_pos->next = NULL;
	return (token_pos);
}

void ft_lstadd_back_tokenpos(t_token_pos **token_pos_head, t_token_pos *new_token_pos)
{
	t_token_pos	*cur_token_pos;

	if (!new_token_pos)
		return ;
	if (!(*token_pos_head))
	{
		*token_pos_head = new_token_pos;
		return ;
	}
	cur_token_pos = *token_pos_head;
	while (cur_token_pos->next)
		cur_token_pos = cur_token_pos->next;
	cur_token_pos->next = new_token_pos;
}

void	ft_free_tokenpos(t_token_pos **token_pos)
{
	t_token_pos	*current;

	if (!(*token_pos))
		return ;
	while (*token_pos)
	{
		current = *token_pos;
		*token_pos = (*token_pos)->next;
		ft_free(current);
	}
}