/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:18:56 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/06 15:29:02 by sbhatta          ###   ########.fr       */
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
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], env_title, ft_strlen(env_title)))
			return (1);
		i++;
	}
	return (0);
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
	environ = shell->env;
}

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf_fd(1, "%s\n", env[i]);
		i++;
	}
}

void	print_export(void)
{
	extern char	**environ;
	int			i;
	int			j;
	int			equalcount;

	i = 0;
	j = 0;
	equalcount = 0;
	while (environ[i])
	{
		ft_printf_fd(1, "declare -x ");
		while (environ[i][j])
		{
			ft_printf_fd(1, "%c", environ[i][j]);
			if (environ[i][j] == '=' && equalcount == 0)
			{
				equalcount = 1;
				ft_printf_fd(1, "\"");
			}
			if (environ[i][j + 1] == '\0')
			{
				if (equalcount)
					ft_printf_fd(1, "\"\n");
				else
					ft_printf_fd(1, "\n");
			}
			j++;
		}
		equalcount = 0;
		j = 0;
		i++;
	}
}

void	ft_putenv(t_shell *shell, char *new_env)
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
	environ = result;
	shell->env = environ;	
}

void	print_export_failure(char *failed_env_title, int *error, int *fail)
{
	ft_printf_fd(2, "minishell: export: `%s': not a valid identifier\n", \
	failed_env_title);
	*fail = 1;
	*error = 1;
}

void	export_more_envs(t_shell *shell, int i, int *fail)
{
	char	**env_split;
	char	*new_full_env;
	int		error;


	error = 0;
	if (*(shell->cmd_head->toks[i]) == '=')
	{
		print_export_failure(shell->cmd_head->toks[i], &error, fail);
		return ;
	}
	// if (!ft_strchr(shell->cmd_head->toks[i], '=') && shell->cmd_head->toks[i + 1] && *shell->cmd_head->toks[i + 1])
	// {
	// 	print_export_failure(shell->cmd_head->toks[i + 1], &error, fail);
	// }
	new_full_env = ft_strdup(shell->cmd_head->toks[i]);
	env_split = ft_split(new_full_env, '=');
	if (!check_valid_identifier(env_split[0]))
		print_export_failure(env_split[0], &error, fail);
	if (!ft_isalpha(*env_split[0]))
		print_export_failure(new_full_env, &error, fail);
	if (error)
	{
		ft_free_split(env_split);
		ft_free(new_full_env);
		return ;
	}
	if (check_env(shell->env, *env_split))
	{
		if (env_split[1])
			update_env_item(shell, *env_split, env_split[1]);
		else
			update_env_item(shell, *env_split, ft_strdup("\0"));
	}
	else
		ft_putenv(shell, new_full_env);
	ft_free_split(env_split);
}

void	export_env(t_shell *shell)
{
	int	i;
	int	fail;

	i = 1;
	fail = 0;
	if (shell->cmd_head->toks[1] == NULL)
	{
		print_export();
		shell->last_status = 0;
		return ;
	}
	else
	{
		while (shell->cmd_head->toks[i])
		{
			export_more_envs(shell, i, &fail);
			i++;
		}
		if (fail)
			shell->last_status = 1;
		else
			shell->last_status = 0;
	}
}

// void	export_more_envs(t_shell *shell, int i)
// {
// 	char	**env_split;
// 	char	*new_full_env;
// 	int		fail;

// 	if (shell->cmd_head->toks[1] == NULL)
// 	{
// 		print_export();
// 		shell->last_status = 0;
// 		return ;
// 	}
// 	fail = 0;
// 	if (*(shell->cmd_head->toks[i]) == '=')
// 	{
// 		print_export_failure(shell->cmd_head->toks[i], &fail);
// 		shell->last_status = 1;
// 		return ;
// 	}
// 	new_full_env = ft_strdup(shell->cmd_head->toks[i]);
// 	env_split = ft_split(new_full_env, '=');
// 	if (!check_valid_identifier(env_split[0]))
// 		print_export_failure(env_split[0], &fail);
// 	if (!ft_isalpha(*env_split[0]))
// 		print_export_failure(new_full_env, &fail);
// 	if (fail)
// 	{
// 		shell->last_status = 1;
// 		ft_free_split(env_split);
// 		ft_free(new_full_env);
// 		return ;
// 	}
// 	if (check_env(shell->env, *env_split))
// 	{
// 		if (env_split[1])
// 			update_env_item(shell, *env_split, env_split[1]);
// 		else
// 			update_env_item(shell, *env_split, ft_strdup("\0"));
// 	}
// 	else
// 		ft_putenv(shell, new_full_env);
// 	ft_free_split(env_split);
// 	shell->last_status = 0;
// }