/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:38:22 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/08 17:42:44 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void expand(char *input, int *i, t_dollar **dollar_head, int dollar_idx)
{
	char	*val;
	char	*value;
	int		len;
	t_dollar	*dollar;

	len = 0;
	while(input[len] && !ft_strchr(" \t\b\v\f\"'", input[len]))
		len++;
	val = ft_substr(input, 0, len);
	value = getenv(val);
	if (value)
		value = ft_strdup(value);
	else
		value = ft_strdup("");
	dollar = ft_lstnew_dollar(dollar_idx, value);
	ft_lstadd_back_dollar(dollar_head, dollar);
	*i += len;
	ft_free(val);
}


static t_dollar	*check_to_expand(t_dollar **dollar_head, int idx)
{
	t_dollar *dollar;

	dollar = *dollar_head;
	while(dollar)
	{
		if (dollar->index == idx)
			return (dollar);
		dollar = dollar->next;
	}
	return (NULL);
}

static void	ft_substitute(char *new_input, t_dollar **dollar_head, t_shell *shell)
{
	t_dollar	*dollar;
	size_t		len;
	char		*found_dollar;
	
	while (*new_input)
	{
		found_dollar = ft_strchr(new_input, '$');
		if (!found_dollar)
		{
			if (!shell->input)
				shell->input = ft_strdup(new_input);
			else
				shell->input = ft_strjoin(shell->input, ft_strdup(new_input));
			break;
		}
		
		len = found_dollar - new_input;
		if (len > 0)
		{
			if (!shell->input)
				shell->input = ft_substr(new_input, 0, len);
			else
				shell->input = ft_strjoin(shell->input, ft_substr(new_input, 0, len));
			new_input += len;
		}
		dollar = check_to_expand(dollar_head, shell->dollar_idx);
		if (dollar)
		{
			if (shell->input)
				shell->input = ft_strjoin(shell->input, dollar->value);
			else
				shell->input = ft_strdup(dollar->value);
			new_input ++;
		}
		else
		{
			found_dollar = ft_strchr(new_input + 1, '$');
			if (!found_dollar)
			{
				if (!shell->input)
					shell->input = ft_strdup(new_input);
				else
					shell->input = ft_strjoin(shell->input, ft_strdup(new_input));
				break;
			}
			len = found_dollar - new_input;
			if (shell->input)
				shell->input = ft_strjoin(shell->input, ft_substr(new_input, 0, len));
			else
				shell->input = ft_substr(new_input, 0, len);
			new_input += len;
		}
		shell->dollar_idx++;
	}
}


void parse_input(t_shell *shell)
{
    char *input;
	char	quote;
    char new_input[ft_strlen(input) + 1];
    int i;
	int j;
	int dollar_idx;

	input = ft_strdup(shell->input);
	i = 0;
	j = 0;
	dollar_idx = 0;
    while (input[i])
	{
        if (input[i] == '\'' || input[i] == '"')
		{
            quote = input[i++];
            while (input[i] && input[i] != quote)
			{
				new_input[j++] = input[i++];
                if (input[i - 1] == '$')
				{
					if (quote == '"')
                    	expand(input + i, &i, &shell->dollar_head, dollar_idx);
                    dollar_idx++;
                }
            }
            i++;
        }
		else if (input[i] == '$')
		{
			new_input[j++] = input[i++];
            expand(input + i, &i, &shell->dollar_head, dollar_idx);
            dollar_idx++;
        }
		else
            new_input[j++] = input[i++];
    }
    new_input[j] = '\0';
    ft_free(shell->input);
    shell->input = NULL;
    shell->dollar_idx = 0;
    ft_free(input);
    ft_substitute(new_input, &shell->dollar_head, shell);
}
