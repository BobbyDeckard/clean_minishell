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
int	rdr_remove_arg(t_rdr *rdr, int i);

static int	rdr_expand(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	printf("In redir expand\n");
	while (contains_dol(rdr->args[index]))
	{
		if (rdr_expand_cat(node, rdr, envp, index))
			return (1);
	}
	return (0);
}

static int	check_arg(t_ast *node, t_rdr *rdr, int index)
{
	if (!ft_strncmp(rdr->args[index], "'", 2))
	{
		rdr_handle_single_quotes(node, rdr, index);
		if (!ft_strncmp(rdr->args[index], "'", 2))
			return (1);
		return (0);
	}
	else if (!ft_strncmp(rdr->args[index], "\"", 2))
	{
		rdr_handle_double_quotes(node, rdr, index);
		if (!ft_strncmp(rdr->args[index], "\"", 2))
			return (1);
		return (0);
	}
	else if (contains_dol(rdr->args[index]))
		return (rdr_expand(node, rdr, node->shell->envp, index));
	return (0);
}

static char	*cat_arg(t_ast *node, char *str, char *arg)
{
	char	*new;
	int		len;

	len = ft_strlen(str) + ft_strlen(arg) + 1;
	new = (char *) ft_calloc(len, sizeof(char));
	if (!new)
	{
		if (str)
			free(str);
		malloc_error(node, node->shell, NULL);
	}
	if (str)
		ft_strlcpy(new, str, len);
	ft_strlcat(new, arg, len);
	if (str)
		free(str);
	return (new);
}

void	redir_expander(t_ast *node, t_rdr *rdr)
{
	char	*new;
	int		i;

	i = 0;
	new = NULL;
	while (rdr->args[i])
	{
			if (check_arg(node, rdr, i))
				continue ;
			new = cat_arg(node, new, rdr->args[i]);
	}
	free(rdr->args[i]);
	rdr->args[i] = new;

	/*
	i = -1;
	while (rdr->args[++i])
	{
		printf("Args at the beginning of redir_expander loop:\n");
		int j = -1;
		while (rdr->args[++j])
			printf("arg[%d]: '%s'\n", j, rdr->args[j]);
		if (!ft_strncmp(rdr->args[i], "'", 2))
		{
			i -= rdr_handle_single_quotes(node, rdr, i);
//			if (!ft_strncmp(rdr->args[i], "'", 2))
//				i--;
		}
		else if (!ft_strncmp(rdr->args[i], "\"", 2))
		{
			i -= rdr_handle_double_quotes(node, rdr, i);
//			if (!ft_strncmp(rdr->args[i], "\"", 2))
//				i--;
		}
		else if (contains_dol(rdr->args[i]))
			i -= rdr_expand(node, rdr, node->shell->envp, i);
		printf("Args at the end of redir_expander loop:\n");
		j = -1;
		while (rdr->args[++j])
			printf("arg[%d]: '%s'\n", j, rdr->args[j]);
	}
	*/
}
