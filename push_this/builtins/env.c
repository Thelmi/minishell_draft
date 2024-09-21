/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:02 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/18 20:59:41 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_func(t_env *env, int *last_exit_status)
{
	t_env	*tmp;

	if (!env)
	{
		write(2, "Environment is empty or not initialized.\n", 41);
		*last_exit_status = 1;
		return ;
	}
	tmp = env;
	while (tmp != NULL)
	{
		printf("%s", tmp->variable);
		printf("=");
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}
