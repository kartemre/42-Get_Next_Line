/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekart <ekart@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:34:51 by ekart             #+#    #+#             */
/*   Updated: 2025/08/11 22:38:07 by ekart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_and_join(int fd, char **stash, char *buf)
{
	ssize_t	nread;
	char	*joined;

	nread = read(fd, buf, BUFFER_SIZE);
	if (nread <= 0)
		return ((int)nread);
	buf[nread] = '\0';
	if (!*stash)
	{
		*stash = gnl_strdup("");
		if (!*stash)
			return (-1);
	}
	joined = gnl_strjoin(*stash, buf);
	if (!joined)
		return (-1);
	free(*stash);
	*stash = joined;
	return (1);
}

static int	fill_stash(int fd, char **stash)
{
	char	*buf;
	int		status;

	buf = (char *)malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	status = 1;
	while (!gnl_strchr(*stash, '\n') && status > 0)
		status = read_and_join(fd, stash, buf);
	free(buf);
	if (status < 0)
		return (-1);
	return (0);
}

static char	*extract_line(const char *stash)
{
	size_t	i;
	size_t	k;
	char	*line;

	if (!stash || !*stash)
		return (NULL);
	i = 0;
	k = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	while (k < i)
	{
		line[k] = stash[k];
		k++;
	}
	line[i] = '\0';
	return (line);
}

static char	*cut_stash(char *stash)
{
	size_t	i;
	size_t	j;
	char	*rest;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free_and_null(&stash));
	i++;
	rest = (char *)malloc(gnl_strlen(stash + i) + 1);
	if (!rest)
		return (free_and_null(&stash));
	j = 0;
	while (stash[i + j])
	{
		rest[j] = stash[i + j];
		j++;
	}
	rest[j] = '\0';
	free(stash);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (fill_stash(fd, &stash) == -1)
		return (free_and_null(&stash));
	line = extract_line(stash);
	stash = cut_stash(stash);
	return (line);
}
