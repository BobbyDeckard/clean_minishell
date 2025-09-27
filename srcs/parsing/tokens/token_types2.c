/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:24:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 13:01:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	is_special_character(char c)
{
	if (c == '\\' || c == '#' || c == '\n' || c == '\0')
		return (1);
	return (0);
}

int	is_env_var(char c)
{
	if (c == '$')
		return (1);
	return (0);
}
