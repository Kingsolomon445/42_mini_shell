/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:25:00 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 19:26:01 by ofadahun         ###   ########.fr       */
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

char	*make_token(char *command, int start, int end)
{
	char	*token;

	token = ft_substr(command, start, end - start);
	if (!token)
		return (NULL);
	// printf("token == <-%s->\n", token);
	return (token);
}

char	**create_tokens(t_commands *cmd)
{
	char **tokens;
	char	*new_cmd;
	int		start;
	int		end;
	int		token_number;
	t_token_pos *cur_token_pos;

	tokens = (char **)malloc(sizeof(char *) * (count_token(cmd->token_pos, cmd->command) + 1));
	new_cmd = ft_strdup(cmd->command);
	cur_token_pos = cmd->token_pos;
	start = 0;
	token_number = 0;
	while(cur_token_pos)
	{
		end = cur_token_pos->index;
		tokens[token_number++] = make_token(new_cmd, start, end);
		if (!tokens[token_number - 1])
			return (ft_free_split(tokens), ft_free(new_cmd), NULL);
		cur_token_pos = cur_token_pos->next;
		start = end + 1;
	}
	end = ft_strlen(new_cmd);
	if (end > start || !token_number)
		tokens[token_number++] = make_token(new_cmd, start, end);
	if (token_number && !tokens[token_number - 1])
		return (ft_free_split(tokens), ft_free(new_cmd), NULL);
	tokens[token_number] = NULL;
	return (tokens);
}
