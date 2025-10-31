/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_args_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 22:29:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/31 22:50:15 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	free_char_array(char **arr);
void	free_char_array_size(char **arr, int i);

static int	get_double_len(t_rdr *rdr, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "\"", 2))
		len += ft_strlen(rdr->args[i++]);
	if (rdr->args[i])
		i++;
	*ptr = len;
	return (i);
}

static int	get_single_len(t_rdr *rdr, int i, int *ptr)
{
	int	len;

	len = *ptr;
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "'", 2))
		len += ft_strlen(rdr->args[i++]);
	if (rdr->args[i])
		i++;
	*ptr = len;
	return (i);
}

static int	get_len(t_rdr *rdr, int i)
{
	int	len;

	len = 0;
	while (rdr->args[i])
	{
		if (!ft_strncmp(rdr->args[i], "\"", 2))
			i = get_double_len(rdr, ++i, &len);
		else if (!ft_strncmp(rdr->args[i], "'", 2))
			i = get_single_len(rdr, ++i, &len);
		else
			len += ft_strlen(rdr->args[i++]);
	}
	return (len);
}

static int	cat_double(t_rdr *rdr, int i, char *arg, int len)
{
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "\"", 2))
		ft_strlcat(arg, rdr->args[i++], len);
	if (rdr->args[i])
		i++;
	return (i);
}

static int	cat_single(t_rdr *rdr, int i, char *arg, int len)
{
	while (rdr->args[i] && ft_strncmp(rdr->args[i], "'", 2))
		ft_strlcat(arg, rdr->args[i++], len);
	if (rdr->args[i])
		i++;
	return (i);
}

static char	*cat_arg(t_rdr *rdr, int *index)
{
	char	*arg;
	int		len;
	int		i;

	i = *index;
	len = get_len(rdr, i) + 1;
	arg = (char *) ft_calloc(len, sizeof(char));
	if (!arg)
		return (NULL);
	while (rdr->args[i])
	{
		if (!ft_strncmp(rdr->args[i], "\"", 2))
			i = cat_double(rdr, ++i, arg, len);
		else if (!ft_strncmp(rdr->args[i], "'", 2))
			i = cat_single(rdr, ++i, arg, len);
		else
			ft_strlcat(arg, rdr->args[i++], len);
	}
	*index = i;
	return (arg);
}

void	rdr_make_args(t_ast *node, t_rdr *rdr, char **args, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (rdr->args[i] && j < count)
	{
		args[j] = cat_arg(rdr, &i);
		if (!args[j])
		{
			free_char_array_size(args, j);
			malloc_error(node, node->shell, NULL);
		}
		j++;
	}
	free_char_array(rdr->args);
	rdr->args = args;
}
