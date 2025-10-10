/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tokenization.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:16:42 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 14:17:15 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	arg_len(const char *str)
{
	int	in_double;
	int	in_single;
	int	i;

	i = -1;
	in_double = 0;
	in_single = 0;
	while (str[++i])
	{
		if (str[i] == '"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (!in_double && !in_single && (str[i] == '<' || str[i] == '>'))
			break ;
		else if (!in_double && !in_single && (str[i] == '|' || str[i] == '&'))
			break ;
		else if (!in_double && !in_single && (str[i] == '(' || str[i] == ')'))
			break ;
		else if (!in_double && !in_single && str[i] == ' ')
			break ;
	}
	if (in_single || in_double)
		return (-1);
	return (i);
}

t_token	*parse_export_args(t_shell *data, char **command, t_token *token,
int *export)
{
	int	len;

	if (**command == ' ')
		return (tokenize_space(command, token));
	len = arg_len(*command);
	if (len < 1)
	{
		*export = 0;
		return (NULL);
	}
	token->type = WORD;
	token->content = (char *) malloc((len + 1) * sizeof(char));
	if (!token->content)
	{
		free(token);
		malloc_error(NULL, data, data->tokens);
	}
	ft_strlcpy(token->content, *command, len + 1);
	*command += len;
	return (token);
}
