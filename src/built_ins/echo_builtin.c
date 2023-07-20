/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:49:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/07/19 18:08:37 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	echo_echo(t_commands *cmd)
{
    // printf("first == %s second == %s-- third == %s\n", cmd->toks[0], cmd->toks[1], cmd->toks[2]);
    if (compare_cmd("-n", cmd->toks[1]))
        write(1, cmd->toks[2], ft_strlen(cmd->toks[2]));
    else
    {
        write(1, cmd->toks[1], ft_strlen(cmd->toks[1]));
        write(1, "\n", 1);
    }
}