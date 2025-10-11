/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:33:04 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 14:42:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

typedef struct s_shell	t_shell;
typedef struct s_ast	t_ast;

typedef enum e_t_type
{
	REDIR_APPEND,
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	PAREN_CLOSE,
	PAREN_OPEN,
	WHITESPACE,
	REDIR_OUT,
	REDIR_IN,
	HEREDOC,
	UNSET,
	WORD,
	PIPE,
	AND,
	OR
}	t_t_type;

typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	t_t_type		type;
	char			*content;
}	t_token;

#endif
