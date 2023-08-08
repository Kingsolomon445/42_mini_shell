/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:25:00 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/08 17:07:36 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	make_token(t_create_tokens *vars)
{
	char	*token;
	
	token = malloc(vars->i + 1);
	if (!token)
		return (0);
	ft_strlcpy(token, vars->temp, vars->i + 1);
	*(vars->tokens + vars->token_no) = token;
	if (!(*vars->tokens + vars->token_no))
		return (ft_free_split(vars->tokens), 0);
	vars->token_no++;
	return (1);
}

static void	eval_quote(t_create_tokens *vars)
{
	char	quote;

	quote = *vars->new_str;
	vars->new_str++;
	vars->i++;
	while(*vars->new_str != quote)
	{
		vars->new_str++;
		vars->i++;
	}
}

static t_create_tokens *create_tokens_vars_init(const char *delims, char *str)
{
	t_create_tokens *vars;

	vars = malloc(sizeof(t_create_tokens));
	vars->size = count_tokens(str, delims);
	vars->tokens = (char **)malloc(sizeof(char *) * (vars->size + 1));
	if (!vars->tokens)
		return (NULL);
	vars->new_str = ft_strtrim(str, delims);
	if (!vars->new_str)
		return (NULL);
	vars->i = 0;
	vars->token_no = 0;
	vars->temp = vars->new_str;
	vars->new_str_ptr = vars->new_str;
	return (vars);
}

char	**create_tokens(char *str, const char *delims)
{
	t_create_tokens *vars;

	vars = create_tokens_vars_init(delims, str);
	if (!vars)
		return (NULL);
	while (*vars->new_str)
	{
		if (ft_strchr("\"'", *vars->new_str))
			eval_quote(vars);
		if (ft_strchr(delims, *vars->new_str))
		{
			if (!make_token(vars))
				return (NULL);
			vars->new_str = ft_increment(vars->new_str, delims);
			vars->i = 0;
			vars->temp = vars->new_str;
			continue ;
		}
		vars->new_str++;
		vars->i++;
	}
	if (!make_token(vars))
		return (NULL);
	vars->tokens[vars->token_no] = NULL;
	return (ft_free(vars->new_str_ptr), vars->tokens);
}