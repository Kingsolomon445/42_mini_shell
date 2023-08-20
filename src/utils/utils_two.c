/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:19:24 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/17 17:19:31 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_all_fds(t_redir **redir)
{
	t_redir	*cur_redir;

	cur_redir = *redir;
	while (cur_redir)
	{
		close(cur_redir->red_fd);
		cur_redir = cur_redir->next;
	}
}
