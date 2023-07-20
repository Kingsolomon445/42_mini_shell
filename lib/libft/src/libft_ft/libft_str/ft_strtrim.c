/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 12:30:30 by sbhatta           #+#    #+#             */
/*   Updated: 2023/06/15 13:44:35 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/libft.h"

static int	ft_find(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_trimstart(char *str, char const *set)
{
	char	*start;

	start = NULL;
	while (*str)
	{
		if (!(ft_find(*str, (char *)set)))
		{
			start = str;
			break ;
		}
		str++;
	}
	return (start);
}

static char	*ft_trimend(char *str, char const *set)
{
	char	*end;

	end = NULL;
	while (*str)
	{
		if (!(ft_find(*str, (char *)set)))
		{
			end = str;
			break ;
		}
		str--;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	char	*start;
	char	*end;
	char	*result;

	str = (char *)s1;
	start = NULL;
	end = NULL;
	if (s1 && set)
	{
		start = ft_trimstart(str, set);
		if (!start)
			return (ft_strdup(""));
		str = (char *)s1 + ft_strlen(s1) - 1;
		end = ft_trimend(str, set);
		result = malloc(end - start + 2);
		if (!result)
			return (NULL);
		ft_strlcpy(result, start, end - start + 2);
		return (result);
	}
	else
		return (0);
}
