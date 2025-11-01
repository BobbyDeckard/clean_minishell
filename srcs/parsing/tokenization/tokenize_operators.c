/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operators.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:09:35 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 18:12:25 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/tokenization.h"

#include <stdio.h>
t_token	*tokenize_or(char **command, t_token *token)
{
	token->type = OR;
	printf("Address of *command before or tokenization: %p\n", *command);
	printf("Content of *command + 2 before or tokenization: '%c' (=%d), '%s'\n", *(*command + 2), *(*command + 2), *command + 2);
	*command += 2;
	printf("Address of *command after or tokenization: %p\n", *command);
	printf("Content after or tokenization: '%c' (= %d), '%s'\n", **command, **command, *command);
	return (token);
}

t_token	*tokenize_and(char **command, t_token *token)
{
	token->type = AND;
	*command += 2;
	return (token);
}

t_token	*tokenize_pipe(char **command, t_token *token)
{
	token->type = PIPE;
	*command += 1;
	return (token);
}

t_token	*tokenize_paren_open(char **command, t_token *token)
{
	token->type = PAREN_OPEN;
	*command += 1;
	return (token);
}

t_token	*tokenize_paren_close(char **command, t_token *token)
{
	token->type = PAREN_CLOSE;
	*command += 1;
	return (token);
}
