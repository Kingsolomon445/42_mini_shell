/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 20:13:23 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 21:18:36 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*make_commands(const char *input, int start, int end)
{
   char *command;

   command = ft_substr(input, start, end - start);
   return (command);
   
}

char **ft_split_input(const char *inp, int cmd_no, t_shell *shell)
{
	const char *input;
	int	cmd_index;
	t_cmd_pos *cmd_pos;
	char **commands;
	int	start;
	int	end;

    if (!inp)
        return (NULL);
    input = inp;
    cmd_index = 0;
    commands = (char **)malloc((cmd_no + 1) * sizeof(char *));
	cmd_pos = shell->cmd_pos_head;
	start = 0;
	while (cmd_pos)
	{
		end = cmd_pos->index;
		commands[cmd_index++] = make_commands(input, start, end);
		start = end + 1;
		cmd_pos = cmd_pos->next;
	}
    end = ft_strlen(input);
	if (end > start)
		commands[cmd_index++] = make_commands(input, start, end);
    commands[cmd_index] = NULL;
    return (commands);
}