/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:59 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 20:49:25 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_if_to_make_space(char *command, char *new_command, char quote)
{
	int 	i;

	i = 0;
	while(command[i] == quote)
		i++;
	if (!ft_strchr(" \t\v\b\f\n", command[i]))
		return (0);
	while(command[i])
	{
		if (!ft_strchr(" \t\v\b\f\n", command[i]))
			return (1);
		i++;
	}
	i = 0;
	if (ft_strnstr(new_command, "echo", ft_strlen(new_command)))
	{
		while(new_command[i] && !ft_strchr(" \t\v\b\f\n", new_command[i]))
			i++;
	}
	while(new_command[i] && ft_strchr(" \t\v\b\f\n", new_command[i]))
		i++;
	if (new_command[i])
		return (1);
	return (0);
}

static int	parse_and_escape_backslash(char **new_cmd, char *cmd, int *i, int *j, t_token_pos **token_pos)
{
	t_token_pos *new_token_pos;

	if (cmd[*i] == '\\')
		(*i)++;
	if (cmd[*i])
	{
		(*new_cmd)[(*j)++] = cmd[(*i)++];
		while (cmd[*i] && ft_strchr(" \v\t\f\b\n", cmd[*i]) && ft_strchr(" \v\t\f\b\n", (*new_cmd)[(*j) - 1]))
			(*i)++;
		new_token_pos = ft_lstnew_tokenpos(*j - 1);
		if (!new_token_pos)
			return (0);
		if (ft_strchr(" \v\t\f\b\n", (*new_cmd)[(*j) - 1]))
			ft_lstadd_back_tokenpos(token_pos, new_token_pos);
	}
	return (1);
}


static void	eval_quotes(t_shell *shell, char *cmd, char **new_cmd, int *i, int *j, int *size, t_token_pos **token_pos)
{
	char	quote;
	t_token_pos *new_token_pos;
	
	quote = cmd[(*i)++];
	(*new_cmd)[(*j)] = '\0';
	if (cmd[(*i)] == quote && check_if_to_make_space(cmd + (*i) + 1, (*new_cmd), quote))
	{
		(*new_cmd)[(*j)++] = ' ';
		new_token_pos = ft_lstnew_tokenpos((*j) - 1);
		if (ft_strchr(" \v\t\f\b\n", (*new_cmd)[(*j) - 1]))
			ft_lstadd_back_tokenpos(token_pos, new_token_pos);
		while (cmd[(*i) + 1] == quote)
			(*i)++;
	}
	parse_quotes(shell, cmd, new_cmd, i, j, size, quote);
}


char	*parse(t_shell *shell, char *cmd, t_token_pos **token_pos, t_redirection **redirection)
{
	char	*new_cmd;
	int		size;
	int		i;
	int		j;

	size = 20;
	i = 0;
	j = 0;
	new_cmd = malloc(size);
	while (cmd[i])
	{
        if (ft_strchr("\"'", cmd[i]))
			eval_quotes(shell, cmd, &new_cmd, &i, &j, &size, token_pos);
		else if (cmd[i] == '$')
			parse_dollar(shell, &new_cmd, cmd, &i, &j, &size);
		else if (ft_strchr("><", cmd[i]))
			shell->sucess = parse_redirection(shell, cmd + i, &i, redirection);
		else
			shell->sucess = parse_and_escape_backslash(&new_cmd, cmd, &i, &j, token_pos);
		if (shell->do_not_run && shell->cmd_cnt > 1)
			break;
		if ((!shell->sucess && shell->cmd_cnt > 1) || shell->do_not_run)
			return (NULL);
		if (j >= size - 1)
		{
			new_cmd[j] = '\0';
			new_cmd = ft_realloc(new_cmd, &size);
			j = ft_strlen(new_cmd);
		}
    }
	new_cmd[j] = '\0';
	return (new_cmd);
}

t_commands *parse_commands(t_shell *shell, char *old_command)
{
    char *new_cmd;
	char	*cmd;
	t_token_pos *token_pos;
	t_redirection *redirection;
	t_commands	*command;
	
	
	shell->cmd_cnt = count_commands(shell->commands);
	shell->do_not_run = 0;
	cmd = ft_strtrim(old_command, " \t\n\b\v\f");
	token_pos = NULL;
	redirection = NULL;
	new_cmd = parse(shell, cmd, &token_pos, &redirection);
	if (!new_cmd)
		return (0);
	ft_free(cmd);
	cmd = ft_strdup(new_cmd);
	if (compare_str("", cmd) && redirection)
	{
		shell->last_status = 0;
		shell->do_not_run = 1;
	}
	if (!cmd)
		return (NULL);
	// printf("new cmd == %s\n", cmd);
	command = ft_lstnew_cmd(shell, redirection, token_pos, cmd);
	return (command);
}
