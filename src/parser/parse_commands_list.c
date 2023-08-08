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

t_commands	*ft_lstnew_cmd(char *command, t_shell *shell, int cmd_pos)
{
	t_commands	*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	cmd->red = NULL;
	shell->red_status = parse_redirection(shell, cmd, command);
	if (shell->red_status != 1)
		return (NULL);
	if (!cmd->cmd_str)
		return (NULL);
	cmd->toks = create_tokens(cmd->cmd_str, " \n\r\f\v\t");
	if (!cmd->toks)
		return (NULL);
	if (!cmd->toks)
		return (NULL);
	cmd->vbin = get_valid_bin(shell, cmd->toks[0]);
	if (!cmd->vbin)
		return (NULL);
	cmd->cmd_pos = cmd_pos;
	return (cmd);
}

void	ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd)
{
	t_commands	*cur_cmd;

	if (!new_cmd)
		return ;
	if (!(*cmd_head))
	{
		*cmd_head = new_cmd;
		return ;
	}
	cur_cmd = *cmd_head;
	while (cur_cmd->next)
		cur_cmd = cur_cmd->next;
	cur_cmd->next = new_cmd;
}

void	ft_free_cmds(t_commands **cmd_head)
{
	t_commands	*current;

	if (!(*cmd_head))
		return ;
	while (*cmd_head)
	{
		current = *cmd_head;
		*cmd_head = (*cmd_head)->next;
		ft_free_split(current->toks);
		ft_free(current->vbin);
		ft_free(current->cmd_str);
		ft_free_red(&(current->red));
		ft_free(current);
	}
}