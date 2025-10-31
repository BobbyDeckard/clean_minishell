/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expander_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:48:27 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/27 19:48:59 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int		get_name_len(const char *str);

int	rdr_remove_arg(t_rdr *rdr, int i)
{
	if (!rdr->args[i])
		return (0);
	free(rdr->args[i]);
	while (rdr->args[i])
	{
		rdr->args[i] = rdr->args[i + 1];
		i++;
	}
	return (1);
}

int	rdr_remove_var(t_ast *node, t_rdr *rdr, int index)
{
	char	*new;
	int		name_len;
	int		len;
	int		i;

	i = 0;
	while (rdr->args[index][i] && rdr->args[index][i] != '$')
		i++;
	name_len = get_name_len(rdr->args[index] + i);
	if (i == 0 && (int) ft_strlen(rdr->args[index]) == name_len)
		return (rdr_remove_arg(rdr, index));
	len = ft_strlen(rdr->args[index]) - name_len + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, rdr->args[index], i + 1);
	ft_strlcat(new, rdr->args[index] + i + name_len, len);
	free(rdr->args[index]);
	rdr->args[index] = new;
	return (0);
}
