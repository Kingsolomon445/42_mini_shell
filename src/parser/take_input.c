/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:51:50 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/20 11:27:02 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	take_input(t_shell *shell)
{
	char	*content;
	t_list	*new;

	shell->input = readline(shell->welcome_str);
	if (!shell->input)
		ft_exit_shell(shell, EXIT_FAILURE);
	if (*shell->input)
	{	
		content = ft_strdup(shell->input);
		if (!content)
			ft_exit_shell(shell, EXIT_FAILURE);
		new = ft_lstnew(content);
		if (!new)
		{
			ft_free(content);
			ft_exit_shell(shell, EXIT_FAILURE);
		}
		if (!shell->hist_head)
			shell->hist_head = new;
		else
			ft_lstadd_back(&(shell->hist_head), new);
		return (0);
	}
	return (ft_free(shell->input), 1);
}

void	print_history(t_shell *shell)
{
	int		index;
	t_list	*temp;

	index = 0;
	if (!shell->hist_head)
		return ;
	temp = shell->hist_head;
	while (temp)
	{
		printf("%5d	%s\n", index + 1, (char *)temp->content);
		temp = temp->next;
		index++;
	}
}
