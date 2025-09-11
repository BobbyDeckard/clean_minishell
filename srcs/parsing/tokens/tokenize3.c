/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:52:15 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/11 20:00:50 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_token	*tokenize_space(char **command, t_token *token)
{
	int	len;

	len = 1;
	token->content = NULL;
	token->type = SPACE;
	*command += len;
	return (token);
}
