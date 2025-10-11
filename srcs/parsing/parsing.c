/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:30:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:21:49 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

t_token	**tokenize_command(t_shell *shell, char *command);
void	print_token_list(t_token **token_list);

t_ast	*parse(char *command, t_shell *data)
{
	t_token	**tokens;
	t_ast	*ast;
//	int		err;

	tokens = tokenize_command(data, command);
	if (!tokens)
	{
		ft_putstr_fd("An unexpected error has occurred while ", 2);
		ft_putstr_fd("tokenizing command\n", 2);
		return (NULL);
	}
	print_token_list(tokens);
//	err = valid_syntax(data, tokens);
//	if (err)
//		return (invalid_syntax(tokens, err));
	data->tokens = tokens;
//	ast = create_ast(token_list);
	ast = NULL;
	return (ast);
}
