/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:24:33 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 11:26:35 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_special_character(char c)
{
	// removed c == '$', should only be flagged for env_var ?
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
