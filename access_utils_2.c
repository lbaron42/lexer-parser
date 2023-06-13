/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-06-05 23:11:53 by lbaron            :+:    #+#             */
/*   Updated: 2023-06-05 23:11:53 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/* access_utils_2.c: 1.*/

char	*ft_strstr(const char *haystack, const char *needle)
{
	char	*phaystack;
	size_t	needle_len;

	phaystack = (char *)haystack;
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return (phaystack);
	while (*phaystack)
	{
		if (*phaystack == *needle)
		{
			if (ft_og_strncmp((char *)needle, phaystack, needle_len) == 0)
				return (phaystack);
		}
		phaystack++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] != '\0' && big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (0);
}

/* access_utils_2.c: 2.*/

int	ft_word_count2(const char *str, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (ft_return(i, 0));
}

/* access_utils_2.c: 3.*/

int	ft_size_word(const char *s, char c, int i)
{
	int	word_size;

	word_size = 0;
	while (s[i] != c && s[i])
	{
		word_size++;
		i++;
	}
	return (ft_return(word_size, 0));
}

/* access_utils_2.c: 4.*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	i;
	unsigned int	str_len;
	unsigned int	correct_size;

	i = 0;
	str_len = ft_strlen((char *)s);
	correct_size = 1;
	if (start > str_len || len == 0)
		correct_size = 1;
	else if (len <= str_len - start)
		correct_size = len + 1;
	else if (len > str_len - start)
		correct_size = str_len - start + 1;
	substr = malloc(correct_size * sizeof(char));
	if (!substr)
		return ((void *) NULL);
	while (i != correct_size - 1)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
