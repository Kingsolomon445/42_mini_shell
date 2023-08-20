/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:38:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/17 18:12:24 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	make_cmds(char **commands, const char *input, int start, int end)
{
	*commands = ft_substr(input, start, end - start);
	if (!*commands)
		return (0);
	commands[1] = NULL;
	return (1);
}

static char	**ft_split_commands(const char *input, int cmd_no, t_shell *shell)
{
	int				cmd_index;
	t_cmd_pos		*cmd_pos;
	char			**commands;
	int				start;

	if (!input)
		return (NULL);
	cmd_index = 0;
	commands = (char **)malloc((cmd_no + 1) * sizeof(char *));
	if (!commands)
		return (NULL);
	cmd_pos = shell->cmd_pos_head;
	start = 0;
	while (cmd_pos)
	{
		if (!make_cmds(commands + cmd_index++, input, start, cmd_pos->index))
			return (ft_free_split(commands), NULL);
		start = cmd_pos->index + 1;
		cmd_pos = cmd_pos->next;
	}
	if (ft_strlen(input) > (size_t)start)
		if (!make_cmds(commands + cmd_index++, input, start, ft_strlen(input)))
			return (ft_free_split(commands), NULL);
	return (commands);
}

static t_split_validate_vars	*init_split_and_validate_vars(char *input)
{
	t_split_validate_vars	*vars;

	vars = malloc(sizeof(t_split_validate_vars));
	if (!vars)
		return (NULL);
	vars->in_s_quotes = 0;
	vars->in_d_quotes = 0;
	vars->pipe_count = 0;
	vars->cmd_pos = NULL;
	vars->parse_pipe_res = 1;
	vars->i = 0;
	vars->input = ft_strdup(input);
	if (!vars->input)
		return (ft_free(vars), NULL);
	return (vars);
}

static int	split_and_validate(t_shell *shell)
{
	t_split_validate_vars	*vars;

	vars = init_split_and_validate_vars(shell->input);
	if (!vars)
		return (0);
	while (vars->input[vars->i])
	{
		validate_input(shell, vars);
		if (vars->parse_pipe_res < 1)
			return (ft_free(vars->input), ft_free(vars), 0);
		else if (vars->parse_pipe_res == 2)
			break ;
		vars->i++;
	}
	if ((vars->in_s_quotes || vars->in_d_quotes))
		return (ft_printf_fd(2, \
		"syntax error near unexpected token `%s'\n", vars->input), \
		ft_free(vars->input), ft_free(vars), set_status(shell, 258), 0);
	shell->commands = ft_split_commands(vars->input, \
	vars->pipe_count + 1, shell);
	return (ft_free(vars->input), ft_free(vars), 1);
}

int	parse_shell(t_shell *shell)
{
	char			**commands;
	t_commands		*cmd_head;
	t_commands		*cmd;
	int				i;

	i = 0;
	cmd_head = NULL;
	if (!split_and_validate(shell))
		return (0);
	if (!shell->commands)
		return (0);
	commands = shell->commands;
	shell->no_cmds = 0;
	while (commands[i])
	{
		shell->no_cmds++;
		cmd = parse_commands(shell, commands[i]);
		if (!cmd)
			return (0);
		ft_lstadd_back_cmd(&cmd_head, cmd);
		i++;
	}
	shell->cmd_head = cmd_head;
	return (1);
}
