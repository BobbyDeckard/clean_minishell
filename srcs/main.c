/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/03 11:56:34 by imeulema         ###   ########.fr       */
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

static t_shell	process_command(char *command, t_shell data)
{
	t_ast	*ast;
	
	add_history(command);
	data.state = EXECUTING;
	data.cmd = command;
	init_execution_signals(command, data);
	ast = parse(command, &data);
	if (ast)
	{
		print_tree(ast);
//		data.exit_status = exec_ast(ast);
		clean_ast(ast);
	}
	if (g_signal_received == SIGINT)
	{
		data.exit_status = 130;
		g_signal_received = 0;
	}
	else if (g_signal_received == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		data.exit_status = 131;
		g_signal_received = 0;
	}
	return (data);
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
			data = process_command(command, data);
		free(command);
		data.cmd = NULL;
	}
	return (clean_data(data));
}
