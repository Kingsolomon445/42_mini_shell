/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 17:49:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 18:21:35 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_n(char	*arg, int *need_print)
{
	int	i;

	i = 0;
	if (*need_print)
		return (0);
	if (arg[i] == '-')
		i++;
	if (arg[i] == '\0')
	{
		*need_print = 1;
		return (0);
	}
	while (arg[i])
	{
		if (arg[i] != 'n')
		{
			*need_print = 1;
			return (0);
		}
		i++;
	}
	*need_print = 0;
	return (1);
}

static void	print_echo_args(char **cmd_with_args)
{
	int	i;
	int	new_line;
	int	need_print;

	i = 1;
	new_line = 1;
	need_print = 0;
	while (cmd_with_args[i])
	{
		if (!is_valid_n(cmd_with_args[i], &need_print))
		{
			if (need_print)
			{
				ft_printf_fd(1, "%s", cmd_with_args[i]);
				if (cmd_with_args[i + 1])
					ft_printf_fd(1, " ");
			}
		}
		else
			new_line = 0;
		i++;
	}
	if (new_line)
		ft_printf_fd(1, "\n");
}

static void	print_echo_offcase(t_commands *cmd, int i)
{
	while (cmd->toks[i])
	{
		ft_printf_fd(1, "%s", cmd->toks[i]);
		if (cmd->toks[i + 1])
			ft_printf_fd(1, " ");
		i++;
	}
}

int	echo_echo(t_commands *cmd)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 0;
	if (cmd->toks[1] == NULL)
		ft_printf_fd(1, "\n");
	else
	{
		if (compare_str(cmd->toks[0], "echo"))
			print_echo_args(cmd->toks);
		else
		{
			if (compare_str(cmd->toks[1], "-n"))
			{
				new_line = 1;
				i++;
			}
			print_echo_offcase(cmd, i);
			if (!new_line)
				ft_printf_fd(1, "\n");
		}
	}
	return (0);
}
