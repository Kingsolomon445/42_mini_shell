/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ofadahun <ofadahun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:16:01 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/20 18:45:11 by ofadahun         ###   ########.fr       */
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
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_commands
{
	char			**toks;
	char			*red_str;
    char    		*cmd_str;
	char			*cmd_type;
	int				fds[2];
	char			*vbin;
	int     		cmd_pos;
	struct s_commands	*next;
}	t_commands;

typedef struct s_shell
{
	char		**path;
	char		**env;
	char		**builtins;
	char		*welcome_str;
	char		*input;
	int			last_status;
	int			no_cmds;
	int			sucess;
	t_commands	*cmd_head;
	t_list		*hist_head;
}	t_shell;

//UTILS
int		count_ocurrence(char *str, char ch);
int	compare_cmd(char *src, char *dst);

//PARSER
void	parse_commands(t_shell *shell);
char    **ft_strtok(char *str, const char *delims);
char	*get_valid_bin(t_shell *shell, char *cmd);

//PROCESS AND PIPE
void	run_commands(t_shell *shell);
int		is_it_builtin(char **builtins, char *cmd);
void	exec_builtin_cmds();
void	run_processes(t_commands *cmd, t_shell *shell, int fds[2]);

//PARSE ENV AND PATH
char	*get_valid_bin(t_shell *tok, char *cmd);
char	**get_path_from_env(char **env);
char	**get_final_path();
void	is_here_doc(t_shell *shell, char *limiter);
char	*ft_getenv(char **env, char *env_title);

//INPUT
int		take_input(t_shell *shell);

//HISTORY
void	print_history(t_shell *shell);

//BUILT IN
void	ft_execute_one_builtin(t_commands *cmd, t_shell *shell);

//ENV
int		env_len(char **str);
void	init_shellenv(t_shell *shell);
void	print_env(char **env);
//EXPORT
void	export_env(t_shell *shell);

//EXPAND DOLLAR
int	search_dollar_in_tok(t_shell *shell);

//CD
void	change_directory(t_shell *shell, t_commands *tok);

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

void	ft_exit_shell(t_shell *shell, int status);

#endif