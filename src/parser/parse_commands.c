/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 20:25:46 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/14 12:59:20 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    parse_redirection(t_commands *cmd, char *command)
{
    int i;

    i = 0;
    while (*(command + i))
    {
        if (ft_strchr("><", *(command + i)))
        {
            cmd->cmd_str = ft_substr(command, 0, i);
			if (!cmd->cmd_str)
				return (0);
            cmd->red_str = ft_substr(command, i, ft_strlen(command) - i + 1);
			if (!cmd->red_str)
				return (0);
            return (1);
        }
        i++;
    }
    cmd->cmd_str = ft_strdup(command);
    cmd->red_str = NULL;
	if (!cmd->cmd_str)
		return (0);
	return (1);
}

char	*get_valid_bin(t_shell *shell, char *cmd)
{
	int			i;
	char		*current_bin;
	char		*new_cmd;
	struct stat	st;

	i = -1;
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	while (shell->path[++i])
	{
		current_bin = ft_strjoin(shell->path[i], new_cmd);
		if (!current_bin)
			return (free(new_cmd), NULL);
		if (stat(current_bin, &st) == 0)
			return (free(new_cmd), current_bin);
		ft_free(current_bin);
	}
	current_bin = ft_strdup(cmd);
	return (ft_free(new_cmd), current_bin);
}

t_commands	*ft_lstnew_cmd(char *command, t_shell *shell, int cmd_pos)
{
	t_commands	*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	if (!parse_redirection(cmd, command))
		return (NULL);
	cmd->toks = ft_strtok(cmd->cmd_str, " \n\r\f\v\t");
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
	t_commands	*current_cmd;

	if (!new_cmd)
		return ;
	if (!(*cmd_head))
	{
		*cmd_head = new_cmd;
		return ;
	}
	current_cmd = *cmd_head;
	while (current_cmd->next)
		current_cmd = current_cmd->next;
	current_cmd->next = new_cmd;
}

void	parse_commands(t_shell *shell)
{
	int		i;
	int		cmd_pos;
	t_commands	*new;
	char	**commands;

	i = 0;
	shell->no_cmds = count_ocurrence(shell->input, '|') + 1;
	cmd_pos = 0;
	commands = ft_split(shell->input, '|');
	if (!commands)
		ft_exit_shell(shell, EXIT_FAILURE);
	while (*(commands + i))
	{
		cmd_pos++;
		new = ft_lstnew_cmd(*(commands + i), shell, cmd_pos);
		if (!new)
			break;
		if (!shell->cmd_head)
			shell->cmd_head = new;
		else
			ft_lstadd_back_cmd(&(shell->cmd_head), new);
		i++;
	}
	free_split_alloc(commands);
	if (!new)
		ft_exit_shell(shell, EXIT_FAILURE);
}
