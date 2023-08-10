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

static int memory_allocation(char **res, const char *start, int letter_count, int word_index)
{
    res[word_index] = (char *)malloc((letter_count + 1) * sizeof(char));
    if (!res[word_index])
	{
		ft_free_split(res);
        return 0;
    }
    ft_strlcpy(res[word_index], start, letter_count + 1);
    return 1;
}

char **ft_split_commands(const char *s, char c, int cmd_no)
{
	const char *str;
	int	word_index;
	int	letter_count;
	const char *start;
	int	in_quotes;
	char **res;

    if (!s)
        return NULL;
    str = s;
    word_index = 0;
    res = (char **)malloc((cmd_no + 1) * sizeof(char *));
    if (!res)
        return NULL;
    while (*str)
	{
       	letter_count = 0;
        start = str;
        in_quotes = 0;
        while (*str && (*str != c || in_quotes))
		{
            if (ft_strchr("\"'", *str))
                in_quotes = !in_quotes;
            letter_count++;
            str++;
        }

        if (letter_count > 0)
		{
            if (!memory_allocation(res, start, letter_count, word_index))
                return NULL;
            word_index++;
        }
        if (*str)
            str++;
    }
    res[word_index] = NULL;
    return res;
}


void	split_and_validate(t_shell *shell)
{
	int in_quotes;
	int	pipe_count;
	int	valid;
	int	i;
	char	*input;

	in_quotes = 0;
	pipe_count = 0;
	i = 0;
	input = ft_strdup(shell->input);
	valid = 1;
	while(input[i])
	{
		if (ft_strchr("\"'", input[i]))
			in_quotes = !in_quotes;
		else if (input[i] == '|' && !in_quotes)
		{
			pipe_count++;
			if (i == 0 || input[i + 1] == '\0' || input[i - 1] == '|')
			{
				valid = 0;
				break;
			}
		}
		i++;
	}
	if (in_quotes || input[strlen(input) - 1] == '|')
		valid = 0;
	if (!valid)
	{
		ft_printf_fd(2, "syntax error\n");
		return ;
	}
	shell->commands = ft_split_commands(input, '|', pipe_count + 1);
}

void	parse_shell(t_shell *shell)
{
	char	**commands;
	t_commands *cmd_head;
	t_commands *cmd;
	int		i;

	i = 0;
	cmd_head = NULL;
	split_and_validate(shell);
	commands = shell->commands;
	shell->no_cmds = 0;
	while(commands[i])
	{
		shell->no_cmds++;
		cmd = parse_commands(shell, commands[i]);
		ft_lstadd_back_cmd(&cmd_head, cmd);
		i++;
	}
	shell->cmd_head = cmd_head;
}

