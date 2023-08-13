/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 20:12:06 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/13 21:07:59 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *expand(t_shell *shell, char *input, int *i)
{
	char	*env_title;
	char	*env_value;
	int		len;

	len = 0;
	if (ft_isdigit(input[len]) || ft_strchr("?$", input[len]))
		len++;
	else if (!ft_strchr("\"'", input[len]))
	{
		while(input[len] && (ft_isalnum(input[len]) || ft_strchr("_", input[len])))
			len++;
	}
	*i += len;
	env_title = ft_substr(input, 0, len);
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
	return (ft_free(env_title), env_value);
}

