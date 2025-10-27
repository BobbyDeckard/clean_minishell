/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:07:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 09:50:24 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	rdr_double_expand(t_ast *node, t_rdr *rdr, char **envp, int index);
int		contains_dol(const char *str);
int		rdr_expand_cat(t_ast *node, t_rdr *rdr, char **envp, int index);
int		rdr_remove_arg(t_rdr *rdr, int i);

static int	handle_single_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	len = 0;
	end = start;
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "'", 2))
		len += ft_strlen(rdr->args[end]);
	rdr_remove_arg(rdr, start);
	if (!len)
	{
		rdr_remove_arg(rdr, start);
		return (1);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start], len);
		if (rdr->args[start + 1] && ft_strncmp(rdr->args[start + 1], "'", 2))
			rdr_remove_arg(rdr, start);
		else if (*new)
		{
			free(rdr->args[start]);
			rdr->args[start] = new;
			rdr_remove_arg(rdr, start + 1);
			break ;
		}
		else
			free(new);
	}
	return (0);
}

static int	rdr_expand(t_ast *node, t_rdr *rdr, char **envp, int index)
{
	while (contains_dol(rdr->args[index]))
	{
		if (rdr_expand_cat(node, rdr, envp, index))
			return (1);
	}
	return (0);
}

static void	expand_inside_double(t_ast *node, t_rdr *rdr, int i)
{
	// make sure replacing if by while for contains_dol works...
	while (rdr->args[++i] && ft_strncmp(rdr->args[i], "\"", 2))
	{
		while (contains_dol(rdr->args[i]))
			rdr_double_expand(node, rdr, node->shell->envp, i);
	}
}

static int	handle_double_quotes(t_ast *node, t_rdr *rdr, int start)
{
	char	*new;
	int		len;
	int		end;
	int		i;

	len = 0;
	end = start;
	expand_inside_double(node, rdr, start);
	while (rdr->args[++end] && ft_strncmp(rdr->args[end], "\"", 2))
		len += ft_strlen(rdr->args[end]);
	rdr_remove_arg(rdr, start);
	if (!len)
	{
		rdr_remove_arg(rdr, start);
		return (1);
	}
	new = (char *) ft_calloc(++len, sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	i = start;
	while (++i < end)
	{
		ft_strlcat(new, rdr->args[start], len);
		if (rdr->args[start + 1] && ft_strncmp(rdr->args[start + 1], "\"", 2))
			rdr_remove_arg(rdr, start);
		else if (*new)
		{
			free(rdr->args[start]);
			rdr->args[start] = new;
			rdr_remove_arg(rdr, start + 1);
			break ;
		}
		else
			free(new);
	}
	return (0);
}

void	redir_expander(t_ast *node, t_rdr *rdr)
{
	int	i;

//	printf("Redir args before expansion:\n");
//	i = -1;
//	while (rdr->args[++i])
//		printf("arg[%d] (%p): '%s'\n", i, rdr->args[i], rdr->args[i]);
//	printf("\n");
	i = -1;
	while (rdr->args[++i])
	{
		if (!ft_strncmp(rdr->args[i], "'", 2))
			i -= handle_single_quotes(node, rdr, i);
		else if (!ft_strncmp(rdr->args[i], "\"", 2))
			i -= handle_double_quotes(node, rdr, i);
		else if (contains_dol(rdr->args[i]))
			i -= rdr_expand(node, rdr, node->shell->envp, i);
	}
//	printf("Redir args after expansion:\n");
//	i = -1;
//	while (rdr->args[++i])
//		printf("arg[%d] (%p): '%s'\n", i, rdr->args[i], rdr->args[i]);
//	printf("\n");
	/*
	i = -1;
	len = 0;
	while (rdr->args[++i])
		len += ft_strlen(rdr->args[i]);
	if (!len)
		return ;
	rdr->file = (char *) ft_calloc(++len, sizeof(char));
	if (!rdr->file)
		malloc_error(node, node->shell, NULL);
	i = -1;
	while (rdr->args[++i])
		ft_strlcat(rdr->file, rdr->args[i], len);
	*/
}
