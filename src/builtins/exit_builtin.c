/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:22:08 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 18:27:20 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	perform_exit(t_commands *cmd)
{
	if (cmd->toks[1] && ft_isnumber(cmd->toks[1]) && cmd->toks[2])
		return (print_error(1, NULL, "exit", MANYARGS));
	else if (cmd->toks[1])
	{
		ft_printf_fd(1, "exit\n");
		if (!ft_isnumber(cmd->toks[1]))
			return (non_numeric_exit(cmd->toks[1]), 255);
		else
		{
			if (*(cmd->toks[1]) == '-')
			{
				if (ft_atoi_ulong(cmd->toks[1] + 1) > 9223372036854775808UL)
					return (non_numeric_exit(cmd->toks[1]), 255);
				else
					return (ft_atoi_ulong(cmd->toks[1]));
			}
			else if (ft_atoi_ulong(cmd->toks[1]) > 9223372036854775807)
				return (non_numeric_exit(cmd->toks[1]), 255);
			else
				return (ft_atoi_ulong(cmd->toks[1]));
		}
	}
	else
		return (ft_printf_fd(1, "exit\n"), EXIT_SUCCESS);
}
