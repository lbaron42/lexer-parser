/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-06-05 23:00:52 by lbaron            :+:    #+#             */
/*   Updated: 2023-06-05 23:00:52 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/* access.c: 1.*/

char	**find_path(char **envp)
{
	int		i;
	char	*path;
	char	**path_buf;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	path = ft_strstr(envp[i], "/");
	path_buf = ft_split2(path, ':');
	return (path_buf);
}

/* access.c: 2.*/

char	*valid_cmd(char **path, char *cmd)
{
	int		i;
	char	*correct_path;

	i = 0;
	while (path[i])
	{
		correct_path = join_strings(path[i], "/", cmd);
		if (access(correct_path, X_OK) == 0)
			return (correct_path);
		else
		{
			free(correct_path);
		}
		i++;
	}
	return (NULL);
}
