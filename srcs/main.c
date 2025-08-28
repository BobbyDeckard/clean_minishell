/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/28 10:32:54 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	print_data(t_shell data)
{
	int	i = -1;

	while (data.envp[++i])
		printf("data.envp[%d]: %s\n", i, data.envp[i]);
	i = -1;
	while (data.paths[++i])
		printf("data.paths[%d]: %s\n", i, data.paths[i]);
}
/*
int	main(int ac, char **av, char **envp)
{
	t_shell data;

	if (ac != 1)
		return (1);
	(void) av;
	data = init_shell_data(envp);
	return (clean_data(data));
}
*/
int	main(int ac, char **av, char **envp)
{
	t_shell	data;
	char	*command;
	char	cwd[256];

	if (ac != 1)
		return (1);
	(void) av;
	data = init_shell_data(envp);
	while (1)
	{
		init_interactive_signals(data);	// in loop ??
		get_trunc_cwd(cwd, data);
		command = readline(cwd);
		if (!command)
		{
			printf("exit\n");
			break ;
		}
		else if (*command)
			printf("%s\n", command);
//			data = process_command(data, command);
		free(command);
	}
	return (clean_data(data));
}
