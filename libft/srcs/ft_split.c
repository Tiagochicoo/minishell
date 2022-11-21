/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 08:59:57 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/21 19:35:19 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
		by mimarques
*/

char	**ft_delete_split_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
	return (NULL);
}

static long long	word_count(char *str, char charset)
{
	long long	count;

	count = 0;
	while (*str)
	{
		if (*str != charset)
		{
			++count;
			while (*str && *str != charset)
				++str;
		}
		if (*str != 0)
			str++;
	}
	return (count);
}

static void	ftl_strcpy(char *dest, char *from, char *to)
{
	while (from < to)
		*(dest++) = *(from++);
	*dest = 0;
}

char	**ft_split(char const *str, char charset)
{
	char		**str_arr;
	long long	i;
	char		*from;

	if (!ft_strchr(str, (unsigned int)charset))
		return (NULL);
	str_arr = (char **)ft_calloc(word_count((char *)str, charset) + 1, sizeof(char *));
	if (!str || !(str_arr))
		return (NULL);
	i = 0;
	while (*str)
	{
		if (*str != charset)
		{
			from = (char *)str;
			while (*str && *str != charset)
				++str;
			str_arr[i] = (char *)malloc(str - from + 1);
			ftl_strcpy(str_arr[i++], from, (char *)str);
		}
		if (*str != 0)
			++str;
	}
	str_arr[i] = 0;
	return (str_arr);
}
