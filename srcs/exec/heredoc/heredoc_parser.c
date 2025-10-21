/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:53:53 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 16:50:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	get_double_body(t_rdr *rdr, char *file, int i, int len);
int	get_single_body(t_rdr *rdr, char *file, int i, int len);
int	is_whitespace(const char *str);

static int	count_double_body(t_rdr *rdr, int i)
{
	int	len;

	len = 0;
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "\"", 2))
		len += ft_strlen(rdr->args[i]);
	return (len);
}

static int	count_single_body(t_rdr *rdr, int i)
{
	int	len;

	len = 0;
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "'", 2))
		len += ft_strlen(rdr->args[i]);
	return (len);
}

static char	*init_del(t_ast *node, int *len)
{
	char	*file;
	int		i;

	i = -1;
	while (node->rdr.args[++i])
	{
		if (!ft_strncmp(node->rdr.args[i], "\"", 2))
		{
			node->rdr.type = RDR_HEREDOC;
			*len += count_double_body(&node->rdr, i);
		}
		else if (!ft_strncmp(node->rdr.args[i], "'", 2))
		{
			node->rdr.type = RDR_HEREDOC;
			*len += count_single_body(&node->rdr, i);
		}
		else
			*len += ft_strlen(node->rdr.args[i]);
	}
	file = (char *) ft_calloc(*len, sizeof(char));
	if (!file)
		malloc_error(node, node->shell, NULL);
	return (file);
}

void	make_del(t_ast *node)
{
	char	*file;
	int		len;
	int		i;

	len = 1;
	file = init_del(node, &len);
	i = -1;
	while (node->rdr.args[++i])
	{
		if (!ft_strncmp(node->rdr.args[i], "\"", 2))
			i = get_double_body(&node->rdr, file, i, len);
		else if (!ft_strncmp(node->rdr.args[i], "'", 2))
			i = get_single_body(&node->rdr, file, i, len);
		else
			ft_strlcat(file, node->rdr.args[i], len);
	}
}
