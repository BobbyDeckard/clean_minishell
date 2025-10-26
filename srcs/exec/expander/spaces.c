/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:20:19 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 20:19:46 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	contains_contig_spaces(const char *str)
{
	int	space;
	int	i;

	i = -1;
	space = 0;
	while (str[++i])
	{
		if (str[i] == ' ' && !space)
			space = !space;
		else if (str[i] == ' ')
			return (1);
		else if (space)
			space = !space;
	}
	return (0);
}

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
		{
			space = !space;
			count++;
		}
		else if (str[i] != ' ' && space)
		{
			space = !space;
			count++;
		}
		else if (str[i] != ' ')
		{
			count++;
			if (start)
				start = !start;
		}
	}
	return (count);
}

static int	is_whitespace_char(const char c)
{
	if (c >= 9 && c <= 13)
		return (1);
	else if (c == 32)
		return (1);
	return (0);
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

char	*filter_spaces(t_ast *node, char *entry, int *flag)
{
	char	*new;
	int		len;

	len = get_shortened_len(entry, 0, 0) + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	make_new_entry(new, entry, len);
	*flag = 1;
	return (new);
}
