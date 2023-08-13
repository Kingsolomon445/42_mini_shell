/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:59 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 17:43:07 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_parse_commands_vars	*init_parse_command_vars(char *old_command, char *new_command)
{
	t_parse_commands_vars	*vars;
	
	vars = malloc(sizeof(t_parse_commands_vars));
	if (!vars)
		return (NULL);
	vars->command = ft_strtrim(old_command, " \v\f\t\n\b");
	if (!vars->command)
		return (ft_free(vars), NULL);
	vars->new_command = new_command;
	vars->dollar = NULL;
	vars->i = 0;
	vars->j = 0;
	vars->dollar_idx = 0;
	vars->redirection = NULL;
	vars->token_pos = NULL;
	return (vars);
}

static int	check_if_to_make_space(char *command, char *new_command, char quote)
{
	int 	i;

	i = 0;
	while(command[i] == quote)
		i++;
	if (!ft_strchr(" \t\v\b\f\n", command[i]))
		return (0);
	while(command[i])
	{
		if (!ft_strchr(" \t\v\b\f\n", command[i]))
			return (1);
		i++;
	}
	i = 0;
	if (ft_strnstr(new_command, "echo", ft_strlen(new_command)))
	{
		while(new_command[i] && !ft_strchr(" \t\v\b\f\n", new_command[i]))
			i++;
	}
	while(new_command[i] && ft_strchr(" \t\v\b\f\n", new_command[i]))
		i++;
	if (new_command[i])
		return (1);
	return (0);
}


// static void	parse_quotes(char *cmd, char **new_cmd, int *i, int *j)
// {
// 	char	quote;

// 	quote = *cmd;
// 	while (cmd[*i] && cmd[*i] != quote)
// 	{
// 		(*new_cmd)[(*j)++] = cmd[(*i)++];
// 		if (cmd[(*i) - 1] == '$')
// 		{
// 			if (quote == '"' && cmd[(*i)] && cmd[(*i)] != '"' && (ft_isalnum(cmd[(*i)]) || ft_strchr("_?$'", cmd[(*i)])))
// 				expand()
// 		}
// 	}
// }

static void	parse_quotes(t_shell *shell, t_parse_commands_vars *vars)
{
	vars->quote = vars->command[vars->i++];
	vars->new_command[vars->j] = '\0';
	if (vars->command[vars->i] == vars->quote && check_if_to_make_space(vars->command + vars->i + 1, vars->new_command, vars->quote))
	{
		vars->new_command[vars->j++] = ' ';
		vars->new_token_pos = ft_lstnew_tokenpos(vars->j - 1);
		// if (!vars->new_token_pos)
		// 	return (0);
		if (ft_strchr(" \v\t\f\b\n", vars->new_command[vars->j - 1]))
			ft_lstadd_back_tokenpos(&vars->token_pos, vars->new_token_pos);
		while (vars->command[vars->i + 1] == vars->quote)
			vars->i++;
	}
	while (vars->command[vars->i] && vars->command[vars->i] != vars->quote)
	{
		vars->new_command[vars->j++] = vars->command[vars->i++];
		if (vars->command[vars->i - 1] == '$')
		{
			if (vars->quote == '"' && vars->command[vars->i] && vars->command[vars->i] != '"' && (ft_isalnum(vars->command[vars->i]) || ft_strchr("_?$'", vars->command[vars->i])))
				shell->sucess = expand(shell, vars, vars->command + vars->i);
			vars->dollar_idx++;
		}
	}
	if (vars->command[vars->i])
		vars->i++;
}

static int	parse_and_escape_backslash(t_parse_commands_vars *vars)
{
	if (vars->command[vars->i] == '\\')
		vars->i++;
	if (vars->command[vars->i])
	{
		if (vars->command[vars->i] == '$')
			vars->dollar_idx++;
		vars->new_command[vars->j++] = vars->command[vars->i++];
		while (vars->command[vars->i] && ft_strchr(" \v\t\f\b\n", vars->command[vars->i]) && ft_strchr(" \v\t\f\b\n", vars->new_command[vars->j - 1]))
			vars->i++;
		vars->new_token_pos = ft_lstnew_tokenpos(vars->j - 1);
		if (!vars->new_token_pos)
			return (0);
		if (ft_strchr(" \v\t\f\b\n", vars->new_command[vars->j - 1]))
			ft_lstadd_back_tokenpos(&vars->token_pos, vars->new_token_pos);
	}
	return (1);
}

int	count_commands(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
		i++;
	return (i);
}


int	parse(t_shell *shell, t_parse_commands_vars *vars)
{
	while (vars->command[vars->i])
	{
        if (ft_strchr("\"'", vars->command[vars->i]))
			parse_quotes(shell, vars);
		else if (vars->command[vars->i] == '$')
		{
			vars->new_command[vars->j++] = vars->command[vars->i++];
			if (vars->command[vars->i] && (ft_isalnum(vars->command[vars->i]) || ft_strchr("_?$\"'", vars->command[vars->i])))
            	shell->sucess = expand(shell, vars, vars->command + vars->i);
            vars->dollar_idx++;
        }
		else if (ft_strchr("><", vars->command[vars->i]))
			shell->sucess = parse_redirection(shell, vars, vars->command + vars->i);
		else
			shell->sucess = parse_and_escape_backslash(vars);
		if (shell->do_not_run && vars->cmd_cnt > 1)
			break;
		if ((!shell->sucess && vars->cmd_cnt > 1) || shell->do_not_run)
			return (0);
    }
	return (1);
}

t_commands *parse_commands(t_shell *shell, char *old_command)
{
    char new_command[ft_strlen(old_command) + 1];
	t_parse_commands_vars	*vars;

	vars = init_parse_command_vars(old_command, new_command);
	vars->cmd_cnt = count_commands(shell->commands);
	if (!vars)
		return (NULL);
	shell->do_not_run = 0;
	if (!parse(shell, vars))
		return (0);
    vars->new_command[vars->j] = '\0';
	ft_free(vars->command);
	vars->command = ft_strdup(vars->new_command);
	if (compare_str("", vars->command) && vars->redirection)
	{
		shell->last_status = 0;
		shell->do_not_run = 1;
	}
	if (!vars->command)
		return (NULL);
	vars->cmd = ft_lstnew_cmd(shell, vars);
	return (vars->cmd);
}
