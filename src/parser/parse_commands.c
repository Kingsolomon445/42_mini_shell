/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:59 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 20:36:12 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_substitute(char *new_command, t_dollar **dollar_head)
{
	t_dollar	*dollar;
	size_t		len;
	char		*found_dollar;
	char		*final_command;
	int			dollar_idx;
	
	final_command = NULL;
	dollar_idx = 0;
	while (*new_command)
	{
		found_dollar = ft_strchr(new_command, '$');
		if (!found_dollar)
		{
			if (!final_command)
				final_command = ft_strdup(new_command);
			else
				final_command = ft_strjoin(final_command, ft_strdup(new_command));
			break;
		}
		
		len = found_dollar - new_command;
		if (len > 0)
		{
			if (!final_command)
				final_command = ft_substr(new_command, 0, len);
			else
				final_command = ft_strjoin(final_command, ft_substr(new_command, 0, len));
			new_command += len;
		}
		dollar = check_to_expand(dollar_head, dollar_idx);
		if (dollar)
		{
			if (final_command)
				final_command = ft_strjoin(final_command, dollar->value);
			else
				final_command = ft_strdup(dollar->value);
			new_command ++;
		}
		else
		{
			found_dollar = ft_strchr(new_command + 1, '$');
			if (!found_dollar)
			{
				if (!final_command)
					final_command = ft_strdup(new_command);
				else
					final_command = ft_strjoin(final_command, ft_strdup(new_command));
				break;
			}
			len = found_dollar - new_command;
			if (final_command)
				final_command = ft_strjoin(final_command, ft_substr(new_command, 0, len));
			else
				final_command = ft_substr(new_command, 0, len);
			new_command += len;
		}
		dollar_idx++;
	}
	return (final_command);
}

t_commands *parse_commands(t_shell *shell, char *old_command)
{
    char *command;
	char	quote;
    char new_command[ft_strlen(old_command) + 1];
    int i;
	int j;
	int dollar_idx;
	t_token_pos *token_pos;
	t_redirection	*redirection;
	t_dollar	*dollar;
	t_commands	*cmd;

	command = ft_strtrim(old_command, " \v\f\t\n\b");
	dollar = NULL;
	i = 0;
	j = 0;
	dollar_idx = 0;
	redirection = NULL;
	token_pos = NULL;
    while (command[i])
	{
        if (command[i] == '\'' || command[i] == '"')
		{
            quote = command[i++];
            while (command[i] && command[i] != quote)
			{
				new_command[j++] = command[i++];
                if (command[i - 1] == '$')
				{
					if (quote == '"' && command[i] && command[i] != '"' && (ft_isalnum(command[i]) || ft_strchr("_?'", command[i])))
                    	expand(shell, command + i, &i, &dollar, dollar_idx);
                    dollar_idx++;
                }
            }
			if (command[i])
            	i++;
        }
		else if (command[i] == '$')
		{
			new_command[j++] = command[i++];
			if (command[i] && (ft_isalnum(command[i]) || ft_strchr("_?\"'", command[i])))
            	expand(shell, command + i, &i, &dollar, dollar_idx);
            dollar_idx++;
        }
		else if (ft_strchr("><", command[i]))
		{
			parse_redirection(&redirection, &i, command + i);
		}
		else
		{
            new_command[j++] = command[i++];
			while (command[i] && ft_strchr(" \v\t\f\b\n", command[i]) && ft_strchr(" \v\t\f\b\n", new_command[j - 1]))
			{
				i++;
			}
			// if (ft_strchr(" \v\t\f\b\n", new_command[j - 1]) && ft_setsinstr(" \v\t\f\b\n", command + i))
			// {
			// 	ft_lstadd_back_tokenpos(&token_pos, ft_lstnew_tokenpos(j - 1));
			// }
			if (ft_strchr(" \v\t\f\b\n", new_command[j - 1]))
			{
				ft_lstadd_back_tokenpos(&token_pos, ft_lstnew_tokenpos(j - 1));
			}
			
		}
    }
    new_command[j] = '\0';
	if (ft_strchr(new_command, '$'))
    	command = ft_substitute(new_command, &dollar);
	else
		command = ft_strdup(new_command);
	cmd = ft_lstnew_cmd(shell, dollar, redirection, token_pos, command);
	return (cmd);
}