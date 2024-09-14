/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:02 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/11 03:31:39 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void builtin_exit(char **args, int *last_exit_status) 
{
	int exit_status = 0;
	char *arg = args[1];

	printf("exit\n");
	if (arg != NULL) {
		while (*arg) 
		{
			if (!isdigit(*arg))
			{
				write(2, "exit: ", 6);
				write(2, args[1] ,strlen(args[1]));
				write(2, ": numeric argument required\n" ,29);
				*last_exit_status = 255;
				return;
			}
			arg++;
		}
			exit_status = atoi(args[1]); // int max
			exit_status %= 256;
		}
	*last_exit_status = exit_status;
	exit(exit_status);
}

