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
	
    // buff = NULL;
    buff = getcwd(NULL, 0);
    if (!buff)
    {
        shell->last_status = 1;
        exit(EXIT_FAILURE);
    }
    // write(1, buff, ft_strlen(buff));
    write(1, "find\nme\n", 8);
    shell->last_status = 0;
}