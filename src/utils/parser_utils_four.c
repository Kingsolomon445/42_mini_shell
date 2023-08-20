/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_four.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 16:11:09 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 16:47:57 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*parse_file_or_delimeter(t_shell *shell, char *str, int is_file)
{
	t_parse_vars	*vars;
	char			*new_str;

	if (!str)
		return (NULL);
	vars = init_parse_vars();
	if (!vars)
		return (ft_free(str), NULL);
	if (!is_file && (ft_strchr(str, '\'') || ft_strchr(str, '"')))
		shell->do_not_expand = 1;
	while (str[vars->i])
	{
		if (!parse_and_expand(shell, vars, str))
			return (ft_free(str), ft_free(vars->new_str), \
			ft_free(vars), NULL);
		if (!append_to_new_cmd(vars, 0))
			return (ft_free(str), ft_free(vars->new_str), \
			ft_free(vars), NULL);
	}
	if (!append_to_new_cmd(vars, 1))
		return (ft_free(str), ft_free(vars->new_str), \
		ft_free(vars), NULL);
	new_str = ft_strdup(vars->new_str);
	return (ft_free(str), ft_free(vars->new_str), ft_free(vars), new_str);
}
