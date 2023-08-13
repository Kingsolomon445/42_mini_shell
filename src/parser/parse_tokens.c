/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:25:00 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 15:20:35 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_dollar	*check_to_substitute(t_dollar **dollar_head, int idx)
{
	t_dollar *dollar;

	dollar = *dollar_head;
	while(dollar)
	{
		if (dollar->index == idx)
			return (dollar);
		dollar = dollar->next;
	}
	return (NULL);
}

char	*add_to_token(char *token, char *to_add)
{
	char	*final_token;

	if (!to_add)
		return (NULL);
	if (!token)
		final_token = ft_strdup(to_add);
	else
		final_token = ft_strjoin(token, to_add);
	ft_free(token);
	ft_free(to_add);
	return (final_token);
}

char	*ft_substitute(char *new_token, t_dollar **dollar_head, int *dollar_idx)
{
	t_dollar	*dollar;
	size_t		len;
	char		*found_dollar;
	char		*final_token;
	
	final_token = NULL;
	while (*new_token)
	{
		found_dollar = ft_strchr(new_token, '$');
		if (!found_dollar)
		{
			final_token = add_to_token(final_token, ft_strdup(new_token));
			if (!final_token)
				return (NULL);
			break;
		}
		len = found_dollar - new_token;
		if (len > 0)
		{
			final_token = add_to_token(final_token, ft_substr(new_token, 0, len));
			if (!final_token)
				return (NULL);
			new_token += len;
		}
		dollar = check_to_substitute(dollar_head, *dollar_idx);
		(*dollar_idx)++;
		if (dollar)
		{
			final_token = add_to_token(final_token, ft_strdup(dollar->value));
			if (!final_token)
				return (NULL);
			new_token ++;
		}
		else
		{
			found_dollar = ft_strchr(new_token + 1, '$');
			if (!found_dollar)
			{
				final_token = add_to_token(final_token, ft_strdup(new_token));
				if (!final_token)
					return (NULL);
				break;
			}
			(*dollar_idx)++;
			len = found_dollar - new_token;
			final_token = add_to_token(final_token, ft_substr(new_token, 0, len));
			if (!final_token)
				return (NULL);
			new_token += len;
		}
	}
	return (final_token);
}

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

char	*make_token(char *command, int start, int end, t_dollar **dollar_head, int *dollar_idx)
{
	char	*token;

	token = ft_substr(command, start, end - start);
	if (!token)
		return (NULL);
	if (ft_strchr(token, '$'))
	{
		token = ft_substitute(token, dollar_head, dollar_idx);
		if (!token)
			return (NULL);
	}
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
	int		dollar_idx;
	t_token_pos *cur_token_pos;

	tokens = (char **)malloc(sizeof(char *) * (count_token(cmd->token_pos, cmd->command) + 1));
	dollar_idx = 0;
	new_cmd = ft_strdup(cmd->command);
	cur_token_pos = cmd->token_pos;
	start = 0;
	token_number = 0;
	while(cur_token_pos)
	{
		end = cur_token_pos->index;
		tokens[token_number++] = make_token(new_cmd, start, end, &cmd->dollar, &dollar_idx);
		if (!tokens[token_number - 1])
			return (ft_free_split(tokens), ft_free(new_cmd), NULL);
		cur_token_pos = cur_token_pos->next;
		start = end + 1;
	}
	end = ft_strlen(new_cmd);
	if (end > start || !token_number)
		tokens[token_number++] = make_token(new_cmd, start, end, &cmd->dollar, &dollar_idx);
	if (token_number && !tokens[token_number - 1])
		return (ft_free_split(tokens), ft_free(new_cmd), NULL);
	tokens[token_number] = NULL;
	return (tokens);
}
