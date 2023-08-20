/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:23:47 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 16:31:49 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_and_expand(\
t_shell *shell, t_parse_vars *vars, char *str)
{
	if (ft_strchr("\"'", str[vars->i]))
	{
		if (!parse_quotes(shell, vars, str))
			return (0);
	}
	else if (str[vars->i] == '$' && !(shell->do_not_expand))
	{
		if (!parse_dollar(shell, vars, str))
			return (0);
	}
	else
	{
		if (str[vars->i] == '\\')
			vars->i++;
		vars->new_str[vars->j++] = str[vars->i++];
	}
	return (1);
}

char	*parse_heredoc_input(t_shell *shell, char *input)
{
	t_parse_vars	*vars;
	char			*new_input;

	vars = init_parse_vars();
	if (!vars)
		return (NULL);
	while (input[vars->i])
	{
		if (input[vars->i] == '$' && !shell->do_not_expand)
		{
			if (!parse_dollar(shell, vars, input))
				return (ft_free(vars->new_str), ft_free(vars), NULL);
		}
		else
			vars->new_str[vars->j++] = input[vars->i++];
		if (!append_to_new_cmd(vars, 0))
			return (ft_free(vars), NULL);
	}
	if (!append_to_new_cmd(vars, 1))
		return (ft_free(vars), NULL);
	new_input = ft_strdup(vars->new_str);
	return (ft_free(input), ft_free(vars->new_str), ft_free(vars), new_input);
}

static void	get_next_heredoc_signal_init(char **inp, int *status)
{
	*inp = get_prompt("> ");
	*status = 1;
	signal(SIGINT, heredoc_sig_int_handler);
	if (!*inp || rl_eof_found)
		*status = 0 ;
	if (g_ctrlc)
		*status = -1;
}

int	get_next_heredoc_input(t_shell *shell, char *new_delim, int heredoc)
{
	char	*inp;
	char	*parsed_inp;
	int		status;

	get_next_heredoc_signal_init(&inp, &status);
	parsed_inp = NULL;
	if (status == 1)
	{
		parsed_inp = parse_heredoc_input(shell, inp);
		if (!parsed_inp)
			status = -1;
		else if (compare_str(parsed_inp, new_delim))
		{
			ft_free(parsed_inp);
			status = 0;
		}
		else
		{
			ft_printf_fd(heredoc, "%s\n", parsed_inp);
			ft_free(parsed_inp);
			get_next_heredoc_input(shell, new_delim, heredoc);
		}
	}
	return (status);
}
