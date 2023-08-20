/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 20:53:57 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 17:30:02 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	append_to_new_cmd(t_parse_vars *vars, int last)
{
	if ((!last && vars->j >= vars->size - 1) || last)
	{
		vars->new_str[vars->j] = '\0';
		if (!last)
			vars->size += 30;
		else
			vars->size = ft_strlen(vars->new_str) + 1;
		vars->new_str = ft_realloc(vars->new_str, vars->size);
		if (!vars->new_str)
			return (0);
		vars->j = ft_strlen(vars->new_str);
	}
	return (1);
}

static void	expand_helper(t_parse_vars *vars, char *env_value, int *i, int *j)
{
	vars->quote = env_value[(*i)];
	vars->new_env_value[(*j)++] = env_value[(*i)++];
	while (env_value[(*i)] && env_value[(*i)] != vars->quote)
		vars->new_env_value[(*j)++] = env_value[(*i)++];
	vars->new_env_value[(*j)++] = env_value[(*i)++];
}

char	*parse_expand(t_parse_vars *vars, t_tok_pos **tok_pos, char *env_value)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	vars->new_env_value = malloc(ft_strlen(env_value) + 1);
	if (vars->j != 0 && ft_strchr(" \v\t\f\b\n", vars->new_str[vars->j - 1]))
		ft_increment_index(env_value, " \v\t\f\b\n", &i, 1);
	while (env_value[i])
	{
		if (ft_strchr("\"'", env_value[i]))
			expand_helper(vars, env_value, &i, &j);
		else if (ft_strchr(" \t\v\f\b\n", env_value[i]))
		{
			vars->new_env_value[j++] = env_value[i++];
			vars->new_tok_pos = ft_lstnew_tokenpos(vars->j + j - 1);
			ft_lstadd_back_tokenpos(tok_pos, vars->new_tok_pos);
			ft_increment_index(env_value, " \v\t\f\b\n", &i, 1);
		}
		else
			vars->new_env_value[j++] = env_value[i++];
	}
	vars->new_env_value[j] = '\0';
	vars->temp = ft_strdup(vars->new_env_value);
	return (ft_free(env_value), ft_free(vars->new_env_value), vars->temp);
}

int	parse_dollar_extra(t_shell *shell, t_parse_vars *vars, \
char *str, t_tok_pos **tok_pos)
{
	char	*env_value;

	if (str[vars->i + 1] && (ft_isalnum(str[vars->i + 1]) \
	|| ft_strchr("_?$\"'", str[vars->i + 1])))
	{
		vars->i++;
		env_value = expand(shell, str + vars->i, &vars->i);
		env_value = parse_expand(vars, tok_pos, env_value);
		if (!env_value)
			return (0);
		vars->new_str[vars->j] = '\0';
		vars->new_str = ft_join(vars->new_str, env_value);
		if (!vars->new_str)
			return (0);
		vars->j = ft_strlen(vars->new_str);
		vars->size = vars->j;
	}
	else
		vars->new_str[vars->j++] = str[vars->i++];
	return (1);
}

int	check_last_char(char *input, int i)
{
	while (i >= 0)
	{
		if (!ft_strchr(" \n\v\f\b><", input[i]))
			return (1);
		else if (ft_strchr("><", input[i]))
			return (0);
		i--;
	}
	return (1);
}
