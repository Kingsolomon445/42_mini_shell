/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:18:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 18:11:42 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_len(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	check_env(char **env, char *env_title)
{
	int		i;
	char	*new_env_title;

	i = 0;
	new_env_title = ft_strjoin(env_title, "=");
	if (!new_env_title)
		return (-1);
	while (env[i])
	{
		if (ft_strnstr(env[i], new_env_title, ft_strlen(new_env_title)))
			return (ft_free(new_env_title), 1);
		i++;
	}
	ft_free(new_env_title);
	return (0);
}

int	init_shellenv(t_shell *shell)
{
	int				i;
	extern char		**environ;

	i = 0;
	if (!environ)
		return (0);
	shell->env = (char **)malloc((env_len(environ) + 1) * sizeof(char *));
	if (!shell->env)
		return (0);
	while (environ[i])
	{
		shell->env[i] = ft_strdup(environ[i]);
		if (!shell->env[i])
			return (0);
		i++;
	}
	shell->env[i] = NULL;
	environ = shell->env;
	return (1);
}

int	print_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf_fd(1, "%s\n", env[i]);
		i++;
	}
	return (0);
}

int	env_main(t_shell *shell, char **toks)
{
	if (toks[1])
		return (print_error(127, "env", toks[1], NOFILEDIR));
	else
		return (print_env(shell->env));
}
