/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:12:39 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/11 12:57:49 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_pwd(t_shell *shell)
{
	char	*buff;

	buff = getcwd(NULL, 0);
	if (!buff)
	{
		shell->last_status = 1;
		exit (1);
	}
	write(1, buff, ft_strlen(buff));
	write(1, "\n", 1);
	shell->last_status = 0;
	return (0);
}
