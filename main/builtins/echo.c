/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:03:09 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/14 18:47:56 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void echo(char **command, t_env *env){
    (void)env;
    bool n_flag;
    int i;
    int j;

    // printf("%s\n", command[0]);
    // printf("%s\n", command[1]);
    // printf("%s\n", command[2]);
    // printf("%s\n", command[3]);
    n_flag=false;
    i=1;
    while(command[i] && command[i][0]=='-' && is_only_n(command[i]+1)){
            n_flag=true;
            i++;
    }
    while(command[i]){
        j=0;
        while(command[i][j]){
            printf("%c", command[i][j]);
            j++;
        }
        // if(command[i+1])
        //     printf(" ");
        i++;
    }
    if(n_flag==false){
        printf("\n");
    }
}