/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 14:17:19 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/20 19:01:13 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void make_string(char *old_post_dollar, char *new_post_dollar, char *not_needed)
{
	int	i;
	int	j;
	
	j = 0;
	i = -1;
	while (old_post_dollar[++i])
	{
		if (ft_strchr(not_needed, old_post_dollar[i]) && !old_post_dollar[i + 1])
			continue;
		else if (ft_strchr(not_needed, old_post_dollar[i]))
		{
			if (ft_strchr(not_needed, old_post_dollar[i]))
				new_post_dollar[j++] = old_post_dollar[i++];
			continue ;
		}
		else
			new_post_dollar[j++] = old_post_dollar[i];
	}
	new_post_dollar[j] = '\0';
}

static char	*make_new_postdollar(char *old_post_dollar)
{
	char	*not_needed;
	char	*new_post_dollar;
	int		count;
	int		i;

	not_needed = "\\";
	i = 0;
	count = 0;
	if (!old_post_dollar)
		return (NULL);
	while (old_post_dollar[i])
	{
		if (ft_strchr(not_needed, old_post_dollar[i]))
			count++;
		i++;
	}
	new_post_dollar = malloc(ft_strlen(old_post_dollar) - count + 1);
	if (!new_post_dollar)
		return (NULL);
	make_string(old_post_dollar, new_post_dollar, not_needed);
	return (ft_free(old_post_dollar), new_post_dollar);
}

char	*make_new_predollar(char *old_pre_dollar, int *is_even)
{
	char	*temp;
	char	*pre_dollar;
	int		i;

	i = 0;
	while (old_pre_dollar[i])
	{
		if (!ft_strchr("\\", old_pre_dollar[i]))
			break ;
		i++;
	}
	if (i % 2)
		*is_even = 0;
	if (i >= 4)
	{
		temp = malloc((i / 4) + 1);
		if (!temp)
			return (ft_free(old_pre_dollar), NULL);
		ft_strlcpy(temp, old_pre_dollar, (i / 4) + 1);
		pre_dollar = ft_strjoin(temp, old_pre_dollar + i);
		return (ft_free(temp), ft_free(old_pre_dollar), pre_dollar);
	}
	pre_dollar = ft_strdup(old_pre_dollar + i);
	return (ft_free(old_pre_dollar), pre_dollar);
}

// static void	sub_expand_dollar()
// {
	
// }

static int	expand_dollar(char	*found_dollar, char *pre_dollar, int i, t_shell *shell)
{
	char	*temp;
	char	*var;
	char	*temp_var;
	char	*value;
	char	*post_dollar;
	int		is_even;
	int		count;

	temp = found_dollar + 1;
	count = 0;
	is_even = 1;
	while (ft_isalnum(temp[count]) || ft_strchr("_", temp[count]))
		count++;
	temp_var = ft_substr(found_dollar, 1, count);
	if (!temp_var)
		return (0);
	post_dollar = make_new_postdollar(ft_strdup(temp + count));
	pre_dollar = make_new_predollar(pre_dollar, &is_even);
	if (!is_even)
		var = ft_strjoin("$", temp_var);
	else
		var = ft_strdup(temp_var);
	ft_free(temp_var);
	printf("var == %s\n", var);
	if (is_even && compare_cmd("?", var))
		value = ft_itoa(shell->last_status);
	else if (is_even)
		value = getenv(var);
	else
		value = var;
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	if (!post_dollar || !value || !pre_dollar)
		return (ft_free(post_dollar), ft_free(value), ft_free(pre_dollar), ft_free(var), 0);
	temp = ft_strjoin(pre_dollar, value);
	if (!temp)
		return (ft_free(post_dollar), ft_free(value), ft_free(pre_dollar), ft_free(var), 0);
	ft_free(shell->cmd_head->toks[i]);
	shell->cmd_head->toks[i] = ft_strjoin(temp, post_dollar);
	if (!shell->cmd_head->toks[i])
		return (ft_free(temp), ft_free(post_dollar), ft_free(value), ft_free(pre_dollar), ft_free(var), 0);
	return (ft_free(temp), ft_free(post_dollar), ft_free(value), ft_free(pre_dollar), ft_free(var), 1);
}

int	search_dollar_in_tok(t_shell *shell)
{
	int		i;
	char	*found_dollar;
	char	*pre_dollar;

	i = 0;
	pre_dollar = NULL;
	while (shell->cmd_head->toks[i])
	{
		found_dollar = ft_strchr(shell->cmd_head->toks[i], '$');
		if (found_dollar)
		{
			pre_dollar = malloc(found_dollar - shell->cmd_head->toks[i] + 1);
			if (!pre_dollar)
				return (0);
			ft_strlcpy(pre_dollar, shell->cmd_head->toks[i], found_dollar - shell->cmd_head->toks[i] + 1);
			if (!expand_dollar(found_dollar, pre_dollar, i, shell))
				return (0);
		}
		i++;
	}
	return (1);
}

//echo $USER\\jj\\\\
//need to handle _
//Ask if we need to handle different symbols in echo $ and export as exactly as bash does
//need to handle echo $ USER