/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:40:18 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/17 17:19:11 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	compare_str(char *src, char *dst)
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

int	is_it_builtin(char **builtins, char *cmd)
{
	int	i;

	i = 0;
	while (*(builtins + i))
	{
		if (compare_builtin(*(builtins + i), cmd))
			return (1);
		i++;
	}
	return (0);
}

void	ft_exit_shell(t_shell *shell, long status)
{
	ft_free_shell(shell);
	exit(status & 0xff);
}

void	print_error_exit(t_shell *shell, char *error_msg, int status)
{
	ft_printf_fd(2, "%s", error_msg);
	ft_exit_shell(shell, status);
}
