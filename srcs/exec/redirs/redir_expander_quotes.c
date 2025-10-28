/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:59:48 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/27 20:02:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	contains_dol(const char *str);
int	rdr_expand_cat(t_ast *node, t_rdr *rdr, char **envp, int index);
int	rdr_handle_double_quotes(t_ast *node, t_rdr *rdr, int start);
int	rdr_handle_single_quotes(t_ast *node, t_rdr *rdr, int start);

static int	rdr_expand(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	while (contains_dol(rdr->args[index]))
	{
		if (rdr_expand_cat(node, rdr, envp, index))
			return (1);
	}
	return (0);
}

void	redir_expander(t_ast *node, t_rdr *rdr)
{
	int	i;

	i = -1;
	while (rdr->args[++i])
	{
		if (!ft_strncmp(rdr->args[i], "'", 2))
			i -= rdr_handle_single_quotes(node, rdr, i);
		else if (!ft_strncmp(rdr->args[i], "\"", 2))
			i -= rdr_handle_double_quotes(node, rdr, i);
		else if (contains_dol(rdr->args[i]))
			i -= rdr_expand(node, rdr, node->shell->envp, i);
	}
}
