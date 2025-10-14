/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:32:03 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 20:24:00 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ispar(const char c)
{
	if (c == '(')
		return (1);
	else if (c == ')')
		return (1);
	return (0);
}

int	iswhitespace(const char c)
{
	if (c == ' ')	// To enhance
		return (1);
	return (0);
}

int	isredir(const char c)
{
	if (c == '<')
		return (1);
	else if (c == '>')
		return (1);
	return (0);
}

int	isquote(const char c)
{
	if (c == '"')
		return (1);
	else if (c == '\'')
		return (1);
	return (0);
}
