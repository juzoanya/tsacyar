/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:13:03 by euyi              #+#    #+#             */
/*   Updated: 2022/10/31 21:42:49 by juzoanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	s1--;
	s2--;
	while (n > 0)
	{
		s1++;
		s2++;
		n--;
		if (*s1 == '\0' && *s2 == '\0')
			return (0);
		else if (*s1 == '\0')
			return (-21);
		else if (*s2 == '\0')
			return (21);
		else if (*s1 != *s2)
			return ((int)*s1 - (int)*s2);
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[(i)] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	hold;
	size_t	i;

	hold = ft_strlen(src);
	i = 0;
	if (size != 0)
	{
		while ((i < size - 1) && (i != hold))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = 0;
	}
	return (hold);
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen((char *)s);
	while (i >= 0)
	{
		if (s[(i)] == (unsigned char)c)
			return ((char *)&s[(i)]);
		i--;
	}
	return (NULL);
}

static int	ft_intcol(int a, int b)
{
	if (b == 0)
		a *= 10;
	else if (b < 10)
		a = (a * 10) + b;
	else if (b < 100)
		a = (a * 100) + b;
	else if (b < 1000)
		a = (a * 1000) + b;
	else if (b < 10000)
		a = (a * 10000) + b;
	else if (b < 100000)
		a = (a * 100000) + b;
	else if (b < 1000000)
		a = (a * 1000000) + b;
	else if (b < 10000000)
		a = (a * 10000000) + b;
	else if (b < 100000000)
		a = (a * 100000000) + b;
	else if (b < 1000000000)
		a = (a * 1000000000) + b;
	else if (b < 2147483647)
		a = (a * 1000000000) + b;
	return (a);
}

int	ft_atoi(const char *nptr)
{
	int	bank;
	int	flag;

	bank = 0;
	flag = 0;
	while ((*nptr >= 9 && *nptr <= 13) || (*nptr == 32))
		nptr++;
	if (*nptr == 45 || *nptr == 43)
	{
		if (*nptr == 45)
			flag = 1;
		nptr++;
	}
	while (*nptr >= 48 && *nptr <= 57)
	{
		bank = ft_intcol(bank, *nptr - '0');
		nptr++;
	}
	if (flag == 1 && bank > 0)
		return (bank - (bank + bank));
	return (bank);
}

int	ft_strstrchr(char *str, char *find)
{
	int	i;
	int	j;

	i = -1;
	while (find[++i])
	{
		j = -1;
		while (str[++j])
		{
			if (find[i] == str[j])
				return (j);
		}
	}
	return (-1);
}