/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:14:05 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/11 04:03:19 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
void pwd(int *last_exit_status)
{
    char cwd[1024];
    if(!getcwd(cwd, sizeof(cwd))){
        perror("pwd");
        *last_exit_status = 1;
    }else{
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
    }
}
