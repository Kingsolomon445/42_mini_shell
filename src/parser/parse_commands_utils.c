/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 20:12:06 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/12 19:27:13 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int expand(t_shell *shell, t_parse_commands_vars *vars, char *input)
{
	char	*env_title;
	char	*env_value;
	int		len;
	t_dollar	*dollar;

	len = 0;
	if (ft_isdigit(input[len]) || ft_strchr("?$", input[len]))
		len++;
	else if (!ft_strchr("\"'", input[len]))
	{
		while(input[len] && (ft_isalnum(input[len])))
			len++;
	}
	env_title = ft_substr(input, 0, len);
	if (!env_title)
		return (0);
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
		return (ft_free(env_title), 0);
	dollar = ft_lstnew_dollar(vars->dollar_idx, env_value);
	if (!dollar)
		return (ft_free(env_title), ft_free(env_value), 0);
	ft_lstadd_back_dollar(&vars->dollar, dollar);
	vars->i += len;
	return (ft_free(env_title), 1);
}
