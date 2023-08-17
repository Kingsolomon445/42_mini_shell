/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:17:48 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/15 20:27:28 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_commands	*ft_lstnew_cmd(t_shell *shell, \
t_redir *redir, t_tok_pos *tok_pos, char *command)
{
	t_commands	*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->red = redir;
	cmd->tok_pos = tok_pos;
	cmd->command = command;
	cmd->vbin = NULL;
	cmd->next = NULL;
	cmd->toks = create_tokens(cmd);
	if (!cmd->toks)
		return (ft_free_cmds(&cmd), NULL);
	cmd->vbin = get_valid_bin(shell, cmd->toks[0]);
	cmd->cmd_pos = shell->no_cmds;
	cmd->do_not_run = shell->do_not_run;
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
		ft_free_red(&current->red);
		ft_free_tokenpos(&current->tok_pos);
		ft_free_split(current->toks);
		ft_free(current->command);
		ft_free(current->vbin);
		ft_free(current);
	}
}
