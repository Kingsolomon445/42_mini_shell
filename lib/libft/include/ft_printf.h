/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbhatta <sbhatta@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 13:44:34 by sbhatta           #+#    #+#             */
/*   Updated: 2023/07/18 12:22:37 by sbhatta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include "libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

int		ft_printf_fd(int fd, const char *str, ...);
void	ft_print_hex(unsigned long n, char ch, int *i, int fd);
void	ft_putnbr_long(long n, int fd);
void	ft_printf_char(int *i, char c, int fd);
void	ft_printf_str(int *i, char *str, int fd);
void	ft_printf_ptr(unsigned long p, int *i, char ch, int fd);
int		ft_printf_count(long n);
void	ft_printf_perc(int *i, int fd);
char	*get_next_line(int fd);
#endif