/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:26:11 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/17 17:21:34 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd_pos	*ft_lstnew_cmdpos(int index)
{
	t_cmd_pos	*cmd_pos;

	cmd_pos = malloc(sizeof(t_tok_pos));
	if (!cmd_pos)
		return (NULL);
	cmd_pos->index = index;
	cmd_pos->next = NULL;
	return (cmd_pos);
}

void	ft_lstadd_back_cmdpos(t_cmd_pos **cmd_pos_head, t_cmd_pos *new_cmd_pos)
{
	t_cmd_pos	*cur_cmd_pos;

	if (!new_cmd_pos)
		return ;
	if (!(*cmd_pos_head))
	{
		*cmd_pos_head = new_cmd_pos;
		return ;
	}
	cur_cmd_pos = *cmd_pos_head;
	while (cur_cmd_pos->next)
		cur_cmd_pos = cur_cmd_pos->next;
	cur_cmd_pos->next = new_cmd_pos;
}

void	ft_free_cmdpos(t_cmd_pos **cmd_pos)
{
	t_cmd_pos	*current;

	if (!(*cmd_pos))
		return ;
	while (*cmd_pos)
	{
		current = *cmd_pos;
		*cmd_pos = (*cmd_pos)->next;
		ft_free(current);
	}
}
