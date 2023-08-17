/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 15:01:35 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/16 20:54:33 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_shell_for_next_read(t_shell *shell)
{
	shell->cmd_head = NULL;
	shell->input = NULL;
	shell->commands = NULL;
	shell->cmd_pos_head = NULL;
	shell->heredoc_ran = 0;
	shell->sucess = 1;
	shell->delims = " \t\b\v\n\f";
	shell->path = get_final_path();
}

static int	init_prompt_title(t_shell *shell)
{
	char	*holder;

	if (getenv("USER"))
	{
		holder = ft_strjoin(getenv("USER"), "@minishell$ ");
		if (!holder)
			return (0);
	}
	else
	{
		holder = ft_strjoin("guest", "@minishell$ ");
		if (!holder)
			return (0);
	}
	shell->success_prompt = ft_strjoin("🟢", holder);
	if (!shell->success_prompt)
		return (ft_free(holder), 0);
	shell->failed_prompt = ft_strjoin("🔴", holder);
	if (!shell->failed_prompt)
		return (ft_free(holder), ft_free(shell->success_prompt), 0);
	return (ft_free(holder), 1);
}

int	init_shell(t_shell *shell)
{
	char	*new_level;

	init_shell_for_next_read(shell);
	if (!init_prompt_title(shell))
		return (0);
	shell->welcome_str = shell->success_prompt;
	shell->builtins = ft_split(BUILTINS, ' ');
	if (!shell->builtins || !shell->welcome_str)
		return (0);
	shell->last_status = 0;
	if (!init_shellenv(shell))
		return (0);
	if (!getenv("SHLVL"))
	{
		if (!ft_putenv(shell, ft_strdup("SHLVL=1")))
			return (0);
		new_level = ft_strdup("1");
	}
	else
		new_level = ft_itoa(ft_atoi(getenv("SHLVL")) + 1);
	if (!new_level)
		return (0);
	if (!update_env_item(shell, "SHLVL", new_level))
		return (ft_free(new_level), 0);
	return (ft_free(new_level), 1);
}
