/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 17:56:35 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 20:52:30 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int	check_delim(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (ft_strchr("'\"", delim[i]))
			return (1);
		i++;
	}
	return (0);
}

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

char	*parse_delimeter(t_shell *shell, char *delim)
{
	t_parse_vars	*vars;
	char			*new_delim;

	if (!delim)
		return (NULL);
	vars = init_parse_vars();
	if (!vars)
		return (NULL);
	if (check_delim(delim))
		shell->do_not_expand = 1;
	while (delim[vars->i])
	{
		if (!parse_and_expand(shell, vars, delim))
			return (ft_free(delim), ft_free(vars->new_str), \
			ft_free(vars), NULL);
		if (!append_to_new_cmd(vars, 0))
			return (ft_free(delim), ft_free(vars->new_str), \
			ft_free(vars), NULL);
	}
	if (!append_to_new_cmd(vars, 1))
		return (ft_free(delim), ft_free(vars->new_str), \
		ft_free(vars), NULL);
	new_delim = ft_strdup(vars->new_str);
	return (ft_free(delim), ft_free(vars->new_str), ft_free(vars), new_delim);
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

char	*get_promt(void)
{
	char	*line;
	char	*inp;

	if (isatty(fileno(stdin)))
		inp = readline("> ");
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		inp = ft_strtrim(line, "\n");
		ft_free(line);
	}
	return (inp);
}

int	get_next_heredoc_input(t_shell *shell, char *new_delim, int heredoc)
{
	char	*inp;
	char	*parsed_inp;
	int		status;

	status = 1;
	signal(SIGINT, heredoc_sig_int_handler);
	inp = get_promt();
	parsed_inp = NULL;
	if (!inp || rl_eof_found)
		status = 0 ;
	if (g_ctrlc)
		status = -1;
	if (status == 1)
	{
		parsed_inp = parse_heredoc_input(shell, inp);
		if (!parsed_inp)
			status = -1;
		else if (compare_str(parsed_inp, new_delim))
			status = 0;
		else
		{
			ft_printf_fd(heredoc, "%s\n", parsed_inp);
			// ft_free(parsed_inp);
			get_next_heredoc_input(shell, new_delim, heredoc);
		}
	}
	return ( status);
}

int	run_here_doc(t_shell *shell, char *delim)
{
	int		heredoc;
	int		status;
	char	*new_delim;

	heredoc = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
		return (print_error(shell->last_status, \
		NULL, strerror(errno), "here_doc"), -1);
	shell->heredoc_ran = 1;
    new_delim = parse_delimeter(shell, ft_strdup(delim));
	if (!new_delim)
		return (close(heredoc), -1);
	status = get_next_heredoc_input(shell, new_delim, heredoc);
	if (status == -1)
		return (close(heredoc), ft_free(new_delim),  status);
	return (close(heredoc), ft_free(new_delim), open(".here_doc", O_RDONLY));
}

int	parse_heredoc(t_shell *shell, t_redir **redir, \
char	*delimeter, char red_type)
{
	int			fd;
	t_redir		*red;

	if (!delimeter)
		return (0);
	if (compare_str("", delimeter))
	{
		shell->do_not_run = 1;
		return (ft_free(delimeter), \
		set_status(shell, 258), ft_printf_fd(2, SYNTAXERR), 0);
	}
	fd = run_here_doc(shell, delimeter);
	if (fd == -1)
		return (ft_free(delimeter), 0);
	red = ft_lstnew_red(red_type, fd);
	if (!red)
		return (close(fd), ft_free(delimeter), 0);
	ft_lstadd_back_red(redir, red);
	return (ft_free(delimeter), set_status(shell, 0), 1);
}
