/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 20:13:23 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 20:54:15 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_quotes(t_shell *shell, t_parse_vars *vars, char *str)
{
	char	quote;

	quote = str[vars->i];
	vars->i++;
	while (str[vars->i] && str[vars->i] != quote)
	{
		if (str[(vars->i)] == '$')
		{
			if (quote == '"' && str[(vars->i) + 1] != '"' \
			&& !shell->do_not_expand)
			{
				if (!parse_dollar(shell, vars, str))
					return (0);
			}
			else
				vars->new_str[vars->j++] = str[vars->i++];
		}
		else
			vars->new_str[vars->j++] = str[vars->i++];
		if (!append_to_new_cmd(vars, 0))
			return (0);
	}
	if (str[vars->i])
		vars->i++;
	return (1);
}

int	parse_dollar(t_shell *shell, t_parse_vars *vars, char *str)
{
	char	*env_value;

	if (str[vars->i + 1] && (ft_isalnum(str[vars->i + 1]) \
	|| ft_strchr("_?$\"'", str[vars->i + 1])))
	{
		vars->i++;
		env_value = expand(shell, str + vars->i, &vars->i);
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

char	*ft_realloc(char *str, int size)
{
	char	*new_str;

	new_str = malloc(size);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, ft_strlen(str) + 1);
	ft_free(str);
	return (new_str);
}

char	*ft_join(char *str, char *to_add)
{
	char	*final_str;

	if (!to_add)
		return (NULL);
	if (!str)
		final_str = ft_strdup(to_add);
	else
		final_str = ft_strjoin(str, to_add);
	ft_free(str);
	ft_free(to_add);
	return (final_str);
}

int	count_commands(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
		i++;
	return (i);
}
