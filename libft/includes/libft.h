/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 23:02:16 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/26 21:27:36 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include "get_next_line.h"
# include "./ft_printf.h"

int				ft_putchar(char c);
int				ft_putstr(char *str);
void			*ft_memset(void *s, int c, size_t n);
void			ft_bzero(void *s, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memccpy(void *dest, const void *src, int c, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
size_t			ft_strlen(const char *s);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *haystack, const char *needle,
					size_t len);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_atoi(const char *str);
int				ft_isnumber(char *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_str_isalnum(char *str);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
void			*ft_calloc(size_t count, size_t size);
char			*ft_strdup(const char *s);
void			ft_swap(int *a, int *b);

char			*ft_strsub(char const *s, size_t start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s1, char const *set);
char			**ft_split(char const *str, char charset);
char			**ft_delete_split_arr(char **arr);
char			*ft_itoa(int n);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
char			*ft_addchartostr(char *str, char c);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **lst, t_list *new);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));

void			ft_addnfix(char **str, char fix_char, int len, int side);
char			*ft_strnew(size_t size);
char			*ft_strndup(const char *str, size_t n);
char			*ft_strncpy(char *dest, const char *src, unsigned int size);
int				ft_putwstr(wchar_t *ws);
int				ft_countletter(char *str, char c);
int				ft_count(char **str);
char			*ft_itoabase_umax(size_t num, int base);
char			*ft_chrtostr(char c);
size_t			ft_wstrlen(const wchar_t *s);
wchar_t			*ft_wstrdup(const wchar_t *wstr);
wchar_t			*ft_wchrtostr(wchar_t wchar);
char			*ft_itoabase(long num, int base);
char			*ft_strrev(char *str);
int				get_next_line(int fd, char **line);
long long		ft_word_count(char *str, char charset);
int				ft_strspn(const char *s, const char *accept);
int				ft_strcspn(const char *s, const char *reject);

#endif
