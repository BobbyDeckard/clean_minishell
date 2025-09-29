/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:26:55 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 22:47:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	has_equal(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			return (1);
	}
	return (0);
}

static void	print_export(t_ast *node, char **abc)
{
	int	i;

	i = -1;
	while (abc[++i])
	{
		ft_putstr_fd("declare -x ", node->cmd.fd_out);
		ft_putstr_fd(abc[i], node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
}

static	void	order(char **abc)
{
	char	*ptr;
	int		i;
	int		j;

	i = -1;
	while (abc[++i])
	{
		j = i;
		while (abc[++j])
		{
			if (ft_strncmp(abc[i], abc[j], ft_strlen(abc[i]) + 1) > 0)
			{
				ptr = abc[i];
				abc[i] = abc[j];
				abc[j] = ptr;
			}
		}
	}
}

static int	export_print(t_ast *node, int size)
{
	char	**abc;
	int		i;

	abc = (char **) malloc((size + 1) * sizeof(char *));
	if (!abc)
		malloc_error(node, node->data, NULL);
	i = -1;
	while (node->data->envp[++i])
		abc[i] = copy_env_entry(node, abc, i, i);
	abc[i] = NULL;
	order(abc);
	print_export(node, abc);
	free_char_array(abc);
	return (set_exit_status(node, 0));
}

int	export_bltn(t_ast *node)
{
	int	size;

	size = char_arr_len(node->data->envp);
	if (node->cmd.args[1] && size == -1)
		return (create_env(node));
	else if (size == -1)
		return (set_exit_status(node, 0));
	else if (!node->cmd.args[1])
		return (export_print(node, size));
	else if (has_equal(node->cmd.args[1]))
		return (assign_var(node, size));
	else
		return (create_var(node, size));
}
