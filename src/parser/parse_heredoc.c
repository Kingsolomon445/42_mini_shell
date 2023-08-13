/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 17:56:35 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 14:21:57 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int p(void)
{
	return (1);
}
// void	int_handler(int sig_num)
// {
// 	(void) sig_num;
// 	rl_replace_line("\x03", 0);
// 	rl_done = 1;
// }

static char *expand_dollar(t_shell *shell, char **input)
{
	char	*env_title;
	char	*env_value;
	int		len;

	len = 0;
	if (ft_isdigit((*input)[len]) || ft_strchr("?$", (*input)[len]))
		len++;
	else if (!ft_strchr("\"'", (*input)[len]))
	{
		while((*input)[len] && (ft_isalnum((*input)[len])))
			len++;
	}
	env_title = ft_substr((*input), 0, len);
    // printf("env title = %s\n", env_title);
    (*input) += len;
	if (!env_title)
		return (NULL);
	if (compare_str("?", env_title))
		env_value = ft_itoa(shell->last_status);
	else if(compare_str("0", env_title))
		env_value = ft_strdup("minishell");
	else
	{
		env_value = getenv(env_title);
		if (env_value)
			env_value = ft_strdup(env_value);
		else
			env_value = ft_strdup("");
	}
	if (!env_value)
		return (ft_free(env_title), NULL);
	return (ft_free(env_title), env_value);
}

static char    *add_to_input(char *inp, char *to_add)
{
    char	*final_input;

	if (!to_add)
		return (NULL);
	if (!inp)
		final_input = ft_strdup(to_add);
	else
		final_input = ft_strjoin(inp, to_add);
	ft_free(inp);
	ft_free(to_add);
	return (final_input);
}

static char    *parse_heredoc_limiter(char *delim, int *do_not_expand)
{
    char    new_delim[ft_strlen(delim)];
    char    quote;
    int     i;
    int     j;
    
    i = 0;
    j = 0;
    while(delim[i])
    {
        if (ft_strchr("\"'", delim[i]))
        {
            *do_not_expand = 1;
            quote = delim[i++];
            while(delim[i] && delim[i] != quote)
                new_delim[j++] = delim[i++];
            i++;
        }
        else
            new_delim[j++] = delim[i++];
    }
    ft_free(delim);
    return (ft_strdup(new_delim));
}

char    *parse_heredoc_input(t_shell *shell, char *new_inp, int do_not_expand)
{
    char    *found_dollar;
    char    *final_inp;
    char    *sub;
    int     len;

    final_inp = NULL;
    while(*new_inp)
    {
        found_dollar = ft_strchr(new_inp, '$');
        if (!found_dollar || do_not_expand)
        {
            final_inp = add_to_input(final_inp, ft_strdup(new_inp));
            break ;
        }
        len = found_dollar - new_inp;
        final_inp = add_to_input(final_inp, ft_substr(new_inp, 0, len));
        if (!final_inp)
            return (NULL);
        if (found_dollar[1] && (ft_isalnum(found_dollar[1]) || ft_strchr("_?$\"'", found_dollar[1])))
        {
            new_inp += len + 1;
            sub = expand_dollar(shell, &new_inp);
            if (!sub)
                return (NULL);
            final_inp = add_to_input(final_inp, sub);
            if (!final_inp)
                return (NULL);
        }
        else
            new_inp += len + 1;
    }
    return (final_inp);
}

int	run_here_doc(t_shell *shell, char *delim)
{
	int		heredoc;
    int     do_not_expand;
	char	*inp;
    char    *parsed_inp;

    do_not_expand = 0;
	heredoc = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (heredoc == -1)
	{
		ft_printf_fd(2, "pipex: %s: %s\n", strerror(errno), "here_doc");
		return (-1);
	}
	// signal(SIGINT, int_handler);
	// rl_event_hook = p;
    delim = parse_heredoc_limiter(delim, &do_not_expand);
	// inp = readline("> ");
	if (isatty(fileno(stdin)))
		inp = readline("> ");
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		inp = ft_strtrim(line, "\n");
		free(line);
	}
    parsed_inp = parse_heredoc_input(shell, inp, do_not_expand);
    // printf("parsed input == <-%s->  delimeter == <-%s->\n", parsed_inp, delim);
	while (parsed_inp && !compare_str(parsed_inp, delim) && !compare_str(parsed_inp, "\x03"))
	{
		ft_putstr_fd(parsed_inp, heredoc);
		ft_putstr_fd("\n", heredoc);
		ft_free(inp);
		// inp = readline("> ");
		if (isatty(fileno(stdin)))
			inp = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			inp = ft_strtrim(line, "\n");
			free(line);
		}
        parsed_inp = parse_heredoc_input(shell, inp, do_not_expand);
	}
	ft_free(inp);
	return (open("here_doc", O_RDONLY));
}