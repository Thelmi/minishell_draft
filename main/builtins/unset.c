/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:09:50 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/11 04:50:33 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void unset_env(t_env **env, char *variable)
{
    t_env *tmp;
    t_env *to_remove;

    if (!env || !variable)
        return;
    tmp = *env;
    if (tmp != NULL && ft_strcmp(tmp->variable, variable) == true)
    {
        *env = tmp->next;
		free_env_node(tmp);
        return;
    }
    while (tmp != NULL && tmp->next != NULL)
    {
        if (num_strncmp(tmp->next->variable, variable) == 0)
        {
            to_remove = tmp->next;
            tmp->next = tmp->next->next;
			free_env_node(to_remove);
            return;
        }
        tmp = tmp->next;
    }
}

void unset(t_env **env, int ac, char **av, int *last_exit_status)
{
	int i;
	char *variable;

	i = 1;
	while (i < ac)
	{
		variable= av[i];
		if (variable == NULL || *variable == '\0')
		{
			write(2, "unset: Invalid argument: ", 25);
			write(2, av[i], strlen(av[i]));
			write(2, "\n", 1);
			*last_exit_status = 1;
		}
		else
			unset_env(env, variable);
		i++;
	}
}
