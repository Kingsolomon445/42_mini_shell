/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:13:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/09 20:25:26 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// We need to handle an occasion whereby redirection/heredoc doesnt have a file or an argument. ===== done
// We still need a welcome string or welcome ascii art
// Need to handle echo $$
// Need to escape backslash
// Unset needs to handle multiple arguments  ===== done
// need to handle when pipe is inside quoted strings
//ls > tmp/file gives malloc error

//echo ""$?""   ==== done
// echo "exit_code ->$? user ->$USER home -> $HOME"
// echo $"42$"    ==== done
// echo "$ "
// echo hi >./outfiles/outfile01 | echo bye
// echo <123 <456 hi | echo 42
// echo "$?"
// echo "'$?'"
// escape backslash
// echo "cat lol.c | cat > lol.c"
// echo '$USER' "$USER" "text  ' text"
// echo " '$USER' " spaces in quotes
// echo '              $USER          '
// echo $USER$TESTNOTFOUND$HOME$
// echo $USER$TESTNOTFOUND$HOME$WTF$PWD

static int	take_input(t_shell *shell)
{
	char	*content;
	t_list	*new;

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
		ft_exit_shell(shell, EXIT_FAILURE);
	if (*shell->input)
	{	
		content = ft_strdup(shell->input);
		if (!content)
			ft_exit_shell(shell, EXIT_FAILURE);
		new = ft_lstnew(content);
		if (!new)
		{
			ft_free(content);
			ft_exit_shell(shell, EXIT_FAILURE);
		}
		ft_lstadd_back(&(shell->hist_head), new);
		return (0);
	}
	return (ft_free(shell->input), 1);
}

static int	init_shell(t_shell *shell)
{
	char	*new_level;
	int		level;

	shell->cmd_head = NULL;
	shell->hist_head = NULL;
	shell->input = NULL;
	shell->welcome_str = ft_strjoin(getenv("USER"), "@minishell$ ");
	shell->path = get_final_path();
	shell->builtins = \
	ft_split("echo cd pwd export unset env exit history", ' ');
	shell->last_status = 0;
	init_shellenv(shell);
	if (!getenv("SHLVL"))
	{
		level = 1;
		ft_putenv(shell, ft_strdup("SHLVL=1"));
	}
	else
		level = ft_atoi(getenv("SHLVL")) + 1;
	new_level = ft_itoa(level);
	return (update_env_item(shell, "SHLVL", new_level), ft_free(new_level), 1);
}

int	main(void)
{
	t_shell			*shell;

	ignore_signal();
	set_signal_act();
	// atexit(&checkleaks);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (-1);
	shell->red_status = 1;
	init_shell(shell);
	shell->dollar_head = NULL;
	while (1)
	{
		shell->token_pos = NULL;
		if (take_input(shell))
			continue ;
		add_history(shell->input);
		parse_shell(shell);
		run_commands(shell);
		ft_free_for_next_read(shell);
	}
	ft_free_shell(shell);
	return (0);
}
