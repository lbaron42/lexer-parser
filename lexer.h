/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'lexer.h'                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-06-13 14:55:28 by lbaron            :+:    #+#             */
/*   Updated: 2023-06-13 14:55:28 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_TEST_LEXER_H
#define FILE_TEST_LEXER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dest, const char *src, size_t destsize);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*join_strings(const char *str1, const char *str2, const char *str3);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_return(int r, int mode);
int		ft_og_strncmp(const char *s1, const char *s2, size_t n);
int		ft_word_count2(const char *str, char c);
int		ft_size_word(const char *s, char c, int i);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split2(char const *s, char c);
char	**find_path(char **envp);
char	*valid_cmd(char **path, char *cmd);

#endif //FILE_TEST_LEXER_H
