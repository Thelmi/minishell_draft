/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:20:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/14 12:18:12 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// functions from libft... temperorally, until we upload a libft to our minishell project 


char *ft_strcat(char *dest, const char *src) {
    int i = 0;
    int j = 0;

    while (dest[i] != '\0') {
        i++;
    }

    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0';

    return dest;
}


struct cmd* print_tree(struct cmd *cmd)
{
  struct execcmd *ecmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (struct execcmd*)cmd;
	printf("%s\n", ecmd->argv[0]);
  if (ecmd->argv[1])
    printf("argv[1] : %s\n", ecmd->argv[1]);
    // i = 0;
    // while (ecmd->argv[i]) {
    //   *ecmd->eargv[i] = 0;
    //   i++;
    // }
  }
  else if (cmd->type == REDIR) {
    rcmd = (struct redircmd*)cmd;
	printf("redirect %d\n", rcmd->type);
    print_tree(rcmd->cmd);
    // *rcmd->efile = 0;
  }
  else if (cmd->type == PIPE) {
	printf("|\n");
    pcmd = (struct pipecmd*)cmd;
    print_tree(pcmd->left);
    print_tree(pcmd->right);
  }
  return cmd;
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tab;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	tab = (char *)malloc(sizeof(*tab) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (tab == 0)
		return (NULL);
	while (s1[i])
	{
		tab[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		tab[i] = s2[j];
		j++;
		i++;
	}
	tab[i] = '\0';
	return (tab);
}

char	*ft_strnstr(const char	*big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (0);
}

bool	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (ft_strlen(s1) != ft_strlen(s2))
	{
		return (false);
	}
	while (s1[i] && s2[i])
	{
		if (s1[i] == s2[i])
		{
			i++;
		}
		else
		{
			return (false);
		}
	}
	return (true);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!dst || !src || dstsize == 0)
		return (ft_strlen(src));
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*s2;

	i = ft_strlen(s1) + 1;
	s2 = malloc(sizeof(char) * i);
	ft_strlcpy(s2, s1, i);
	return (s2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;

	if (!s || start >= ft_strlen(s) || !len)
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
	{
		str = malloc((sizeof(char) * (ft_strlen(s) - start + 1)));
		if (!str)
			return (NULL);
		ft_strlcpy(str, s + start, ft_strlen(s) - start + 1);
	}
	else
	{
		str = malloc((sizeof(char) * len + 1));
		if (!str)
			return (NULL);
		ft_strlcpy(str, s + start, len + 1);
	}
	return (str);
}

int	wordcount(char *str, char sep)
{
	int	i;
	int	word;

	word = 0;
	i = 1;
	if (!*str)
		return (0);
	while (str[i])
	{
		if ((str[i] == sep && str[i - 1] != sep)
			|| (str[i] != sep && str[i + 1] == '\0'))
			word++;
		i++;
	}
	return (word);
}

char	**free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

char	**getwords(char **arr, char *s, char c, int word)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (s[i] && k < word)
	{
		j = 0;
		if (s[i] && s[i] != c)
		{
			while (s[i + j] && s[i + j] != c)
				j++;
			arr[k] = ft_substr(s, i, j);
			if (!arr[k])
				return (free_arr(arr));
			k++;
			i += j;
		}
		else
			i++;
	}
	arr[k] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		word;

	if (!s)
		return (NULL);
	word = wordcount((char *)s, c);
	arr = malloc(sizeof(char *) * (word + 1));
	if (!arr)
		return (NULL);
	return (getwords(arr, (char *)s, c, word));
}
