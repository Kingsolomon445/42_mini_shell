/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:40:18 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/19 12:53:01 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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