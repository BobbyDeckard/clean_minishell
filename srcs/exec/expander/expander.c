/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:00:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/29 19:17:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	contains_dol(const char *str);
int	expand_cat(t_ast *node, t_cmd *cmd, char **envp, int index);
int	handle_double_quotes(t_ast *node, t_cmd *cmd, int start);
int	handle_single_quotes(t_ast *node, t_cmd *cmd, int start);
int	is_whitespace(const char *str);
int	remove_arg(t_cmd *cmd, int i);

//	If the environment variable exists, expand_cat always returns 0.
//	If the environment variable doesn't exist,
//	expand_cat returns 1 if the arg consisted only of the environment variable
//	and the argument as a whole had to be removed (replaced by the next one)
//	expand_cat returns 0 if the arg had more content and only the environment
//	variable was removed but the rest of the argument remains.
static int	expand(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	while (contains_dol(cmd->args[index]))
	{
		if (expand_cat(node, cmd, envp, index))
		{
			if (is_whitespace(cmd->args[index]))
				remove_arg(cmd, index);
			return (1);
		}
	}
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

static int	check_arg(t_ast *node, t_cmd *cmd, int i)
{
	if (!ft_strncmp(cmd->args[i], "'", 2))
	{
		if (handle_single_quotes(node, cmd, i))
			return (1);
		return (0);
	}
	else if (!ft_strncmp(cmd->args[i], "\"", 2))
	{
		if (handle_double_quotes(node, cmd, i))
			return (1);
		return (0);
	}
	else if (contains_dol(cmd->args[i]))
		return (expand(node, cmd, node->shell->envp, i));
	return (0);
}

//	The idea is that for each argument, we check the following arguments until
//	we encounter a whitespace (marking the end of the argument) and we
//	concatenate those arguments together.
//	This concatenated new argument will then take the place of the initial
//	argument.
//	When going back to the expander, we should be able to move to the next
//	argument without missing anything in the process.
//	A difficulty we had previously was whenever an argument consists only of
//	an invalid environment variable and is thus removed.
//	We just rediscovered that we had added a way to know whenever such a
//	situation occurred (when expand returns 1).
char	*make_new_arg(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;

	new = NULL;
	if (ft_strncmp(cmd->args[i], "'", 2) && ft_strncmp(cmd->args[i], "\"", 2)
		&& ((cmd->args[i + 1] && is_whitespace(cmd->args[i + 1]))
			|| !cmd->args[i + 1]) && !contains_dol(cmd->args[i]))
		return (cmd->args[i]);
	while (cmd->args[i] && !is_whitespace(cmd->args[i]))
	{
		if (check_arg(node, cmd, i))
			continue ;
		new = cat_arg(node, new, cmd->args[i]);
		if (cmd->args[i + 1] && is_whitespace(cmd->args[i + 1]))
		{
			free(cmd->args[i]);
			break ;
		}
		else if (ft_strncmp(cmd->args[i], "'", 2)
			&& ft_strncmp(cmd->args[i], "\"", 2))
			remove_arg(cmd, i);
	}
	if (!new && is_whitespace(cmd->args[i]))
		return (cmd->args[i]);
	return (new);
}

void	expander(t_ast *node, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (is_whitespace(cmd->args[i]))
			remove_arg(cmd, i);
		else
		{
			cmd->args[i] = make_new_arg(node, cmd, i);
			i++;
		}
	}
}
