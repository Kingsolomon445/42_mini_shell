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

#include <stdbool.h>

void echo_echo(t_commands *cmd)
{
    int no_nl;
    int start_idx;
    int i;

    start_idx = 1;
    no_nl = 0;
    if (compare_cmd("-n", cmd->toks[1]))
    {
        no_nl = 1;
        start_idx = 2;
    }

    i = start_idx;
    while (cmd->toks[i])
    {
        write(1, cmd->toks[i], ft_strlen(cmd->toks[i]));
        if (cmd->toks[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!no_nl)
        write(1, "\n", 1);
}