/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:49:59 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/16 19:36:03 by sbhatta          ###   ########.fr       */
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
	vars->new_str = malloc(vars->size);
	if (!vars->new_str)
		return (ft_free(vars), NULL);
	return (vars);
}


char    *parse_expand(t_parse_vars *vars, t_tok_pos **tok_pos, char *env_value)
{
    char new_env_value[ft_strlen(env_value)];
    int i;
    int j;
    char    quote;
    t_tok_pos *new_tok_pos;

    i = 0;
    j = 0;
	if (ft_strchr(" \v\t\f\b\n", vars->new_str[vars->j - 1]))
		ft_increment_index(env_value, " \v\t\f\b\n", &i, 1);
    while(env_value[i])
    {
        if (ft_strchr("\"'", env_value[i]))
        {
            quote = env_value[i];
            new_env_value[j++] = env_value[i++];
            while(env_value[i] && env_value[i] != quote)
                new_env_value[j++] = env_value[i++];
            new_env_value[j++] = env_value[i++];
        }
        else if (ft_strchr(" \t\v\f\b\n", env_value[i]))
        {
            new_env_value[j++] = env_value[i++];
            new_tok_pos = ft_lstnew_tokenpos(vars->j + j - 1);
            ft_lstadd_back_tokenpos(tok_pos, new_tok_pos);
            ft_increment_index(env_value, " \v\t\f\b\n", &i, 1);
        }
        else
            new_env_value[j++] = env_value[i++];
    }
	new_env_value[j] = '\0';
    return (ft_free(env_value), ft_strdup(new_env_value));
}

int parse_dollar_extra(t_shell *shell, t_parse_vars *vars, char *str, t_tok_pos **tok_pos)
{
    char    *env_value;

    if (str[vars->i + 1] && (ft_isalnum(str[vars->i + 1]) \
    || ft_strchr("_?$\"'", str[vars->i + 1])))
    {
        vars->i++;
        env_value = expand(shell, str + vars->i, &vars->i);
        env_value = parse_expand(vars, tok_pos, env_value);
        if (!env_value)
            return (0);
        vars->new_str[vars->j] = '\0';
        vars->new_str = ft_join(vars->new_str, env_value);
        if (!vars->new_str)
            return (0);
        vars->j = ft_strlen(vars->new_str);
        vars->size = vars->j;
    }
    else
        vars->new_str[vars->j++] = str[vars->i++];
    return (1);
}

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


int	parse_all(t_shell *shell, t_redir **redir, t_tok_pos **tok_pos, char *cmd, t_parse_vars *vars)
{
	if (ft_strchr("\"'", cmd[vars->i]))
	{
		if (!parse_quotes(shell, vars, cmd))
			return (-1);
	}
	else if (cmd[vars->i] == '$')
	{
		if (!parse_dollar_extra(shell, vars, cmd, tok_pos))
			return (-1);
	}
	else if (ft_strchr("><", cmd[vars->i]))
		return (parse_redir(shell, cmd + vars->i, &vars->i, redir));
	else
		return (parse_backslash(tok_pos, vars, cmd));
	return (1);
}

char	*parse(t_shell *shell, char *cmd, t_tok_pos **tok_pos, t_redir **redir)
{
	t_parse_vars	*vars;
	char			*new_cmd;

	vars = init_parse_vars();
	if (!vars)
		return (NULL);
	while (cmd[vars->i])
	{
		shell->do_not_expand = 0;
		shell->sucess = parse_all(shell, redir, tok_pos, cmd, vars);
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
		return (NULL);
	if (compare_str("", new_cmd) && redir)
	{
		shell->last_status = 0;
		shell->do_not_run = 1;
	}
	return (ft_lstnew_cmd(shell, redir, tok_pos, new_cmd));
}
