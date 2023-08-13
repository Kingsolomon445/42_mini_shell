/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:13:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/13 17:23:21 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// ls | cat << stop | ls -la | cat << stop1 | ls | cat << stop2 | ls -la > > out | cat << stop3
// We need to handle when a command starts with / or ~ or /bin/ 


// ls >"./ with spaces"

/*touch whatever
cat <"./whatever" >"./whatever"
rm -rf whatever */

/*>echo>
/bin/rm -f echo

<echo<
/bin/rm -f echo

>>echo>>
/bin/rm -f echo*/
/*
cd ~/Desktop/

export test=" * "
touch "$USER * ?eHallo"
/bin/echo "$USER "*" ?e"*
rm -f "$USER * ?eHallo"

strip whitespaces after expansion

$tmp_test


echo $TES_T
*/


int	init_prompt_title(t_shell *shell)
{
	char *holder;
	
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
		return (ft_free(holder), 0);
	return (ft_free(holder), 1);
}

static int	take_input(t_shell *shell)
{
	char	*content;
	t_list	*new;

	if (!shell->last_status)
		shell->welcome_str = shell->success_prompt;
	else
		shell->welcome_str = shell->failed_prompt;
	// shell->input = readline(shell->welcome_str);
	if (isatty(fileno(stdin)))
		shell->input = readline(shell->welcome_str);
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		shell->input = ft_strtrim(line, "\n");
		free(line);
	}
	if (!shell->input)
		return (1);
	if (*shell->input)
	{	
		content = ft_strdup(shell->input);
		if (!content)
			return (-1);
		new = ft_lstnew(content);
		if (!new)
			return (ft_free(content), -1);
		if (!shell->hist_head)
			shell->hist_head = new;
		else
			ft_lstadd_back(&(shell->hist_head), new);
		return (0);
	}
	return (ft_free(shell->input), -1);
}


static int	init_shell(t_shell *shell)
{
	char	*new_level;

	shell->cmd_head = NULL;
	shell->hist_head = NULL;
	shell->input = NULL;
	shell->commands = NULL;
	shell->cmd_pos_head = NULL;
	shell->sucess = 1;
	if (!init_prompt_title(shell))
		return (0);
	shell->welcome_str = shell->success_prompt;
	shell->path = get_final_path();
	shell->builtins = \
	ft_split("echo cd pwd export unset env exit history", ' ');
	if (!shell->builtins || !shell->welcome_str)
		return (0);
	shell->last_status = 0;
	if (!init_shellenv(shell))
		return (0);
	if (!getenv("SHLVL"))
	{
		ft_putenv(shell, ft_strdup("SHLVL=1"));
		new_level = ft_strdup("1");
	}
	else
		new_level = ft_itoa(ft_atoi(getenv("SHLVL")) + 1);
	if (!new_level)
		return (0);
	return (update_env_item(shell, "SHLVL", new_level), ft_free(new_level), 1);
}

static	void	init_shell_for_next_read(t_shell *shell)
{
	shell->cmd_head = NULL;
	shell->hist_head = NULL;
	shell->input = NULL;
	shell->commands = NULL;
	shell->cmd_pos_head = NULL;
	shell->sucess = 1;
	shell->path = get_final_path();
}

void	printf_welcome(void)
{
	ft_printf_fd(1, "*************************************************************************\n");
	ft_printf_fd(1, "\t\t\t🌟Welcome to 42 MiniShell🌟\n");
	ft_printf_fd(1, "\t\t    Developed by: @ofadahun & @sbhatta\n");
	if (getenv("SHLVL"))
		ft_printf_fd(1, "\t\t\t💡Current Shell Level: %s\n", getenv("SHLVL"));
	if (getenv("USER"))
		ft_printf_fd(1, "\t\t\t 💁Current User: %s\n", getenv("USER"));
	else
		ft_printf_fd(1, "\t\t\t 💁Current User: %s\n", "guest");
	ft_printf_fd(1, "*************************************************************************\n");
}

int	main(void)
{
	t_shell			*shell;
	int				status;
	int				input_sucess;

	status = 0;
	ignore_signal();
	set_signal_act();
	// atexit(&checkleaks);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		exit(12);
	init_shell(shell);
	while (1)
	{
		input_sucess = take_input(shell);
		if (input_sucess == 1)
			exit(shell->last_status);
		else if (input_sucess == -1)
			continue ;
		add_history(shell->input);
		if (parse_shell(shell))
			run_commands(shell);
		status = shell->last_status;
		ft_free_for_next_read(shell);
		init_shell_for_next_read(shell);
	}
	return (ft_free_shell(shell), status);
}
