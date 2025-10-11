/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:17:37 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:08:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/tokenization.h"

t_token	*tokenize_redir_in(char **command, t_token *token)
{
	token->type = REDIR_IN;
	*command += 1;
	return (token);
}

t_token	*tokenize_redir_out(char **command, t_token *token)
{
	token->type = REDIR_OUT;
	*command += 1;
	return (token);
}

t_token	*tokenize_redir_append(char **command, t_token *token)
{
	token->type = REDIR_APPEND;
	*command += 2;
	return (token);
}

t_token	*tokenize_heredoc(char **command, t_token *token)
{
	token->type = HEREDOC;
	*command += 2;
	return (token);
}
