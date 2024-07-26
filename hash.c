#include "minishell.h"

int	init_arr(t_data *data)
{
	data->uev = malloc(VAR_BUFF * sizeof(t_var_tb *));
	if (!data->uev)
		return (1);
	ft_memset(data->uev, 0, VAR_BUFF);
	return (0);
}

int	poly_r_hash(char *key)
{
	unsigned long long	base;
	unsigned long long	pow;
	unsigned long long	mod;
	unsigned long long	index;
	int					i;
	
	base = 31;
	pow = 1;
	mod = 1000000009;
	i = 0;
	index = 0;
	while (key[i])
	{
		index = (index + ((unsigned long long)key[i] - 'a' + 1) * pow) % mod;
		pow = (pow * base) % mod;
		i++;
	}
	return ((int)index % VAR_BUFF);
}

// int	main()
// {
// 	printf("%d\n", poly_r_hash("hello", 100));
// 	printf("%d\n", poly_r_hash("this", 100));
// 	printf("%d\n", poly_r_hash("is", 100));
// 	printf("%d\n", poly_r_hash("a", 100));
// 	printf("%d\n", poly_r_hash("polynomial", 100));
// 	printf("%d\n", poly_r_hash("rolling", 100));
// 	printf("%d\n", poly_r_hash("hash", 100));
// 	printf("%d\n", poly_r_hash("algorithm", 100));
// 	printf("%d\n", poly_r_hash("for", 100));
// 	printf("%d\n", poly_r_hash("unbash", 100));
// }
