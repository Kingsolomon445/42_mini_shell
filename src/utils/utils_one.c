/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:40:18 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/07 17:45:21 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_increment(char *str, const char *delims)
{
	char	*tmp;

	tmp = str;
	while (*tmp)
	{
		if (ft_strchr(delims, *tmp))
			tmp++;
		else
			break ;
	}
	return (tmp);
}

int	count_ocurrence(char *str, char ch)
{
	int occ;
	int	i;

	occ = 0;
	i = 0;
	while (*(str + i))
	{
		if (*(str + i) == ch)
			occ++;
		i++;
	}
	return (occ);
}

int	count_commands(char **commands)
{
	int	i;

	i = 0;
	while(commands[i])
		i++;
	return (i);
}

int	count_tokens(char *str, const char *delims)
{
	int		count;
	char	*tmp;
	int		found_delim;

	count = 0;
	tmp = str;
	found_delim = 0;
	if (ft_strchr(delims, *tmp))
		tmp = ft_increment(tmp, delims);
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

int	compare_cmd(char *src, char *dst)
{
	int	i;

	i = 0;
	if (src[ft_strlen(src) - 1] == '\n')
		src[ft_strlen(src) - 1] = '\0';
	if (src[i] == '\n' || ft_strlen(src) != ft_strlen(dst))
		return (0);
	while (src[i] && src[i] != '\n')
	{
		if (src[i] != dst[i] || (dst[i] == '\0' && src[i]))
			return (0);
		i++;
	}
	return (1);
}

int	compare_builtin(char *src, char *dst)
{
	int	i;

	i = 0;
	if (src[ft_strlen(src) - 1] == '\n')
		src[ft_strlen(src) - 1] = '\0';
	if (src[i] == '\n' || ft_strlen(src) != ft_strlen(dst))
		return (0);
	while (src[i] && src[i] != '\n')
	{
		if (ft_tolower(src[i]) != ft_tolower(dst[i]) \
		|| (dst[i] == '\0' && src[i]))
			return (0);
		i++;
	}
	return (1);
}
