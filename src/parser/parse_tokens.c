// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   parse_tokens.c                                     :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/08/07 17:38:34 by ofadahun          #+#    #+#             */
// /*   Updated: 2023/08/08 11:55:53 by ofadahun         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../include/minishell.h"

// static void	make_new_dollar(char *str, t_eval_quotedtok_vars *vars, t_tokens *tok)
// {
// 	t_dollar	*dollar;

// 	if (vars->expand_dollar == 1)
// 	{
// 		if (!str[vars->i + 1])
// 			vars->expand_dollar = 0;
// 		else if (!ft_isalpha(str[vars->i + 1]) && str[vars->i + 1] == '_' && str[vars->i + 1] != '?')
// 			vars->expand_dollar = 0;
// 	}
// 	if (str[vars->i + 1] && ft_strchr("\"'", str[vars->i + 1]))
// 	{
// 		(vars->i)++;
// 		vars->expand_dollar = 0;
// 	}
// 	else
// 	{
// 		dollar = ft_lstnew_dollar(vars->expand_dollar, vars->idx);
// 		if (vars->idx == 0)
// 			tok->dollar_head = dollar;
// 		else
// 			ft_lstadd_back_dollar(&tok->dollar_head, dollar);
// 		(vars->idx)++;
// 	}
// }

// static void	eval_quotes_with_dollar_symbol(t_eval_quotedtok_vars *vars, char *str, t_tokens *tok, char quote)
// {
// 	vars->expand_dollar = 0;
// 	if (str[vars->i] == '"')
// 		vars->expand_dollar = 1;
// 	vars->i++;
// 	while (str[vars->i] && str[vars->i] != quote)
// 	{
// 		if (str[vars->i] == '$')
// 			make_new_dollar(str, vars, tok);
// 		vars->new_str[vars->j++] = str[vars->i++];
// 	}
// 	vars->i++;
// }

// static t_eval_quotedtok_vars *eval_quotedtok_vars_init(char *new_str)
// {
// 	t_eval_quotedtok_vars	*vars;
	
// 	vars = malloc(sizeof(t_eval_quotedtok_vars));
// 	if (!vars)
// 		return (NULL);
// 	vars->i = 0;
// 	vars->j = 0;
// 	vars->idx = 0;
// 	vars->new_str = new_str;
// 	return (vars);
// }

// char	*eval_quotedtok(char *str, t_tokens *tok)
// {
// 	char					new_str[ft_strlen(str) + 1];
// 	t_eval_quotedtok_vars	*vars;

// 	vars = eval_quotedtok_vars_init(new_str);
// 	if (!vars)
// 		return (NULL);
// 	while (str[vars->i])
// 	{
// 		if (ft_strchr("\"'", str[vars->i]))
// 		{
// 			eval_quotes_with_dollar_symbol(vars, str, tok, str[vars->i]);
// 			continue ;
// 		}
// 		if (str[vars->i] == '$')
// 		{
// 			vars->expand_dollar = 1;
// 			make_new_dollar(str, vars, tok);
// 		}
// 		if (ft_strchr("\"'", str[vars->i]))
// 			continue ;
// 		vars->new_str[vars->j++] = str[vars->i];
// 		vars->i++;
// 	}
// 	vars->new_str[vars->j] = '\0';
// 	return (ft_strdup(vars->new_str));
// }
