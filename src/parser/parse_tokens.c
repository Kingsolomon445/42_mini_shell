/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:25:00 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 19:09:17 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_token(t_tok_pos *tok_pos, char *command)
{
	t_tok_pos	*cur_tok_pos;
	int			i;

	cur_tok_pos = tok_pos;
	i = 0;
	while (cur_tok_pos)
	{
		i++;
		if (!cur_tok_pos->next)
		{
			if (ft_strlen(command) >= (size_t)cur_tok_pos->index)
				i++;
		}
		cur_tok_pos = cur_tok_pos->next;
	}
	return (i);
}

static int	make_tok(char **tokens, char *command, int *start, int end)
{
	*tokens = ft_substr(command, *start, end - *start);
	if (!*tokens)
		return (0);
	tokens[1] = NULL;
	*start = end + 1;
	return (1);
}

char	**create_tokens(t_commands *cmd)
{
	char			**tokens;
	char			*new_cmd;
	int				start;
	int				tok_num;
	t_tok_pos		*cur_tok_pos;

	new_cmd = ft_strdup(cmd->command);
	tokens = (char **)malloc(sizeof(char *) * \
	(count_token(cmd->tok_pos, cmd->command) + 2));
	if (!tokens || !new_cmd)
		return (ft_free(new_cmd), NULL);
	cur_tok_pos = cmd->tok_pos;
	start = 0;
	tok_num = 0;
	while (cur_tok_pos)
	{
		if (!make_tok(tokens + tok_num++, new_cmd, &start, cur_tok_pos->index))
			return (ft_free_split(tokens), ft_free(new_cmd), NULL);
		cur_tok_pos = cur_tok_pos->next;
	}
	if (ft_strlen(new_cmd) >= (size_t)start)
		if (!make_tok(tokens + tok_num++, new_cmd, &start, ft_strlen(new_cmd)))
			return (ft_free_split(tokens), ft_free(new_cmd), NULL);
	return (ft_free(new_cmd), tokens);
}
