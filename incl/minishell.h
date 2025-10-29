/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:43:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/14 14:09:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* LIBRARIES */
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"
# include "tokenization.h"
# include "shell.h"
# include "ast.h"

/* GLOBAL VARIABLE */
extern volatile sig_atomic_t	g_signal_received;

/* MacOS compilation solution */
extern void	rl_replace_line(const char *str, int i);

/* General use functions */
void		malloc_error(t_ast *node, t_shell *shell, t_token **tokens);

#endif
