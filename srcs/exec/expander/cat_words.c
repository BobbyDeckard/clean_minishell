/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cat_words.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 19:56:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
static void	cat_args(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;
	int		len;

	if (!i)
		return ;
	len = ft_strlen(cmd->args[i - 1]) + ft_strlen(cmd->args[i]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[i - 1], len);
	ft_strlcat(new, cmd->args[i], len);
	free(cmd->args[i - 1]);
	cmd->args[i - 1] = new;
	free(cmd->args[i]);
	while (i < cmd->arg_count)
	{
		cmd->args[i] = cmd->args[i + 1];
		if (i + 1 < cmd->arg_count)
		{
			cmd->exp[i] = cmd->exp[i + 1];
			cmd->cat[i] = cmd->cat[i + 1];
		}
		i++;
	}
	cmd->arg_count--;
}
*/

int		remove_arg(t_cmd *cmd, int i);

int	is_whitespace(const char *str)
{
	if (ft_strlen(str) != 1)
		return (0);
	else if (*str >= 9 && *str <= 13)
		return (1);
	else if (*str == 32)
		return (1);
	return (0);
}

void	cat_args(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;
	int		len;

//	printf("In cat_args()\n");
//	printf("\targ[%d]: '%s'\n", i, cmd->args[i]);
//	printf("\targ[%d]: '%s'\n", i + 1, cmd->args[i + 1]);
	len = ft_strlen(cmd->args[i]) + ft_strlen(cmd->args[i + 1]) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(new, cmd->args[i], len);
	ft_strlcat(new, cmd->args[i + 1], len);
	free(cmd->args[i]);
	cmd->args[i] = new;
	remove_arg(cmd, i + 1);
}

/*
void	cat_words(t_ast *node, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (++i < cmd->arg_count)
	{
		if (cmd->cat[i])
			cat_args(node, cmd, i--);
	}
}
*/
