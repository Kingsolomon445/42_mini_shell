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
	{
		len++;
	}
	val = ft_substr(input + 1, 0, len - 1);
	value = ft_strdup(getenv(val));
	dollar = ft_lstnew_dollar(dollar_idx, value);
	if (!dollar_head)
		*dollar_head = dollar;
	else
		ft_lstadd_back_dollar(dollar_head, dollar);
	*i += len;
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

static void	ft_substitute(char	*new_input, t_dollar **dollar_head, t_shell *shell)
{
	char	*found_dollar;
	t_dollar	*dollar;
	int		len;
	
	found_dollar = ft_strchr(new_input, '$');
	if (found_dollar)
	{
		len = found_dollar - new_input;
		if (!shell->input)
			shell->input = ft_substr(new_input, 0, len);
		else
			shell->input = ft_strjoin(shell->input, ft_substr(new_input, 0, len));
		new_input += len;
		dollar = check_to_expand(dollar_head, shell->dollar_idx);
		if (dollar)
		{
			// printf("dollar value == %s, dollar idx == %d, idx == %d\n", dollar->value, dollar->index, shell->dollar_idx);
			// printf("i found an expandable dollar\n");
			if(shell->input)
			{
				shell->input = ft_strjoin(shell->input, dollar->value);
			}
			else
				shell->input = ft_strdup(dollar->value);
			new_input++;
		}
		else
		{
			found_dollar = ft_strchr(new_input + 1, '$');
			if (found_dollar)
			{
				len = found_dollar - new_input;
				if (shell->input)
				{
					shell->input = ft_strjoin(shell->input, ft_substr(new_input, 0, len));
				}
				else
					shell->input = ft_substr(new_input, 0, len);
				new_input += len - 1;
			}
			else
			{
				if (!shell->input)
					shell->input = ft_strdup(new_input);
				else
					shell->input = ft_strjoin(shell->input, ft_strdup(new_input));
				new_input = NULL;
			}
		}
		shell->dollar_idx++;
	}
	else
	{
		if (!shell->input)
			shell->input = ft_strdup(new_input);
		else
			shell->input = ft_strjoin(shell->input, ft_strdup(new_input));
		new_input = NULL;
	}
	printf("new input here == %s   , final input here == %s\n", new_input, shell->input);
	if (new_input && *new_input)
		ft_substitute(new_input, dollar_head, shell);
}

void	parse_input(t_shell *shell)
{
	int	i;
	int	j;
	char	new_input[ft_strlen(shell->input)];
	char	*input;
	int		s_quote;
	int		d_quote;
	int		dollar_idx;
	
	i = 0;
	j = 0;
	s_quote = 0;
	d_quote = 0;
	dollar_idx = 0;
	input = ft_strdup(shell->input);
	while(input[i])
	{
		s_quote = 0;
		d_quote = 0;
		if (input[i] == '\'')
			s_quote = 1;
		if (input[i] == '"')
			d_quote = 1;
		if (s_quote)
		{
			i++;
			while(input[i] && input[i] != '\'')
			{
				if (input[i] =='$')
					dollar_idx++;
				new_input[j++] = input[i++];
			}
			i++;
			continue ;
		}
		if (d_quote)
		{
			i++;
			while(input[i] && input[i] != '"')
			{
				new_input[j] = input[i];
				if (input[i] == '$')
				{
					expand(input + i, &i, &shell->dollar_head, dollar_idx);
					dollar_idx++;
				}
				else
					i++;
				j++;
			}
			i++;
			continue ;
		}
		if (input[i])
			new_input[j] = input[i];
		else
			break ;
		if (input[i] == '$')
		{
			expand(input + i,  &i, &shell->dollar_head, dollar_idx);
			dollar_idx++;
		}
		else
			i++;
		j++;
	}
	new_input[j] = '\0';
	ft_free(shell->input);
	shell->input = NULL;
	shell->dollar_idx = 0;
	ft_free(input);
	ft_substitute(new_input, &shell->dollar_head, shell);
}