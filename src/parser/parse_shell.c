/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:38:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 20:27:53 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_shell(t_shell *shell)
{
	char	**commands;
	t_commands *cmd_head;
	t_commands *cmd;
	int		i;

	i = 0;
	cmd_head = NULL;
	commands = ft_split(shell->input, '|');
	shell->no_cmds = 0;
	while(commands[i])
	{
		shell->no_cmds++;
		cmd = parse_commands(shell, commands[i]);
		printf("my command here is %s\n", cmd->command);
		ft_lstadd_back_cmd(&cmd_head, cmd);
		i++;
	}
	shell->cmd_head = cmd_head;
}

