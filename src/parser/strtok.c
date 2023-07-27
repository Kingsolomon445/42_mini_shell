/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:36:24 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/20 11:39:18 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *ft_increment(char *str, const char *delims)
{
	char *tmp;

	tmp = str;
	while (*tmp && ft_strchr(delims, *tmp))
		tmp++;

	return (tmp);
}

static int	ft_counter(char *str, const char *delims)
{
	int		count;
	char	*tmp;
	int		found_delim;

	count = 0;
	found_delim = 0;
	tmp = ft_increment(str, delims);
	while (*tmp)
	{
		tmp++;
		if (ft_strchr(delims, *tmp))
		{
			tmp = ft_increment(tmp, delims);
			found_delim = 1;
		}
		if (found_delim && *tmp)
			count++;
	}
	return (count + 1);
}

static char	*make_string(char *tmp, int len)
{
	char	*token;

	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, tmp, len + 1);
	return (token);
}

char	**ft_strtok(char *str, const char *delims)
{
	char	**tokens;
	int		size;
	int		token_no;
	char	*new_str;
	char	*new_str_ptr;
	char	*temp;

	size = ft_counter(str, delims);
	tokens = (char **)malloc(sizeof(char *) * (size + 1));
	if (!tokens)
		return (NULL);
	new_str = ft_strtrim(str, delims);
	if (!new_str)
		return (ft_free(tokens), NULL);
	token_no = 0;
	temp = new_str;
	new_str_ptr = new_str;
	while (*new_str)
	{
		if (ft_strchr(delims, *new_str))
		{
			*(tokens + token_no) = make_string(temp, new_str - temp);
			if (!(*tokens + token_no))
				return (ft_free(new_str_ptr), free_split_alloc(tokens), NULL);
			token_no++;
			new_str = ft_increment(new_str, delims);
			if (!*new_str)
				break ;
			temp = new_str;
		}
		else
			new_str++;
	}
	*(tokens + token_no) = make_string(temp, new_str - temp);
	if (!(*(tokens + token_no)))
		return(ft_free(new_str_ptr), free_split_alloc(tokens), NULL);
	tokens[token_no + 1] = NULL;
	return (ft_free(new_str_ptr), tokens);
}