/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:59 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/18 17:15:09 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parse_backslash(t_tok_pos **tok_pos, t_parse_vars *vars, char *cmd)
{
	t_tok_pos	*new_tok_pos;

	if (cmd[vars->i] == '\\')
		(vars->i)++;
	if (cmd[vars->i])
	{
		vars->new_str[vars->j++] = cmd[(vars->i)++];
		if (ft_strchr(" \v\t\f\b\n", vars->new_str[vars->j - 1]))
			ft_increment_index(cmd, " \v\t\f\b\n", &vars->i, 1);
		if (ft_strchr("><", cmd[vars->i]) && \
		ft_strchr(" \v\t\f\b\n", vars->new_str[vars->j - 1]))
		{
			vars->new_str[--vars->j] = '\0';
			return (1);
		}
		new_tok_pos = ft_lstnew_tokenpos(vars->j - 1);
		if (!new_tok_pos)
			return (0);
		if (ft_strchr(" \v\t\f\b\n", vars->new_str[vars->j - 1]))
			ft_lstadd_back_tokenpos(tok_pos, new_tok_pos);
		else
			ft_free(new_tok_pos);
	}
	return (1);
}

t_parse_vars	*init_parse_vars(void)
{
	t_parse_vars	*vars;

	vars = malloc(sizeof(t_parse_vars));
	if (!vars)
		return (NULL);
	vars->size = 30;
	vars->i = 0;
	vars->j = 0;
	vars->str = NULL;
	vars->new_str = malloc(vars->size);
	if (!vars->new_str)
		return (ft_free(vars), NULL);
	return (vars);
}

int	parse_all(t_shell *shell, t_redir **redir, t_tok_pos **tok_pos, \
t_parse_vars *vars)
{
	if (ft_strchr("\"'", vars->str[vars->i]))
	{
		if (!parse_quotes(shell, vars, vars->str))
			return (-1);
	}
	else if (vars->str[vars->i] == '$')
	{
		if (!parse_dollar_extra(shell, vars, vars->str, tok_pos))
			return (-1);
	}
	else if (ft_strchr("><", vars->str[vars->i]))
		return (parse_redir(shell, vars->str + vars->i, &vars->i, redir));
	else
		return (parse_backslash(tok_pos, vars, vars->str));
	return (1);
}

char	*parse(t_shell *shell, char *cmd, t_tok_pos **tok_pos, t_redir **redir)
{
	t_parse_vars	*vars;
	char			*new_cmd;

	vars = init_parse_vars();
	if (!vars)
		return (NULL);
	vars->str = cmd;
	while (cmd[vars->i])
	{
		shell->do_not_expand = 0;
		shell->sucess = parse_all(shell, redir, tok_pos, vars);
		if (shell->do_not_run && shell->cmd_cnt > 1)
			break ;
		if (shell->sucess < 1 || (shell->cmd_cnt == 1 && shell->do_not_run))
			return (ft_free(vars->new_str), ft_free(vars), ft_free(cmd), NULL);
		if (!append_to_new_cmd(vars, 0))
			return (ft_free(vars->new_str), ft_free(vars), ft_free(cmd), NULL);
	}
	if (!append_to_new_cmd(vars, 1))
		return (ft_free(vars->new_str), ft_free(vars), ft_free(cmd), NULL);
	new_cmd = ft_strdup(vars->new_str);
	return (ft_free(vars->new_str), ft_free(vars), ft_free(cmd), new_cmd);
}

t_commands	*parse_commands(t_shell *shell, char *old_command)
{
	char		*new_cmd;
	char		*cmd;
	t_tok_pos	*tok_pos;
	t_redir		*redir;

	shell->cmd_cnt = count_commands(shell->commands);
	shell->do_not_run = 0;
	cmd = ft_strtrim(old_command, " \t\n\b\v\f");
	if (!cmd)
		return (NULL);
	tok_pos = NULL;
	redir = NULL;
	new_cmd = parse(shell, cmd, &tok_pos, &redir);
	if (!new_cmd)
		return (ft_free_tokenpos(&tok_pos), NULL);
	if (compare_str("", new_cmd) && redir)
	{
		shell->last_status = 0;
		shell->do_not_run = 1;
	}
	return (ft_lstnew_cmd(shell, redir, tok_pos, new_cmd));
}
