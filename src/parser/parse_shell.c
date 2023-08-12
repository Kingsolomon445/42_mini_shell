/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:38:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/12 19:46:17 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*make_commands(const char *input, int start, int end)
{
   char *command;

   command = ft_substr(input, start, end - start);
   return (command);
   
}

char **ft_split_commands(const char *inp, int cmd_no, t_shell *shell)
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

int	parse_pipe(t_shell *shell, char **input, int *pipe_count, int i, char last_char)
{
	char	*temp;
	
	(*pipe_count)++;
	if (i == 0 || (*input)[i + 1] == '\0' || ((*input)[i - 1] == '|' && (*input)[i + 1] == '|') || ft_strchr("><", last_char))
		return (ft_printf_fd(2, PIPESYNTAXERR), ft_free(*input), set_status(shell, 258), 0);
	while((*input)[i] && ft_strchr(" \n\t\v\f\b><", (*input)[i]))
		i++;
	if (!((*input)[i]))
		return (ft_printf_fd(2, SYNTAXERR), ft_free(*input), set_status(shell, 258), 0);
	if ((*input)[i - 1] == '|')
	{
		temp = ft_substr(*input, 0, i - 1);
		ft_free(*input);
		*input = temp;
		return (2);
	}
	return (1);
}


int	split_and_validate(t_shell *shell)
{
	int in_s_quotes;
	int	in_d_quotes;
	int	pipe_count;
	int	i;
	char	*input;
	char	last_char;
	int	parse_pipe_res;
	t_cmd_pos *cmd_pos;

	in_s_quotes = 0;
	in_d_quotes = 0;
	pipe_count = 0;
	i = 0;
	input = ft_strdup(shell->input);
	if (!input)
		return (0);
	while(input[i])
	{
		if (ft_strchr("'", input[i]) && !in_d_quotes)
			in_s_quotes = !in_s_quotes;
		if (ft_strchr("\"", input[i]) && !in_s_quotes)
			in_d_quotes = !in_d_quotes;
		else if (input[i] == '|' && !in_d_quotes && !in_s_quotes)
		{
			parse_pipe_res = parse_pipe(shell, &input, &pipe_count, i, last_char);
			if (!parse_pipe_res)
				return (0);
			else if (parse_pipe_res == 2)
				break ;
			cmd_pos = ft_lstnew_cmdpos(i);
			if (!cmd_pos)
				return (0);
			ft_lstadd_back_cmdpos(&shell->cmd_pos_head, cmd_pos);
		}
		last_char = input[i];
		i++;
	}
	if ((in_s_quotes || in_d_quotes))
		return (ft_printf_fd(2, "syntax error near unexpected token `%s'\n", input), ft_free(input), set_status(shell, 258), 0);
	shell->commands = ft_split_commands(input,  pipe_count + 1, shell);
	return (ft_free(input), 1);
}

int	parse_shell(t_shell *shell)
{
	char	**commands;
	t_commands *cmd_head;
	t_commands *cmd;
	int		i;

	i = 0;
	cmd_head = NULL;
	if (!split_and_validate(shell))
		return (0);
	if (!shell->commands)
		return (0);
	commands = shell->commands;
	shell->no_cmds = 0;
	while(commands[i])
	{
		// printf("commands here == %s\n", commands[i]);
		shell->no_cmds++;
		cmd = parse_commands(shell, commands[i]);
		if (!cmd)
			return (0);
		ft_lstadd_back_cmd(&cmd_head, cmd);
		i++;
	}
	shell->cmd_head = cmd_head;
	return (1);
}

