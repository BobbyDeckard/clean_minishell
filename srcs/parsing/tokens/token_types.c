/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:16:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/08 11:47:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft/libft.h"

//	This method seems to be absolute shite at detecting command elements' types
//	What happens if we have special characters inside of a string argument ?
int	is_command_char(char c)
{
	if (ft_isprint(c))
		return (1);
	else if (c == '_' || c == '-' || c == '.')
		return (1);
	else if (c == '/' || c == '=' || c == '+' || c == ':')	// ajout suite à mes commentaires ?
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == 39 || c == 34)
		return (1);
	return (0);
}

int	is_redirection(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_logical_operator(char c)
{
	if (c == '&' || c == '|' || c == ';')
		return (1);
	return (0);
}

int	is_parenthesis(char c)
{
	if (c == '(' || c == ')')
		return (1);
	return (0);
}
