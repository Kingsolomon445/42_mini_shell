/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:18:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/19 13:10:43 by ofadahun         ###   ########.fr       */
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

void	init_shellenv(t_shell *shell)
{
	int				i;
	extern char		**environ;

	i = 0;
	shell->env = (char **)malloc((env_len(environ) + 1) * sizeof(char *));
	if (!shell->env)
		return ;
	while (environ[i])
	{
		shell->env[i] = ft_strdup(environ[i]);
		if (!shell->env[i])
			return ;
		i++;
	}
	shell->env[i] = NULL;
}

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_printf_fd(1, "%s\n", env[i]);
		i++;
	}
	i = 0;
}

void	add_back_str_array(t_shell *shell, char *new_env)
{
	extern char		**environ;
	int				array_len;
	char			**result;
	int				i;

	i = 0;
	array_len = env_len(shell->env);
	result = (char **)malloc((array_len + 2) * sizeof(char *));
	if (!result)
		return ;
	while (shell->env[i])
	{
		result[i] = shell->env[i];
		i++;
	}
	result[i] = new_env;
	result[++i] = NULL;
	ft_free(environ);
	environ = result;
	shell->env = environ;
}

void	export_env(t_shell *shell)
{
	char			*equalsto;
	char			*temp;
	char			*new_tmp;
	char			*env_title;
	int				i;

	i = 0;
	equalsto = ft_strchr(shell->input, '=');
	if (*(equalsto - 1) == ' ')
	{
		ft_printf_fd(1, "minishell: export: `=': not a valid identifier\n");
		shell->last_status = 1;
		return ;
	}
	if (equalsto)
	{
		new_tmp = ft_strtrim(shell->input, " \t\r\f\v");
		temp = ft_strtrim(new_tmp + 7, " \t\r\f\v");
		ft_free(new_tmp);
		if (!ft_isalpha(*temp))
		{
			ft_printf_fd(1, "minishell: export: `%s': not a valid identifier\n", temp);
			shell->last_status = 1;
			ft_free(temp);
			return ;
		}
		equalsto = ft_strchr(temp, '=');
		env_title = ft_substr(temp, 0, equalsto - temp);
		if (*(equalsto + 1) == ' ')
		{
			ft_free(temp);
			temp = ft_strjoin(env_title, "= ");
		}
		if (getenv(env_title))
		{
			while (!ft_strnstr(shell->env[i], env_title, \
			ft_strlen(shell->env[i])) && shell->env[i])
				i++;
			if (i) // why?
			{
				free(shell->env[i]);
				shell->env[i] = ft_strdup(temp);
			}
			else
				add_back_str_array(shell, temp);
		}
		else
			add_back_str_array(shell, temp);
		ft_free(temp);
		ft_free(env_title);
	}
	shell->last_status = 0;
}

//issues: need to handle spaces infront and after = : DONE
//issue2: need to handle _