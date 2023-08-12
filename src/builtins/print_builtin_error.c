/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_builtin_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:16:05 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/12 19:27:13 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../include/minishell.h"

void	invalid_option(t_shell *shell, char *builtin, char *error_arg)
{
	if (compare_str(builtin, "export"))
	{
		ft_printf_fd(2, "minishell: %s: `%c%c': invalid option\nexport: ", \
		builtin, error_arg[0], error_arg[1]);
		ft_printf_fd(2, "usage: export [-nf] [name[=value] ...] or export -p\n");
	}
	else
	{
		ft_printf_fd(2, "minishell: %s: `%s': invalid option\n", \
		builtin, error_arg);
		ft_printf_fd(2, "unset: usage: unset [-f] [-v] [name ...]\n");
	}
	shell->last_status = 2;
}

void	non_numeric_exit(t_shell *shell, char *invalid_arg, int exit_code)
{
	ft_printf_fd(2, "minishell: exit: ");
	ft_printf_fd(2, "%s: numeric argument required\n", invalid_arg);
	ft_exit_shell(shell, exit_code);
}

void	normal_exit(t_shell *shell)
{
	// ft_printf_fd(2, "exit\n");
	shell->last_status = 0;
	ft_exit_shell(shell, EXIT_SUCCESS);
}

int	print_error(int status, char *builtin, char *error_arg, char *msg)
{
	// if (compare_str(error_arg, "exit"))
	// 	ft_printf_fd(2, "exit\n");
	if (builtin)
		ft_printf_fd(2, "minishell: %s: `%s': %s\n", builtin, error_arg, msg);
	else
		ft_printf_fd(2, "minishell: %s: %s\n", error_arg, msg);
	return (status);
}
