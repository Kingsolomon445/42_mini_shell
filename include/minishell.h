/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:01 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/13 14:13:21 by ofadahun         ###   ########.fr       */
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
# define NOFILEDIR "No such file or directory"
# define NOCMDFOUND "command not found"
# define INVALIDID "not a valid identifier"
# define MANYARGS "too many arguments"
# define OLDPWDNOTSET "OLDPWD not set"
# define PIPESYNTAXERR "minishell: syntax error near unexpected token `|'\n"
# define SYNTAXERR "minishell: syntax error near unexpected token `newline'\n"

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

typedef struct s_cmd_pos
{
	int	index;
	struct s_cmd_pos	*next;
} t_cmd_pos;

typedef struct s_commands
{
	char				**toks;
	char				*command;
	char				*cmd_type;
	int					fds[2];
	char				*vbin;
	int					cmd_pos;
	int					do_not_run;
	t_redirection		*red;
	t_dollar		*dollar;
	t_token_pos		*token_pos;
	struct s_commands	*next;
}	t_commands;

typedef struct s_parse_commands_vars
{
	char	*command;
	char	*new_command;
	char	quote;
    int i;
	int j;
	int	cmd_cnt;
	int dollar_idx;
	int	do_not_run;
	t_token_pos *token_pos;
	t_token_pos *new_token_pos;
	t_redirection	*redirection;
	t_dollar	*dollar;
	t_commands	*cmd;
} t_parse_commands_vars;


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
	char		*success_prompt;
	char		*failed_prompt;
	char		*input;
	int			last_status;
	int			no_cmds;
	int			dollar_idx;
	int			sucess;
	int			do_not_run;
	t_commands	*cmd_head;
	t_cmd_pos	*cmd_pos_head;
	t_list		*hist_head;
}	t_shell;


char	**create_tokens(t_commands *cmd);
int		parse_shell(t_shell *shell);
t_commands *parse_commands(t_shell *shell, char *old_command);
int	parse_redirection(t_shell *shell, t_parse_commands_vars *vars,  char *input);
int expand(t_shell *shell, t_parse_commands_vars *vars, char *input);
t_dollar	*check_to_substitute(t_dollar **dollar_head, int idx);
char	*ft_substitute(char *new_command, t_dollar **dollar_head, int *dollar_idx);
int	handle_redirections(t_commands *cmd);

//PARSERS

//PARSE INPUT
int	open_fd(char *file_name, char red_type);


//PROCESS && EXECUTORS
void	run_commands(t_shell *shell);
int		is_it_builtin(char **builtins, char *cmd);
void	run_processes(t_commands *cmd, t_shell *shell, int fds[2]);
void	ft_exec_in_child_process(t_shell *shell, t_commands *cmd);

//UTILS
int		compare_str(char *src, char *dst);
int		compare_builtin(char *src, char *dst);
char	*get_valid_bin(t_shell *shell, char *cmd);
char	**get_path_from_env(char **env);
char	**get_final_path(void);

//HISTORY
void	print_history(t_shell *shell);

//BUILT IN
void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell);
int	update_env_item(t_shell *shell, char *env_title, char *new_env);
int		check_valid_identifier(char *env_title);
int		check_env(char **env, char *env_title);
void	ft_execute_builtin(t_commands *cur_cmd, t_shell *shell);
void    when_one_builtin(t_shell *shell, t_commands *cur_cmd);

//HERE DOC
int		run_here_doc(t_shell *shell, char *delim);

//ENV
int		env_len(char **str);
int		init_shellenv(t_shell *shell);
int		print_env(char **env);
void	env_main(t_shell *shell, char **toks);
int		ft_putenv(t_shell *shell, char *new_env);

//EXPORT
int	export_env(t_shell *shell);


//CD
int		change_directory(t_shell *shell, t_commands *cmd);

//PWD
int		get_pwd(t_shell *shell);

//UNSET
int		unset_var(t_shell *shell);
void	unset_main(t_shell *shell);

//ECHO
void	echo_echo(t_commands *cmd);

//EXIT
void	perform_exit(t_commands *cmd, t_shell *shell);

//BUILTIN ERRORS
void	invalid_option(t_shell *shell, char *builtin, char *error_arg);
void    non_numeric_exit(t_shell *shell, char *invalid_arg, int exit_code);
void    normal_exit(t_shell *shell);
int		print_error(int status,  char *builtin, char *error_arg, char *msg);

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
t_commands	*ft_lstnew_cmd(t_shell *shell, t_parse_commands_vars *vars);
void	ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd);
t_dollar *ft_lstnew_dollar(int index, char *value);
void ft_lstadd_back_dollar(t_dollar **dollar_head, t_dollar *new_dollar);
t_token_pos  *ft_lstnew_tokenpos(int index);
void ft_lstadd_back_tokenpos(t_token_pos **token_pos_head, t_token_pos *new_token_pos);
void	ft_free_tokenpos(t_token_pos **token_pos);
t_cmd_pos  *ft_lstnew_cmdpos(int index);
void ft_lstadd_back_cmdpos(t_cmd_pos **cmd_pos_head, t_cmd_pos *new_cmd_pos);
void	ft_free_cmdpos(t_cmd_pos **cmd_pos);

#endif