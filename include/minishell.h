/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:01 by sbhatta           #+#    #+#             */
/*   Updated: 2023/08/08 17:51:01 by ofadahun         ###   ########.fr       */
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

typedef struct s_eval_red_vars
{
	t_redirection	*new_red;
	char			*cmd_tmpptr;
	int				fname_len;
	int				delim_len;
	char			red_type;
	int				fd;
}	t_eval_red_vars;

typedef struct s_commands
{
	char				**toks;
	char				*red_str;
	char				*cmd_str;
	char				*cmd_type;
	int					fds[2];
	char				*vbin;
	int					cmd_pos;
	t_redirection		*red;
	struct s_commands	*next;
}	t_commands;

typedef struct s_dollar
{
	int				expand_dollar;
	int				index;
	char			*value;
	struct s_dollar	*next;

}	t_dollar;

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
	char		*welcome_str;
	char		*input;
	int			last_status;
	int			no_cmds;
	int			dollar_idx;
	int			sucess;
	int			red_status;
	t_dollar		*dollar_head;
	t_commands	*cmd_head;
	t_create_tokens	*tok_var;
	t_list		*hist_head;
}	t_shell;



//PARSERS

//PARSE INPUT
void	parse_input(t_shell *shell);

//PARSE COMMANDS
char	**ft_split_commands(char const *s, char c, t_shell *shell);

//TOKENIZATION
char	**create_tokens(t_shell *shell, char *str, const char *delims);

//REDIRECTIONS
int	handle_redirections(t_commands *cmd);
int	parse_redirection(t_shell *shell, t_commands *cmd, char *command);
//REDIRECTION UTILS
int	extract_file(t_shell *shell, char *cmd_tmp, char red_type, int *fname_len);
int	extract_delimiter(t_shell *shell, char *cmd_tmp, int *delim_len);

//PROCESS && EXECUTORS
void	run_commands(t_shell *shell);
int		is_it_builtin(char **builtins, char *cmd);
void	exec_builtin_cmds(void);
void	run_processes(t_commands *cmd, t_shell *shell, int fds[2]);
void	ft_exec_in_child_process(t_commands *cmd);

//UTILS
int		count_ocurrence(char *str, char ch);
int		count_commands(char **commands);
int		compare_cmd(char *src, char *dst);
int		compare_builtin(char *src, char *dst);
void	ft_putenv(t_shell *shell, char *new_env);
int		count_tokens(char *str, const char *delims);
char	*get_valid_bin(t_shell *shell, char *cmd);
char	*ft_increment(char *str, const char *delims);
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
void	free_commands(t_commands **cmd_head);
void	free_split_alloc(char **str);
void	free_me_from_this_burden(t_shell *shell);
void	ft_free(void *ptr);
void	ft_free_lst(t_list **headref);
void	free_for_next_read(t_shell *shell);
void	ft_free_dollar(t_dollar **headref);

void	ft_exit_shell(t_shell *shell, long status);
void    set_status(t_shell *shell, int status);


//LINKED LISTS
t_redirection	*ft_lstnew_red(int red_type, int fd);
void	ft_lstadd_back_red(t_redirection **red_head, t_redirection *new_red);
t_commands	*ft_lstnew_cmd(char *command, t_shell *shell, int cmd_pos);
void	ft_lstadd_back_cmd(t_commands **cmd_head, t_commands *new_cmd);
t_dollar *ft_lstnew_dollar(int index, char *value);
void ft_lstadd_back_dollar(t_dollar **dollar_head, t_dollar *new_dollar);

#endif