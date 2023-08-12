/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:26:11 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/12 16:44:53 by ofadahun         ###   ########.fr       */
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

t_cmd_pos  *ft_lstnew_cmdpos(int index)
{
	t_cmd_pos *cmd_pos;

	cmd_pos = malloc(sizeof(t_token_pos));
	cmd_pos->index = index;
	cmd_pos->next = NULL;
	return (cmd_pos);
}

void ft_lstadd_back_cmdpos(t_cmd_pos **cmd_pos_head, t_cmd_pos *new_cmd_pos)
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