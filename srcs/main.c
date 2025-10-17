/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:31:12 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 16:00:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

t_shell	init_shell(char **envp);
t_ast	*parse(char *command, t_shell *data);
char	*read_command(t_shell *shell);
void	clean_ast(t_ast *ast);
void	setup_execution_signals(char *command, t_shell *shell);
void	setup_interactive_signals(t_shell *shell);

void	print_tree(t_ast *ast);
static t_shell	process_command(char *command, t_shell shell)
{
	t_ast	*ast;

	add_history(command);
	shell.state = EXECUTING;
	shell.command = command;
	setup_execution_signals(command, &shell);
	ast = parse(command, &shell);
	if (ast)
	{
		printf("\nAST:\n");
		print_tree(ast);
//		shell.exit_status = exec_ast(ast);
		clean_ast(ast);
		shell.root = NULL;
	}
	if (g_signal_received == SIGINT)
	{
		shell.exit_status = 130;
		g_signal_received = 0;
	}
	else if (g_signal_received == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		shell.exit_status = 131;
		g_signal_received = 0;
	}
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*command;

	if (ac != 1)
		return (1);
	(void) av;
	shell = init_shell(envp);
	while (1)
	{
		setup_interactive_signals(&shell);
		command = read_command(&shell);
		if (!command)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else if (*command)
			shell = process_command(command, shell);
		free(command);
		shell.command = NULL;
	}
	return (clean_shell(&shell));
}
