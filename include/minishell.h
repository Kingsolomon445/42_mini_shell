/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:01 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/09 20:24:28 by ofadahun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../lib/libft/include/ft_printf.h"
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <termios.h>
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

# define RED_IN 0
# define RED_OUT 1
# define RED_APPEND 2
# define HEREDOC 3

typedef struct s_redirection
{
	int						red_type;
	int						red_fd;
	struct s_redirection	*next;
}	t_redirection;


typedef struct s_dollar
{
	int				expand_dollar;
	int				index;
	char			*value;
	struct s_dollar	*next;

}	t_dollar;

typedef struct s_token_pos
{
	int	index;
	struct s_token_pos	*next;
	
} t_token_pos;

typedef struct s_commands
{
	char				**toks;
	char				*command;
	char				*cmd_type;
	int					fds[2];
	char				*vbin;
	int					cmd_pos;
	t_redirection		*red;
	t_dollar		*dollar;
	t_token_pos		*token_pos;
	struct s_commands	*next;
}	t_commands;


typedef struct s_create_tokens
{
	char		**tokens;
	int			size;
	int			i;
	int			is_quote;
	int			token_no;
	char		*new_str;
	char		*new_str_ptr;
	char		*single_quote;
	char		*double_quote;
	char		*temp;
}	t_create_tokens;

typedef struct s_shell
{
	char		**path;
	char		**env;
	char		**builtins;
	char		**commands;
	char		*welcome_str;
	char		*input;
	int			last_status;
	int			no_cmds;
	int			dollar_idx;
	int			sucess;
	int			red_status;
	t_dollar		*dollar_head;
	t_commands	*cmd_head;
	t_token_pos	*token_pos;
	t_list		*hist_head;
}	t_shell;


char	**create_tokens(t_commands *cmd);
void	parse_shell(t_shell *shell);
t_commands *parse_commands(t_shell *shell, char *old_command);
void	parse_redirection(t_redirection **redirection,  int *i, char *input);
void expand(t_shell *shell, char *input, int *i, t_dollar **dollar_head, int dollar_idx);
t_dollar	*check_to_expand(t_dollar **dollar_head, int idx);
char	*ft_substitute(char *new_command, t_dollar **dollar_head, int *dollar_idx);

int	ft_setsinstr(char *sets, char *rem_input);

//PARSERS

//PARSE INPUT
int	open_fd(char *file_name, char red_type);


//PROCESS && EXECUTORS
void	run_commands(t_shell *shell);
int		is_it_builtin(char **builtins, char *cmd);
void	exec_builtin_cmds(void);
void	run_processes(t_commands *cmd, t_shell *shell, int fds[2]);
void	ft_exec_in_child_process(t_commands *cmd);

//UTILS
int		count_ocurrence(char *str, char ch);
int		compare_cmd(char *src, char *dst);
int		compare_builtin(char *src, char *dst);
void	ft_putenv(t_shell *shell, char *new_env);
char	*get_valid_bin(t_shell *shell, char *cmd);
char	**get_path_from_env(char **env);
char	**get_final_path(void);
char	*ft_getenv(char **env, char *env_title);

//HISTORY
void	print_history(t_shell *shell);

//BUILT IN
void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell);
void	update_env_item(t_shell *shell, char *env_title, char *new_env);
int		check_valid_identifier(char *env_title);
int		check_env(char **env, char *env_title);

//HERE DOC
int		run_here_doc(char *limiter);

//ENV
int		env_len(char **str);
void	init_shellenv(t_shell *shell);
void	print_env(char **env);
//EXPORT
void	export_env(t_shell *shell);




//CD
int		change_directory(t_shell *shell, t_commands *cmd);

//PWD
void	get_pwd(t_shell *shell);

//UNSET
void	run_unset(t_shell *shell, char *env_title);

//ECHO
void	echo_echo(t_commands *cmd);

//SIGNALS
void	set_signal_act(void);
void	ignore_signal(void);
void	sig_handler(int sig_num);

//FREE
void	ft_free_cmds(t_commands **cmd_head);
void	ft_free_split(char **str);
void	ft_free_shell(t_shell *shell);
void	ft_free(void *ptr);
void	ft_free_lst(t_list **headref);
void	ft_free_for_next_read(t_shell *shell);
void	ft_free_red(t_redirection **headref);
void	ft_free_dollar(t_dollar **headref);

void	ft_exit_shell(t_shell *shell, long status);
void    set_status(t_shell *shell, int status);


//LINKED LISTS
t_redirection	*ft_lstnew_red(int red_type, int fd);
void	ft_lstadd_back_red(t_redirection **red_head, t_redirection *new_red);
t_commands	*ft_lstnew_cmd(t_shell *shell, t_dollar *dollar, t_redirection *red, t_token_pos *token_pos, char *command);
void	ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd);
t_dollar *ft_lstnew_dollar(int index, char *value);
void ft_lstadd_back_dollar(t_dollar **dollar_head, t_dollar *new_dollar);
t_token_pos  *ft_lstnew_tokenpos(int index);
void ft_lstadd_back_tokenpos(t_token_pos **token_pos_head, t_token_pos *new_token_pos);
void	ft_free_tokenpos(t_token_pos **token_pos);

#endif