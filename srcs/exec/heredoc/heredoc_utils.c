/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:27:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:05:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	check_and_open(char *name, t_ast *node, t_cmd *cmd)
{
	if (access(name, F_OK) == 0)
		return (0);
	cmd->fd_in = open(name, O_WRONLY | O_CREAT, 0644);
	node->rdr.file = name;
	if (cmd->fd_in < 0)
		perror(node->rdr.file);
	return (1);
}

static int	find_name(char *name, t_ast *node, t_cmd *cmd)
{
	int	i;
	int	j;

	i = '0';
	while (i < '9')
	{
		name[4] = i;
		if (check_and_open(name, node, cmd))
			return (1);
		j = '0';
		while (j < '9')
		{
			name[5] = j;
			if (check_and_open(name, node, cmd))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	open_temp(t_ast *node, t_cmd *cmd)
{
	char	*name;

	name = (char *) malloc(7 * sizeof(char));
	if (!name)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(name, "temp", 7);
	name[4] = 0;
	name[5] = 0;
	name[6] = 0;
	if (check_and_open(name, node, cmd))
		return (1);
	return (find_name(name, node, cmd));
}

char	*copy_delimiter(t_ast *node)
{
	char	*del;
	int		len;

	len = ft_strlen(node->rdr.file) + 1;
	del = (char *) malloc(len * sizeof(char));
	if (!del)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(del, node->rdr.file, len);
	return (del);
}

void	unlink_heredoc(t_ast *node)
{
	t_n_type	type;
	int			i;

	if (!node->children)
		return ;
	i = -1;
	while (node->children[++i])
	{
		type = node->children[i]->type;
		if (type == NODE_HEREDOC)
			unlink(node->children[i]->rdr.file);
	}
}
