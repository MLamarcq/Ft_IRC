/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imoumini <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 19:28:54 by imoumini          #+#    #+#             */
/*   Updated: 2022/05/27 19:28:58 by imoumini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "ft_split.hpp"

char* copyString(const char* original) {
    char* copy = (char*)malloc(strlen(original) + 1);

    if (copy != NULL) {
        // Copy alphanumeric characters only
        int copyIndex = 0;
        for (int i = 0; original[i] != '\0'; ++i) {
            if (isalnum(original[i])) {
                copy[copyIndex++] = original[i];
            }
        }
        copy[copyIndex] = '\0'; // Null-terminate the new string
    }

    return copy;
}

 int	number_of_tab(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if ((s[i] != c) && (s[i] != '\0'))
		{
			count++;
			i++;
		}
		while ((s[i] != c) && (s[i] != '\0'))
			i++;
		while ((s[i] == c) && (s[i] != '\0'))
			i++;
	}
	return (count);
}

 void	fill_tab(char const *s, char c, char **p, int l)
{
	int	i;
	int	j;
	int	k;

	j = 0;
	i = 0;
	k = 0;
	while (s[i] != '\0')
	{
		while ((s[i] == c) && (i <= l - 1))
			i++;
		while ((s[i] != c) && (i <= l - 1))
			p[j][k++] = s[i++];
		if (i <= l - 1)
			p[j][k] = '\0';
		if (i <= l - 1)
		{
			k = 0;
			j++;
			i++;
		}
	}
}

 void	ft_free(char **p, int j)
{
	while (0 <= j)
	{
		free(p[j]);
		p[j] = NULL;
		j--;
	}
}

 char	**numberofcharacter(char const *s, char c, char **p)
{
	size_t	i;
	int		j;
	int		count;

	i = 0;
	j = 0;
	while (i <= (strlen(s) - 1))
	{
		count = 0;
		while ((s[i] == c) && (s[i]))
			i++;
		while ((s[i] != c) && (s[i++]))
			count++;
		if (count != 0)
		{
			p[j] = (char *)calloc((char) sizeof(char), (count + 1));
			if (p[j++] == NULL)
			{
				ft_free(p, j);
				return (p);
			}
		}
		i++;
	}
	return (p);
}

char	**ft_split(char const *s, char c)
{
	char	**p;

	p = (char **)calloc(sizeof(char *), (number_of_tab(s, c) + 1));
	if (number_of_tab(s, c) == 0)
		return (p);
	p = numberofcharacter(s, c, p);
	if (p == NULL)
	{
		return (p);
	}
	fill_tab(s, c, p, strlen(s));
	return (p);
}

// int main()
// {
//     char    **tabstr;
//     int        i;

//     i = 0;
//     if (!(tabstr = ft_split("lorem ipsnon risus. Suspendisse", ' ')))
//         printf("NULL\n");
//     // if (tabstr)
//     //     printf("NULLL");
//     else
//     {
//         while (tabstr[i])
//         {
//             printf("%s\n", tabstr[i]);
//             free(tabstr[i]);        
//             write(1, "\n", 1);
//             i++;
//         }
//     }
//     free (tabstr);
// }