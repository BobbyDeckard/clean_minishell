/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 09:15:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 12:39:42 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	define_double_redir(char **command, t_token *token, int *len)
{
	if (**command == '>' && (*command)[1] == '>')
		token->type = REDIR_APPEND;
	else if (**command == '<' && (*command)[1] == '<')
		token->type = HEREDOC;
	else
		token->type = UNKNOWN;
	*len = 2;
}

t_token	*tokenize_redir(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	int	len;
	int	i;

	len = 1;
	if (**command == '>')
		token->type = REDIR_OUT;
	else if (**command == '<')
		token->type = REDIR_IN;
	if (is_redirection((*command)[1]))
		define_double_redir(command, token, &len);
	token->content = (char *) malloc((len + 1) * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tl);
	}
	i = -1;
	while (is_redirection((*command)[++i]) && i < len)
		token->content[i] = (*command)[i];
	token->content[i] = 0;
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
	ft_strlcat(token->content, *command, len);
	*command += --len;
	return (token);
}

t_token	*tokenize_quote(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	int	len;

	len = 1;
	if (**command == 39)
		token->type = SINGLE_QUOTE;
	else if (**command == 34)
		token->type = DOUBLE_QUOTE;
	token->content = (char *) malloc(++len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tl);
	}
	ft_strlcat(token->content, *command, len);
	*command += --len;
	return (token);
}

t_token	*tokenize_word(t_shell *data, t_token **tl, char **command,
t_token *token)
{
	int	len;

	token->type = WORD;
	len = ft_wordlen(*command);
	token->content = (char *) malloc(++len * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, tl);
	}
	ft_strlcat(token->content, *command, len);
	*command += --len;
	return (token);
}
