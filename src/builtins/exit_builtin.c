/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 12:22:08 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/11 16:49:13 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	perform_exit(t_commands *cmd, t_shell *shell)
{
	if (cmd->toks[1] && ft_isnumber(cmd->toks[1]) && cmd->toks[2])
		shell->last_status = print_error(1, NULL, "exit", MANYARGS);
	else if (cmd->toks[1])
	{
		// ft_printf_fd(2, "exit\n");
		if (!ft_isnumber(cmd->toks[1]))
			non_numeric_exit(shell, cmd->toks[1], 255);
		else
		{
			if (*(cmd->toks[1]) == '-')
			{
				if (ft_atoi_ulong(cmd->toks[1] + 1) > 9223372036854775808UL)
					non_numeric_exit(shell, cmd->toks[1], 255);
				else
					ft_exit_shell(shell, ft_atoi_ulong(cmd->toks[1]));
			}
			else if (ft_atoi_ulong(cmd->toks[1]) > 9223372036854775807)
				non_numeric_exit(shell, cmd->toks[1], 255);
			else
				ft_exit_shell(shell, ft_atoi_ulong(cmd->toks[1]));
		}
	}
	else
		normal_exit(shell);
}
