/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:01 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/18 16:31:13 by sbhatta          ###   ########.fr       */
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
# define BUILTINS "echo cd pwd export unset env exit history"

typedef struct s_redir
{
	int					red_type;
	int					red_fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_tok_pos
{
	int					index;
	struct s_tok_pos	*next;
}	t_tok_pos;

typedef struct s_parse_vars
{
	char		*str;
	char		*new_str;
	char		*new_env_value;
	char		*temp;
	char		quote;
	int			i;
	int			j;
	int			size;
	int			parse_expand;
	t_tok_pos	*new_tok_pos;
}	t_parse_vars;

typedef struct s_cmd_pos
{
	int					index;
	struct s_cmd_pos	*next;
}	t_cmd_pos;

typedef struct s_commands
{
	char				**toks;
	char				*command;
	int					fds[2];
	char				*vbin;
	int					cmd_pos;
	int					do_not_run;
	t_tok_pos			*tok_pos;
	t_redir				*red;
	struct s_commands	*next;
}	t_commands;

typedef struct s_split_validate_vars
{
	int				in_s_quotes;
	int				in_d_quotes;
	int				pipe_count;
	int				i;
	char			*input;
	int				parse_pipe_res;
	t_cmd_pos		*cmd_pos;
}	t_split_validate_vars;

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
	char		*delims;
	int			last_status;
	int			no_cmds;
	int			dollar_idx;
	int			sucess;
	int			do_not_run;
	int			do_not_expand;
	int			cmd_cnt;
	int			expanded;
	int			heredoc_ran;
	t_commands	*cmd_head;
	t_cmd_pos	*cmd_pos_head;
	t_list		*hist_head;
}	t_shell;

extern int	g_ctrlc;

// INIT SHELL
void			init_shell_for_next_read(t_shell *shell);
int				init_shell(t_shell *shell);

//PARSERS

//PARSERS UTILS
int				parse_quotes(t_shell *shell, t_parse_vars *vars, char *str);
int				parse_dollar(t_shell *shell, t_parse_vars *vars, char *str);
char			*ft_realloc(char *str, int size);
char			*ft_join(char *str, char *to_add);
t_parse_vars	*init_parse_vars(void);
int				append_to_new_cmd(t_parse_vars *vars, int last);
void			ft_increment_index(char *str, char *delims, int *idx, int comp);

//PARSE INPUT
int				parse_shell(t_shell *shell);
char			*expand(t_shell *shell, char *input, int *i);
void			validate_input(t_shell *shell, t_split_validate_vars *vars);

//PARSE COMMAND AND TOKENIZE
t_commands		*parse_commands(t_shell *shell, char *old_command);
int				count_commands(char **commands);
char			**create_tokens(t_commands *cmd);

//PARSE REDIRECTION
int				parse_redir(t_shell *shell, char *command, int *idx, \
				t_redir **redir);
int				parse_and_expand(t_shell *shell, t_parse_vars *vars, char *str);
int				handle_redirs(t_commands *cmd);
int				parse_in_out_append_redir(t_shell *shell, t_redir **redir, \
				char *file, char red_type);
void			close_all_fds(t_redir **redir);
int				check_last_char(char *input, int i);
int				parse_dollar_extra(t_shell *shell, t_parse_vars *vars, \
				char *str, t_tok_pos **tok_pos);
char			*parse_file_or_delimeter(\
t_shell *shell, char *str, int is_file);

//PARSE HEREDOC
int				get_next_heredoc_input(t_shell *shell, char *new_delim, \
				int heredoc);
int				parse_heredoc(t_shell *shell, t_redir **redir, \
				char	*delimeter, char red_type);

//PROCESS && EXECUTORS
void			run_commands(t_shell *shell);
int				is_it_builtin(char **builtins, char *cmd);
void			run_processes(t_commands *cmd, t_shell *shell, int fds[2]);
int				ft_exec_in_child_process(t_shell *shell, t_commands *cmd);

//UTILS
int				compare_str(char *src, char *dst);
int				compare_builtin(char *src, char *dst);
char			*get_valid_bin(t_shell *shell, char *cmd);
char			**get_path_from_env(char **env);
char			**get_final_path(void);
void			set_status(t_shell *shell, int status);
void			checkleaks(void);
char			*get_prompt(char *prompt);
void			set_terminal_attributes(void);

//HISTORY
int				print_history(t_shell *shell);

//BUILT IN
int				ft_execute_one_builtin(t_commands *cmd, t_shell *shell);
int				update_env_item(t_shell *shell, char *env_title, char *new_env);
int				check_valid_identifier(char *env_title);
int				check_env(char **env, char *env_title);
int				ft_execute_builtin(t_commands *cur_cmd, t_shell *shell);
void			when_one_builtin(t_shell *shell, t_commands *cur_cmd);

//HERE DOC
int				run_here_doc(t_shell *shell, char *delim);

//ENV
int				env_len(char **str);
int				init_shellenv(t_shell *shell);
int				print_env(char **env);
int				env_main(t_shell *shell, char **toks);
int				ft_putenv(t_shell *shell, char *new_env);

//EXPORT
int				export_env(t_shell *shell);
int				export_more_envs(t_shell *shell, int i);
void			print_export(char **env, int equalcount);

//CD
int				change_directory(t_shell *shell, t_commands *cmd);

//PWD
int				get_pwd(t_shell *shell);

//UNSET
int				unset_var(t_shell *shell);
int				unset_main(t_shell *shell);

//ECHO
int				echo_echo(t_commands *cmd);

//EXIT
int				perform_exit(t_commands *cmd);

//BUILTIN ERRORS
void			invalid_option(char *builtin, char *error_arg);
void			non_numeric_exit(char *invalid_arg);
void			normal_exit(t_shell *shell);
int				print_error(int status, char *builtin, \
				char *error_arg, char *msg);

//SIGNALS
void			sig_int_handler_after_exec(int sig_num);
void			sig_int_handler_before_exec(int sig_num);
void			heredoc_sig_int_handler(int sig_num);

//FREE
void			ft_free_cmds(t_commands **cmd_head);
void			ft_free_split(char **str);
void			ft_free_shell(t_shell *shell);
void			ft_free(void *ptr);
void			ft_free_lst(t_list **headref);
void			ft_free_for_next_read(t_shell *shell);
void			ft_free_red(t_redir **headref);
void			ft_exit_shell(t_shell *shell, long status);
void			print_error_exit(t_shell *shell, char *error_msg, int status);

//LINKED LISTS
t_redir			*ft_lstnew_red(int red_type, int fd);
void			ft_lstadd_back_red(t_redir **red_head, t_redir *new_red);
t_commands		*ft_lstnew_cmd(t_shell *shell, t_redir *redir, \
				t_tok_pos *tok_pos, char *command);
void			ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd);
t_tok_pos		*ft_lstnew_tokenpos(int index);
void			ft_lstadd_back_tokenpos(t_tok_pos **tok_pos_head, \
				t_tok_pos *new_tok_pos);
void			ft_free_tokenpos(t_tok_pos **tok_pos);
t_cmd_pos		*ft_lstnew_cmdpos(int index);
void			ft_lstadd_back_cmdpos(t_cmd_pos **cmd_pos_head, \
				t_cmd_pos *new_cmd_pos);
void			ft_free_cmdpos(t_cmd_pos **cmd_pos);

#endif