/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-06-06 00:01:39 by lbaron            :+:    #+#             */
/*   Updated: 2023-06-06 00:01:39 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_return(int r, int mode)
{
	static int	ret;

	if (!mode)
		ret = r;
	return (ret);
}

/* access_utils_3.c: 1.*/

int	ft_og_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while ((s1[i] != 0 || s2[i] != 0) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/* access_utils_3.c: 2.*/

char	**ft_free_str( char **strs, int j)
{
	while (j-- > 0)
		free(strs[j]);
	free(strs);
	strs = NULL;
	return (NULL);
}

/* access_utils_3.c: 3.*/

char	**ft_split2(char const *s, char c)
{
	char			**array;
	int				word_count;
	int				i;
	int				y;
	unsigned int	len;

	word_count = ft_word_count2(s, c);
	y = -1;
	i = 0;
	array = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	while (++y < word_count)
	{
		while (s[i] == c)
			i++;
		len = ft_size_word(s, c, i);
		array[y] = ft_substr(s, i, len);
		if (!array[y])
			ft_free_str(array, y);
		i = i + len;
	}
	array[y] = 0;
	return (array);
}
