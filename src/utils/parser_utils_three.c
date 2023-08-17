/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 20:53:57 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/16 20:54:20 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	append_to_new_cmd(t_parse_vars *vars, int last)
{
	if ((!last && vars->j >= vars->size - 1) || last)
	{
		vars->new_str[vars->j] = '\0';
		if (!last)
			vars->size += 30;
		else
			vars->size = ft_strlen(vars->new_str) + 1;
		vars->new_str = ft_realloc(vars->new_str, vars->size);
		if (!vars->new_str)
			return (0);
		vars->j = ft_strlen(vars->new_str);
	}
	return (1);
}
