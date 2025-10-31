/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:20:19 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 14:22:38 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_whitespace_char(const char c);

static int	get_last_non_space(const char *str)
{
	int	last_nonsp;
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == ' ')
			continue ;
		else
			last_nonsp = i;
	}
	return (last_nonsp);
}

static int	invert_space(int space, int *count)
{
	(*count)++;
	return (!space);
}

static int	get_shortened_len(const char *str, int count, int space)
{
	int	last_nonsp;
	int	start;
	int	i;

	i = -1;
	start = (*str == ' ');
	last_nonsp = get_last_non_space(str);
	while (str[++i])
	{
		if (str[i] == ' ' && i > last_nonsp)
			break ;
		else if (str[i] == ' ' && !space && !start)
			space = invert_space(space, &count);
		else if (str[i] != ' ' && space)
			space = invert_space(space, &count);
		else if (str[i] != ' ')
		{
			count++;
			if (start)
				start = !start;
		}
	}
	return (count);
}

static void	make_new_entry(char *str, char *old, int len)
{
	int	space;
	int	i;
	int	j;

	i = -1;
	j = -1;
	space = 1;
	while (old[++i] && j + 2 < len)
	{
		if (is_whitespace_char(old[i]) && !space)
		{
			space = !space;
			str[++j] = ' ';
		}
		else if (is_whitespace_char(old[i]))
			continue ;
		else
		{
			if (space)
				space = !space;
			str[++j] = old[i];
		}
	}
	if (str[j] == ' ')
		str[j] = 0;
}

char	*filter_spaces(t_ast *node, char *entry)
{
	char	*new;
	int		len;

	len = get_shortened_len(entry, 0, 0) + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	make_new_entry(new, entry, len);
	return (new);
}
