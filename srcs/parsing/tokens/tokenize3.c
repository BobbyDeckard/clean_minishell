/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:52:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 13:03:09 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static t_token	*tokenize_exit_status(t_shell *data, t_token **tokens,
char **command, t_token *token)
{
	token->type = EXIT_STATUS;
	token->content = (char *) malloc(3 * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tokens);
	}
	ft_strlcpy(token->content, "$?", 3);
	(*command)++;
	return (token);
}

t_token	*tokenize_env_var(t_shell *data, t_token **tokens, char **command,
t_token *token)
{
	int	len;

	(*command)++;
	token->type = ENV_VAR;
	if (**command == '?')
		tokenize_exit_status(data, tokens, command, token);
	len = ft_wordlen(*command) + 1;
	(*command)--;
	token->content = (char *) malloc(++len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tokens);
	}
	ft_strlcpy(token->content, *command, len);
	*command += --len;
	return (token);
}

t_token	*tokenize_space(char **command, t_token *token)
{
	int	len;

	len = 1;
	token->content = NULL;
	token->type = SPACE;
	*command += len;
	return (token);
}

t_token	*tokenize_parenthesis(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	int	len;

	len = 1;
	if (**command == '(')
		token->type = PAREN_OPEN;
	else if (**command == ')')
		token->type = PAREN_CLOSE;
	token->content = (char *) malloc(++len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tl);
	}
	ft_strlcpy(token->content, *command, len);
	*command += --len;
	return (token);
}
