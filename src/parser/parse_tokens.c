/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:25:00 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 20:28:30 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_token(t_token_pos *token_pos, char *command)
{
	t_token_pos *cur_token_pos;
	int	i;

	cur_token_pos = token_pos;
	i = 0;
	while (cur_token_pos)
	{
		i++;
		if (!cur_token_pos->next)
		{
			if (ft_strlen(command) > (size_t)cur_token_pos->index)
				i++;
		}
		cur_token_pos = cur_token_pos->next;
	}
	return (i);
}

char	*make_token(char *command, int *start, int *end, t_dollar **dollar_head, int *dollar_idx)
{
	char	*token;
	int		i;

	token = malloc(*end - *start + 1);
	i = 0;
	while(*start < *end)
	{
		token[i++] = command[*start];
		(*start)++;
	}
	token[i] = '\0';
	(*start)++;
	if (ft_strchr(token, '$'))
	{
		token = ft_substitute(token, dollar_head, dollar_idx);
	}
	return (token);
}

char	**create_tokens(t_commands *cmd)
{
	char **tokens;
	char	*new_str;
	int		start;
	int		end;
	int		token_number;
	int		dollar_idx;
	t_token_pos *cur_token_pos;

	tokens = (char **)malloc(sizeof(char *) * (count_token(cmd->token_pos, cmd->command) + 1));
	dollar_idx = 0;
	new_str = ft_strdup(cmd->command);
	cur_token_pos = cmd->token_pos;
	start = 0;
	token_number = 0;
	while(cur_token_pos)
	{
		end = cur_token_pos->index;
		tokens[token_number++] = make_token(new_str, &start, &end, &cmd->dollar, &dollar_idx);
		cur_token_pos = cur_token_pos->next;
	}
	end = ft_strlen(new_str);
	if (end > start)
		tokens[token_number++] = make_token(new_str, &start, &end, &cmd->dollar, &dollar_idx);
	tokens[token_number] = NULL;
	return (tokens);
}
