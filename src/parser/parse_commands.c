/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 20:25:46 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/14 12:59:20 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redirection *ft_lstnew_red(int red_type, int fd)
{
	t_redirection *new_red;

	new_red = malloc(sizeof(t_redirection));
	if (!new_red)
		return (NULL);
	new_red->red_type = red_type;
	new_red->red_fd = fd;
	new_red->next = NULL;
	return (new_red);
}

void	ft_lstadd_back_red(t_redirection **red_head, t_redirection *new_red)
{
	t_redirection	*current_red;

	if (!new_red)
		return ;
	if (!(*red_head))
	{
		*red_head = new_red;
		return ;
	}
	current_red = *red_head;
	while (current_red->next)
		current_red = current_red->next;
	current_red->next = new_red;
}

int parse_redirection(t_commands *cmd, char *command)
{
    int cmd_length;
    int red_type;
    char *file_name;
	int	fd;
	int	file_name_length;
	t_redirection	*new_red;
	char	*command_tmp;
	char	*command_tmp_ptr;

    cmd_length = 0;
	cmd->cmd_str = NULL;
	command_tmp = ft_strtrim(command, " \t\b\r\v");
    while (*command_tmp)
    {
        if (*command_tmp == '<' || *command_tmp == '>')
        {
            if (*command_tmp == '<')
                red_type = RED_IN;
            else
                red_type = RED_OUT;
            if (*(command_tmp + 1) == '>')
                red_type = RED_APPEND;
            command_tmp_ptr = command_tmp;
			command_tmp = ft_strtrim(command_tmp, " ><\t\b\r\v");
			ft_free(command_tmp_ptr);
            file_name_length = 0;
            while (command_tmp[file_name_length] && !ft_strchr(" ><\t\b\r\v", command_tmp[file_name_length]))
                file_name_length++;
            file_name = ft_substr(command_tmp, 0, file_name_length);
            if (!file_name)
                return 0;
            if (red_type == RED_IN)
                fd = open(file_name, O_RDONLY);
            else if (red_type == RED_OUT)
                fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (red_type == RED_APPEND)
                fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            ft_free(file_name);
            if (fd == -1)
                return 0;
            new_red = ft_lstnew_red(red_type, fd);
            if (!new_red)
                return 0;
            if (!cmd->red)
                cmd->red = new_red;
            else
                ft_lstadd_back_red(&(cmd->red), new_red);
			command_tmp_ptr = command_tmp;
			command_tmp = ft_strtrim(command_tmp + file_name_length, " \t\b\r\v");
			ft_free(command_tmp_ptr);
        }
        else
        {
            while (command_tmp[cmd_length] && !ft_strchr("<>", command_tmp[cmd_length]))
                cmd_length++;
            cmd->cmd_str = ft_substr(command_tmp, 0, cmd_length);
            if (!cmd->cmd_str)
                return 0;
			command_tmp_ptr = command_tmp;
			command_tmp = ft_strtrim(command_tmp + cmd_length, " \t\b\r\v");
			ft_free(command_tmp_ptr);
        }
    }
	ft_free(command_tmp);
	if (!cmd->cmd_str)
    	cmd->cmd_str = ft_strdup(command);
    if (!cmd->cmd_str)
        return 0;
    return 1;
}

char	*get_valid_bin(t_shell *shell, char *cmd)
{
	int			i;
	char		*current_bin;
	char		*new_cmd;
	struct stat	st;

	i = -1;
	new_cmd = ft_strjoin("/", cmd);
	if (!new_cmd)
		return (NULL);
	while (shell->path[++i])
	{
		current_bin = ft_strjoin(shell->path[i], new_cmd);
		if (!current_bin)
			return (free(new_cmd), NULL);
		if (stat(current_bin, &st) == 0)
			return (free(new_cmd), current_bin);
		ft_free(current_bin);
	}
	current_bin = ft_strdup(cmd);
	return (ft_free(new_cmd), current_bin);
}

t_commands	*ft_lstnew_cmd(char *command, t_shell *shell, int cmd_pos)
{
	t_commands	*cmd;

	cmd = malloc(sizeof(t_commands));
	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	cmd->red = NULL;
	if (!parse_redirection(cmd, command))
		return (NULL);
	if (!cmd->cmd_str)
		return (NULL);
	cmd->toks = ft_strtok(cmd->cmd_str, " \n\r\f\v\t");
	if (!cmd->toks)
		return (NULL);
	if (!cmd->toks)
		return (NULL);
	cmd->vbin = get_valid_bin(shell, cmd->toks[0]);
	if (!cmd->vbin)
		return (NULL);
	cmd->cmd_pos = cmd_pos;
	return (cmd);
}

void	ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd)
{
	t_commands	*current_cmd;

	if (!new_cmd)
		return ;
	if (!(*cmd_head))
	{
		*cmd_head = new_cmd;
		return ;
	}
	current_cmd = *cmd_head;
	while (current_cmd->next)
		current_cmd = current_cmd->next;
	current_cmd->next = new_cmd;
}

void	parse_commands(t_shell *shell)
{
	int		i;
	int		cmd_pos;
	t_commands	*new;
	char	**commands;

	i = 0;
	shell->no_cmds = count_ocurrence(shell->input, '|') + 1;
	cmd_pos = 0;
	commands = ft_split(shell->input, '|');
	if (!commands)
		ft_exit_shell(shell, EXIT_FAILURE);
	while (*(commands + i))
	{
		cmd_pos++;
		new = ft_lstnew_cmd(*(commands + i), shell, cmd_pos);
		if (!new)
			break;
		if (!shell->cmd_head)
			shell->cmd_head = new;
		else
			ft_lstadd_back_cmd(&(shell->cmd_head), new);
		i++;
	}
	free_split_alloc(commands);
	if (!new)
		ft_exit_shell(shell, EXIT_FAILURE);
}
