/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:16:36 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 14:24:54 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig, siginfo_t *info, void *context)
{
	t_context	*ctx;

	(void)sig;
	(void)info;
	ctx = (t_context *)context;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ctx->last_exit_status = 130;
}

void	sigquit_handler(int sig, siginfo_t *info, void *context)
{
	t_context	*ctx;

	(void)sig;
	(void)info;
	ctx = (t_context *)context;
	write(1, "minishell$ ", 12);
	ctx->last_exit_status = 131;
}

void	setup_signals(t_context *context)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	(void)context;
	sa_int.sa_sigaction = sigint_handler;
	sa_int.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_SIGINFO;
	sa_int.sa_sigaction = sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_sigaction = sigquit_handler;
	sa_quit.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_SIGINFO;
	sa_quit.sa_sigaction = sigquit_handler;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	configure_terminal_behavior(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
