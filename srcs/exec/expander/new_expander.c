/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:00:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/22 16:46:40 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	*get_name(t_ast *node, const char *str);
void	handle_double_quotes(t_ast *node, t_cmd *cmd, int start);
void	handle_single_quotes(t_ast *node, t_cmd *cmd, int start);
int		contains_dol(const char *str);
int		handle_exit_status(t_ast *node, t_cmd *cmd, int index);
int		handle_var(t_ast *node, t_cmd *cmd, char *entry, int index);
int		is_whitespace(const char *str);
int		remove_arg(t_cmd *cmd, int i);
int		remove_var(t_ast *node, t_cmd *cmd, int index);

static int	expand_cat(t_ast *node, t_cmd *cmd, char **envp, int index)
{
	char	*name;
	int		i;
	int		j;

	name = get_name(node, cmd->args[index]);
//	printf("\nIn expand_cat() with arg: '%s'\n", cmd->args[*index]);
//	printf("Found name to be '%s'\n", name);
	if (!name)
		return (handle_exit_status(node, cmd, index));
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (name[j] && name[j] == envp[i][j])
			j++;
		if (envp[i][j] == '=' && !name[j])
		{
			free(name);
			return (handle_var(node, cmd, envp[i] + j + 1, index));
		}
	}
	free(name);
	return (remove_var(node, cmd, index));
}

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
			return (1);
	}
	return (0);
}

static char	*cat_arg(t_ast *node, char *str, char *arg)
{
	char	*new;
	int		len;

	len = ft_strlen(str) + ft_strlen(arg) + 1;
	new = (char *) malloc(len * sizeof(char));
	if (!new)
	{
		free(str);
		malloc_error(node, node->shell, NULL);
	}
	ft_strlcpy(new, str, len);
	ft_strlcat(new, arg, len);
	if (str)
		free(str);
	return (new);
}

//	The idea is that for each argument, we check the following arguments until we encounter
//	a whitespace (marking the end of the argument) and we concatenate those arguments together.
//	This concatenated new argument will then take the place of the initial argument.
//	When going back to the expander, we should be able to move to the next argument without
//	missing anything in the process.
//	A difficulty we had previously was whenever an argument consists only of an invalid
//	environment variable and is thus removed.
//	We just rediscovered that we had added a way to know whenever such a situation occurred
//	(when expand returns 1).
char	*make_new_arg(t_ast *node, t_cmd *cmd, int i)
{
	char	*new;

	new = NULL;
//	int j = -1;
	printf("In make_new_arg starting with arg[%d]: '%s'\n", i, cmd->args[i]);
//	while (cmd->args[++j])
//		printf("arg[%d] (%p): '%s'\n", j, cmd->args[j], cmd->args[j]);
	while (cmd->args[i] && !is_whitespace(cmd->args[i]))
	{
		printf("Making new (%s), checking arg[%d]: '%s'\n", new, i, cmd->args[i]);
		if (!ft_strncmp(cmd->args[i], "'", 2))
			handle_single_quotes(node, cmd, i);
		else if (!ft_strncmp(cmd->args[i], "\"", 2))
			handle_double_quotes(node, cmd, i);
		else if (contains_dol(cmd->args[i]))
		{
			if (expand(node, cmd, node->shell->envp, i))
				continue ;
		}
		new = cat_arg(node, new, cmd->args[i]);
		printf("About to remove arg[%d] (%p): '%s'\n", i, cmd->args[i], cmd->args[i]);
		remove_arg(cmd, i);
//		printf("About to free arg[%d]: %p\n", i, cmd->args[i]);
//		free(cmd->args[i]);
//		i++;
//		printf("Calling remove_arg from make_new_arg\n");
//		remove_arg(cmd, i);
	}
	return (new);
}

void	expander(t_ast *node, t_cmd *cmd)
{
	int	i;

	printf("Args before expansion:\n");
	i = -1;
	while (cmd->args[++i])
		printf("arg[%d] (%p): '%s'\n", i, cmd->args[i], cmd->args[i]);
	i = 0;
	while (cmd->args[i])
	{
		printf("\nExpanding arg[%d] (%p): '%s'\n", i, cmd->args[i], cmd->args[i]);
		if (is_whitespace(cmd->args[i]))
		{
			printf("Detected whitespace, about to remove arg[%d]\n", i);
			remove_arg(cmd, i);
		}
		else
		{
			cmd->args[i] = make_new_arg(node, cmd, i);
			printf("Made new arg[%d]: '%s'\n", i, cmd->args[i]);
			i++;
		}
		printf("Args at end of loop iteration:\n");
		int j = -1;
		while (cmd->args[++j])
			printf("arg[%d] (%p): '%s'\n", j, cmd->args[j], cmd->args[j]);
	}
	printf("Args after expansion:\n");
	i = -1;
	while (cmd->args[++i])
		printf("arg[%d] (%p): '%s'\n", i, cmd->args[i], cmd->args[i]);
	printf("\n");
}
