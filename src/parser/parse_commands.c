/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:46:54 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/07 17:50:35 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	get_word_count(char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while ((*(++s) != c) && *s)
				;
		}
		else
			s++;
	}
	return (count);
}

static int	make_command(char **res, char *start, \
int letter_count, int word_index)
{
    char    *new_cmd;
    char    *trimmed_cmd;

    new_cmd = malloc(letter_count + 1);
	res[word_index] = NULL;
	if (!new_cmd)
        return (ft_free_split(res), 0);
	ft_strlcpy(new_cmd, start, letter_count + 1);
    trimmed_cmd = ft_strtrim(new_cmd, " \n\t\b\v\f");
    if (!*trimmed_cmd)
        return (ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n"), ft_free(new_cmd), ft_free_split(res), 0);
    res[word_index] = trimmed_cmd;
	return (ft_free(new_cmd), 1);
}

char	**ft_split_commands(char const *s, char c, t_shell *shell)
{
	char	*str;
	char	**res;
	char	*start;
	int		letter_count;
	int		word_index;
    int     s_quotes;
	int		d_quotes;
    int     pipes;

	if (!s)
		return (NULL);
	str = (char *)s;
	word_index = 0;
    s_quotes = 0;
	d_quotes = 0;
    pipes = 0;
	res = (char **)malloc((get_word_count(str, c) + 1) * sizeof(char *));
	if (!res)
        return (set_status(shell, 12), NULL);
	while (*str)
	{
		letter_count = 0;
		start = str;
		while (*str && (*str != c || (s_quotes % 2) || (d_quotes % 2)))
		{
            s_quotes += (*str == '\'');
			d_quotes += (*str == '"');
			letter_count += (*str != c || (s_quotes % 2) || (d_quotes % 2));
			str++;
		}

        if (letter_count == 0 && *str == c)
		{
            ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n");
            set_status(shell, 258);
            ft_free_split(res);
            return NULL;
        }

		if (letter_count > 0)
		{
            pipes += (*str == c);
            str += (*str == c);
			if (!make_command(res, start, letter_count, word_index))
			{
                set_status(shell, 12);
                return NULL;
            }
            word_index++;
		}

		if (*str)
			str++;
	}
	*(res + word_index) = NULL;
    if (count_commands(res) != pipes + 1 && pipes)
    {
        *(res + word_index) = readline("> ");
        *(res + word_index + 1) = NULL;
    }
	return (res);
}
