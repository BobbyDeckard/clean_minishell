/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordlen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:36:38 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 15:37:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	isop(const char c);
int	ispar(const char c);
int	isredir(const char c);
int	isquote(const char c);
int	iswhitespace(const char c);

static int	iswordchar(const char c)
{
	if (isop(c))
		return (0);
	else if (ispar(c))
		return (0);
	else if (iswhitespace(c))
		return (0);
	else if (isredir(c))
		return (0);
	else if (isquote(c))
		return (0);
	return (1);
}

int	wordlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && iswordchar(str[i]))
		i++;
	return (i);
}
