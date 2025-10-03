/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paren_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:01:19 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 13:02:33 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_operator(t_token *token)
{
	if (!token)
		return (1);
	else if (token->type == AND)
		return (1);
	else if (token->type == OR)
		return (1);
	return (0);
}
