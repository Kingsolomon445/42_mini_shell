/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:14:58 by ofadahun          #+#    #+#             */
/*   Updated: 2023/08/08 12:55:03 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*eval_red_init(t_shell *shell, t_eval_red_vars *vars, char *cmd_tmp)
{
	if (*cmd_tmp == '<')
		vars->red_type = RED_IN;
	else
		vars->red_type = RED_OUT;
	if (*(cmd_tmp + 1) == '>')
		vars->red_type = RED_APPEND;
	else if (*(cmd_tmp + 1) == '<')
		vars->red_type = HEREDOC;
	vars->cmd_tmpptr = cmd_tmp;
	cmd_tmp = ft_strtrim(cmd_tmp, " ><\t\b\r\v");
	if (!cmd_tmp)
	{
		shell->sucess = 0;
		return (NULL);
	}
	ft_free(vars->cmd_tmpptr);
	if (vars->red_type == HEREDOC)
		vars->fd = extract_delimiter(shell, cmd_tmp, &vars->delim_len);
	else
		vars->fd = \
		extract_file(shell, cmd_tmp, vars->red_type, &vars->fname_len);
	return (cmd_tmp);
}

char	*eval_red(t_shell *shell, t_commands *cmd, char *cmd_tmp)
{
	t_eval_red_vars	*vars;

	vars = malloc(sizeof(t_eval_red_vars));
	if (!vars)
		return (set_status(shell, 12), NULL);
	cmd_tmp = eval_red_init(shell, vars, cmd_tmp);
	if (shell->sucess != 1)
		return (set_status(shell, 12), NULL);
	vars->new_red = ft_lstnew_red(vars->red_type, vars->fd);
	if (!vars->new_red)
	{
		shell->sucess = 0;
		return (set_status(shell, 12), NULL);
	}
	if (!cmd->red)
		cmd->red = vars->new_red;
	else
		ft_lstadd_back_red(&(cmd->red), vars->new_red);
	vars->cmd_tmpptr = cmd_tmp;
	if (vars->red_type == HEREDOC)
		cmd_tmp = ft_strtrim(cmd_tmp + vars->delim_len, " \t\b\r\v");
	else
		cmd_tmp = ft_strtrim(cmd_tmp + vars->fname_len, " \t\b\r\v");
	if (!cmd_tmp)
	{
		shell->sucess = 0;
		return (ft_free(vars->cmd_tmpptr), set_status(shell, 12), NULL);
	}
	return (ft_free(vars->cmd_tmpptr), cmd_tmp);
}

char	*eval_cmd(t_shell *shell, t_commands *cmd, char *cmd_tmp, int *dbl, int *sngl)
{
	char	*command_tmp_ptr;
	char	*to_add;
	int		cmd_length;

	cmd_length = -1;
	while (cmd_tmp[++cmd_length])
	{
		if (cmd_tmp[cmd_length] == '\'')
			(*sngl)++;
		if (cmd_tmp[cmd_length] == '"')
			(*dbl)++;
		if (ft_strchr("<>", cmd_tmp[cmd_length]) && *dbl % 2 == 0 && *sngl == 0)
			break ;
	}
	to_add = ft_substr(cmd_tmp, 0, cmd_length);
	//Need to fix couple fo stuffs
	if (!cmd->cmd_str)
		cmd->cmd_str = ft_strdup(to_add);
	else
	{
		cmd->cmd_str = ft_strjoin(cmd->cmd_str, " ");
		cmd->cmd_str = ft_strjoin(cmd->cmd_str, to_add);
	}
	if (!cmd->cmd_str)
	{
		shell->sucess = 0;;
		return (set_status(shell, 12), NULL);
	}
	command_tmp_ptr = cmd_tmp;
	cmd_tmp = ft_strtrim(cmd_tmp + cmd_length, " \t\b\r\v");
	if (!cmd_tmp)
	{
		shell->sucess = 0;
		return (ft_free(command_tmp_ptr), set_status(shell, 12), NULL);
	}
	return (ft_free(command_tmp_ptr), cmd_tmp);
}

int	parse_redirection(t_shell *shell, t_commands *cmd, char *command)
{
	int		sngl;
	int		dbl;
	char	*cmd_tmp;

	cmd->cmd_str = NULL;
	sngl = 0;
	dbl = 0;
	cmd_tmp = ft_strtrim(command, " \t\b\r\v");
	if (!cmd_tmp)
		return (set_status(shell, 12), 0);
	while (*cmd_tmp)
	{
		shell->sucess = 1;
		if (*cmd_tmp == '\'')
			sngl++;
		if (*cmd_tmp == '"')
			dbl++;
		if ((*cmd_tmp == '<' || *cmd_tmp == '>') && !(dbl % 2) && !(sngl % 2))
			cmd_tmp = eval_red(shell, cmd, cmd_tmp);
		else
			cmd_tmp = eval_cmd(shell, cmd, cmd_tmp, &dbl, &sngl);
		if (shell->sucess != 1)
			return (ft_free(cmd_tmp), shell->sucess);
	}
	if (!cmd->cmd_str)
		cmd->cmd_str = ft_strdup(command);
	if (!cmd->cmd_str)
		return (ft_free(cmd_tmp), 0);
	// printf("my comamnd string here is %s\n", cmd->cmd_str);
	return (ft_free(cmd_tmp), 1);
}