/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_one.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 14:53:09 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/17 19:21:54 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_env_value(char *env_title, int last_status)
{
	char	*env_value;

	if (compare_str("?", env_title))
		env_value = ft_itoa(last_status);
	else if (compare_str("0", env_title))
		env_value = ft_strdup("minishell");
	else
	{
		env_value = getenv(env_title);
		if (env_value)
			env_value = ft_strdup(env_value);
		else
			env_value = ft_strdup("");
	}
	return (env_value);
}

static int	parse_dir(t_shell *shell, char *current_bin)
{
	struct stat	st;
	DIR			*temp_dir;

	shell->do_not_run = 1;
	temp_dir = opendir(current_bin);
	if (temp_dir)
		return (closedir(temp_dir), set_status(shell, 126), \
		print_error(1, NULL, current_bin, "is a directory"), 1);
	if (stat(current_bin, &st) == 0)
	{
		shell->do_not_run = 0;
		return (1);
	}
	print_error(1, NULL, current_bin, NOFILEDIR);
	return (set_status(shell, 127), 0);
}

char	*expand(t_shell *shell, char *input, int *i)
{
	char	*env_title;
	char	*env_value;
	int		len;

	len = 0;
	if (ft_isdigit((input)[len]) || ft_strchr("?$", (input)[len]))
		len++;
	else if (!ft_strchr("\"'", (input)[len]))
	{
		while ((input)[len] && (ft_isalnum(input[len]) || \
		ft_strchr("_", input[len])))
			len++;
	}
	*i += len;
	env_title = ft_substr((input), 0, len);
	if (!env_title)
		return (NULL);
	env_value = get_env_value(env_title, shell->last_status);
	if (!env_value)
		return (ft_free(env_title), NULL);
	return (ft_free(env_title), env_value);
}

void	ft_increment_index(char *str, char *delims, int *idx, int comp)
{
	while (str[*idx])
	{
		if (comp && !ft_strchr(delims, str[*idx]))
			break ;
		else if (!comp && ft_strchr(delims, str[*idx]))
			break ;
		(*idx)++;
	}
}

char	*get_valid_bin(t_shell *shell, char *cmd)
{
	int			i;
	char		*current_bin;
	char		*new_cmd;
	struct stat	st;

	i = -1;
	if (!shell->path)
		return (ft_strdup(cmd));
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	while (shell->path[++i] && !(ft_strchr(cmd, '/')))
	{
		current_bin = ft_strjoin(shell->path[i], new_cmd);
		if (!current_bin)
			return (free(new_cmd), NULL);
		if (stat(current_bin, &st) == 0)
			return (free(new_cmd), current_bin);
		ft_free(current_bin);
	}
	current_bin = ft_strdup(cmd);
	if (ft_strchr(current_bin, '/'))
		parse_dir(shell, current_bin);
	return (ft_free(new_cmd), current_bin);
}
