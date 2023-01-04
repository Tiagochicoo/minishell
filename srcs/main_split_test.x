/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_split_test.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 16:29:39 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/04 16:47:47 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>



char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == '\0' && c == '\0')
	{
		return (0);
	}
	return (0);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!dest || !src)
		return (0);
	i = 0;
	while (src[i])
		i++;
	if (size == 0)
		return (i);
	i = 0;
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	i = 0;
	while (src[i])
		i++;
	return (i);
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	finish(char buffer[], int count[], char **str_arr)
{
	buffer[count[1]] = '\0';
	count[2] = sizeof(char) * (ft_strlen(buffer) + 1);
	str_arr[count[0]] = malloc(count[2]);
	ft_strlcpy(str_arr[count[0]], buffer, ft_strlen(buffer) + 1);
	return ('\0');
}

void	str_alloc(const char *str, char *charset, int i, char **str_arr)
{
	int		count[4];
	char	buffer[1638954];

	count[0] = 0;
	count[3] = ft_strlen(str);
	while (i < count[3])
	{
		while (i < count[3])
		{
			if (ft_strchr(charset, str[i]) == NULL)
				break ;
			i++;
		}
		count[1] = 0;
		while (i < count[3])
		{
			if (ft_strchr(charset, str[i]) != NULL)
				break ;
			buffer[count[1]] = str[i++];
			count[1] += 1;
		}
		if (count[1] > 0)
		{
			buffer[count[1]] = finish(buffer, count, str_arr);
			count[0] += 1;
		}
	}
}

char	**ft_split(char *str, char *charset)
{
	int		len;
	int		count;
	int		i;
	int		j;
	char	**str_arr;

	len = ft_strlen(str);
	count = 0;
	i = 0;
	while (i < len)
	{
		while (i < len)
			if (ft_strchr(charset, str[i++]) == NULL)
				break ;
		j = i;
		while (i < len)
			if (ft_strchr(charset, str[i++]) != NULL)
				break ;
		if (i > j)
			count += 1;
	}
	str_arr = malloc(sizeof(char *) * (count + 1));
	str_arr[count] = NULL;
	str_alloc(str, charset, 0, str_arr);
	return (str_arr);
}

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
    {
        printf("%d - %s\n", i, arr[i]);
        free(arr[i]);
        i++;
    }
    free(&arr[0]);
}


int main(void)
{
    char    **tmp;

    tmp = ft_split("ls -la | grep .c", "|<>()");
    ft_free_split(tmp);

    return (0);
}