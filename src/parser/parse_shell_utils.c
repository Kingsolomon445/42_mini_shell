/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 18:01:55 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/16 20:54:56 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_last_char(char *input, int i)
{
	while(i >= 0)
	{
		if (!ft_strchr(" \n\v\f\b><", input[i]))
			return (1);
		else if (ft_strchr("><", input[i]))
			return (0);
		i--;
	}
	return (1);
}

static int	parse_pipe(t_shell *shell, char **input, int *pipe_count, int i)
{
	char	*temp;

	(*pipe_count)++;
	if (i == 0 || (*input)[i + 1] == '\0' || ((*input)[i - 1] == '|' \
	&& (*input)[i + 1] == '|') || ft_strchr("<", (*input)[i - 1]))
		return (ft_printf_fd(2, PIPESYNTAXERR), set_status(shell, 258), 0);
	if (ft_strchr(">", (*input)[i - 1]))
	{
		if (!check_last_char(*input, i - 2))
			return (ft_printf_fd(2, PIPESYNTAXERR), set_status(shell, 258), 0);
		(*input)[i] = ' ';
		return (3);
	}
	ft_increment_index(*input, " \n\t\v\f\b><", &i, 1);
	if (!((*input)[i]))
		return (ft_printf_fd(2, SYNTAXERR), set_status(shell, 258), 0);
	if ((*input)[i - 1] == '|')
	{
		temp = ft_substr(*input, 0, i - 1);
		if (!temp)
			return (-1);
		ft_free(*input);
		*input = temp;
		return (2);
	}
	return (1);
}

void	validate_input(t_shell *shell, t_split_validate_vars *vars)
{
	if (ft_strchr("'", vars->input[vars->i]) && !vars->in_d_quotes)
		vars->in_s_quotes = !vars->in_s_quotes;
	else if (ft_strchr("\"", vars->input[vars->i]) && !vars->in_s_quotes)
		vars->in_d_quotes = !vars->in_d_quotes;
	else if (vars->input[vars->i] == '|' \
	&& !vars->in_d_quotes && !vars->in_s_quotes)
	{
		vars->parse_pipe_res = \
		parse_pipe(shell, &vars->input, &vars->pipe_count, vars->i);
		if (!vars->parse_pipe_res)
			return ;
		else if (vars->parse_pipe_res >= 2)
			return ;
		vars->cmd_pos = ft_lstnew_cmdpos(vars->i);
		if (!vars->cmd_pos)
		{
			vars->parse_pipe_res = -1;
			return ;
		}
		ft_lstadd_back_cmdpos(&shell->cmd_pos_head, vars->cmd_pos);
	}
	else if (ft_strchr("><", vars->input[vars->i]) && !vars->in_d_quotes && !vars->in_s_quotes)
	{
		vars->i++;
		ft_increment_index(vars->input, " \n\t\v\f\b", &vars->i, 1);
		if (!vars->input[vars->i])
		{
			ft_printf_fd(2, SYNTAXERR);
			set_status(shell, 258);
			vars->parse_pipe_res = 0;
		}
		vars->i--;
	}
}
