/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 20:13:23 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 21:18:36 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_quotes(t_shell *shell, char *str, char **new_str, int *i, int *j, int *size, char quote)
{
	while (str[*i] && str[*i] != quote)
	{
		if (str[(*i)] == '$')
		{
			if (quote == '"'  && str[(*i) + 1] != '"')
				parse_dollar(shell, new_str, str, i, j, size);
			else
				(*new_str)[(*j)++] = str[(*i)++];
		}
		else
			(*new_str)[(*j)++] = str[(*i)++];
		if (*j == *size - 1)
		{
            (*new_str)[(*j)] = '\0';
			*new_str = ft_realloc(*new_str, size);
			*j = ft_strlen(*new_str);
		}
	}
	if (str[(*i)])
		(*i)++;
}

int	parse_dollar(t_shell *shell, char **new_str, char *str, int *i, int *j, int *size)
{
	char *env_value;
	
	if (str[(*i) + 1] && (ft_isalnum(str[(*i) + 1]) || ft_strchr("_?$\"'", str[(*i) + 1])))
	{
        (*i)++;
		env_value = expand(shell, str + *i, i);
        (*new_str)[*j] = '\0';
		*new_str = ft_join(*new_str, env_value);
		*j = ft_strlen(*new_str);
		*size = *j;
	}
    else
        (*new_str)[(*j)++] = str[(*i)++];
	return (1);
}

char	*ft_realloc(char *str, int *size)
{
	char	*new_str;

	*size += 20;
	new_str = malloc(*size);
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