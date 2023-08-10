/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shell_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 20:12:06 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/09 20:20:46 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void expand(t_shell *shell, char *input, int *i, t_dollar **dollar_head, int dollar_idx)
{
	char	*env_title;
	char	*env_value;
	int		len;
	t_dollar	*dollar;

	len = 0;
	if (ft_isdigit(input[len]))
		len++;
	else if (!ft_strchr("\"'", input[len]))
	{
		while(input[len] && ft_isalnum(input[len]))
			len++;
	}
	env_title = ft_substr(input, 0, len);
	if (compare_cmd("?", env_title))
		env_value = ft_itoa(shell->last_status);
	else
	{
		env_value = getenv(env_title);
		if (env_value)
			env_value = ft_strdup(env_value);
		else
			env_value = ft_strdup("");
	}
	dollar = ft_lstnew_dollar(dollar_idx, env_value);
	ft_lstadd_back_dollar(dollar_head, dollar);
	*i += len;
	ft_free(env_title);
}


t_dollar	*check_to_expand(t_dollar **dollar_head, int idx)
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


int	ft_setsinstr(char *sets, char *rem_input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while(sets[i])
	{
		j = 0;
		while(rem_input[j])
		{
			if (sets[i] == rem_input[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}