/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 21:38:10 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/26 21:45:20 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_redir(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_semicolon(char c)
{
	if (c == ';')
		return (1);
	return (0);
}

int	is_special(char c)
{
	if (is_space(c) || is_quote(c) || is_pipe(c) || is_redir(c) || is_semicolon(c))
		return (1);
	return (0);
}

int	is_valid(char c)
{
	if (is_special(c) || c == '\0')
		return (0);
	return (1);
}

int	is_valid_token(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (!is_valid(token[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_redir(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '>' && token[i + 1] == '>')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_pipe(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '|' && token[i + 1] == '|')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_semicolon(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == ';' && token[i + 1] == ';')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_quote(char *token)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (token[i])
	{
		if (token[i] == '\'' && quote == 0)
			quote = 1;
		else if (token[i] == '\'' && quote == 1)
			quote = 0;
		i++;
	}
	if (quote == 1)
		return (0);
	return (1);
}

int	is_valid_token_list(t_token *token_list)
{
	while (token_list)
	{
		if (!is_valid_token(token_list->token))
			return (0);
		if (token_list->type == REDIR && !is_valid_redir(token_list->token))
			return (0);
		if (token_list->type == PIPE && !is_valid_pipe(token_list->token))
			return (0);
		if (token_list->type == SEMICOLON && !is_valid_semicolon(token_list->token))
			return (0);
		if (token_list->type == QUOTE && !is_valid_quote(token_list->token))
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list2(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == SEMICOLON && token_list->next == NULL)
			return (0);
		if (token_list->type == PIPE && token_list->next == NULL)
			return (0);
		if (token_list->type == REDIR && token_list->next == NULL)
			return (0);
		if (token_list->type == QUOTE && token_list->next == NULL)
			return (0);
		if (token_list->type == SEMICOLON && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == QUOTE)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list3(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == SEMICOLON && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == SEMICOLON && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == SEMICOLON && token_list->next->type == QUOTE)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == QUOTE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == QUOTE)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == REDIR)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list4(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == SEMICOLON && token_list->next->type == SPACE)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == SPACE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == SPACE)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == SPACE)
			return (0);
		if (token_list->type == SPACE && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == SPACE && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == SPACE && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == SPACE && token_list->next->type == QUOTE)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list5(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == SEMICOLON && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == QUOTE)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list6(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == SEMICOLON && token_list->next->type == SEMICOLON)
			return (0);
		if (token_list->type == PIPE && token_list->next->type == PIPE)
			return (0);
		if (token_list->type == REDIR && token_list->next->type == REDIR)
			return (0);
		if (token_list->type == QUOTE && token_list->next->type == QUOTE)
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

int	is_valid_token_list(t_token *token_list)
{
	if (!is_valid_token_list1(token_list))
		return (0);
	if (!is_valid_token_list2(token_list))
		return (0);
	if (!is_valid_token_list3(token_list))
		return (0);
	if (!is_valid_token_list4(token_list))
		return (0);
	if (!is_valid_token_list5(token_list))
		return (0);
	if (!is_valid_token_list6(token_list))
		return (0);
	return (1);
}


