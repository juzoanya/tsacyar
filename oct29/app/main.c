/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 15:14:57 by euyi              #+#    #+#             */
/*   Updated: 2022/10/29 18:16:35 by juzoanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../assets/cub3d.h"

int main(int argc, char *argv[])
{
	t_cube	cube;
	
	if (argc != 2 || !check_valid_scene(&cube, argv[1]))
		return (write(2, "Error\nCheck Input\n", 18));
	run_cube2(&cube);
//	run_cube(&cube); // this should run graphics AND FREE cube.map by calling //mem_switch(cube.map, 0);
/*
	// FOR TESTING PURPOSES ONLY BEGIN***********************************************************
	printf("north_tex = %s\n", cube.tex_n);
	printf("south_tex = %s\n", cube.tex_s);
	printf("east_tex = %s\n", cube.tex_e);
	printf("west_tex = %s\n", cube.tex_w);
	printf("Fcolor R = %i, G = %i, B = %i\n", cube.col_f[0], cube.col_f[1], cube.col_f[2]);
	printf("Ccolor R = %i, G = %i, B = %i\n", cube.col_c[0], cube.col_c[1], cube.col_c[2]);
	int i = 0;
	while (cube.map[i])
	{
		printf("MAPDATA=%s\n", cube.map[i]);
		i++;
	}
	mem_switch(cube.map, 0);
	// FOR TESTING PURPOSES ONLY END****************************************************************
*/
	return (0);
}
