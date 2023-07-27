/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:12:39 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/19 17:28:21 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	get_pwd(t_shell *shell)
{
	char	*buff;
	
    buff = getcwd(NULL, 0);
    if (!buff)
    {
        perror("minishell: pwd");
        shell->last_status = 1;
        return;
    }
    write(1, buff, ft_strlen(buff));
    write(1, "\n", 1);
    ft_free(buff);
    shell->last_status = 0;
}