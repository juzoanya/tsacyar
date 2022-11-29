/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cube.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 12:14:11 by juzoanya          #+#    #+#             */
/*   Updated: 2022/11/10 18:59:38 by juzoanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../assets/cub3d.h"

// void	destroy_image(t_cube *cube)
// {
// 	if (game->wall.img)
// 		mlx_destroy_image(game->mlx, game->wall.img);
// 	if (game->space.img)
// 		mlx_destroy_image(game->mlx, game->space.img);
// 	if (game->coins.img)
// 		mlx_destroy_image(game->mlx, game->coins.img);
// 	if (game->exit[0].img)
// 		mlx_destroy_image(game->mlx, game->exit[0].img);
// 	if (game->exit[1].img)
// 		mlx_destroy_image(game->mlx, game->exit[1].img);
// }

void	destroy_map(t_cube *cube)
{
	int	i;

	i = 0;
	while (cube->map[i])
	{
		free(cube->map[i]);
		i++;
	}
	free(cube->map);
}

int	close_cube(t_cube *cube)
{
	destroy_map(cube);
	//destroy_image(cube);
	mlx_clear_window(cube->mlx, cube->window);
	//mlx_loop_end(cube->mlx);
	mlx_destroy_window(cube->mlx, cube->window);
	//mlx_destroy_display(cube->mlx);
	free(cube->mlx);
	exit(0);
	return (1);
}

void	set_orientation(t_cube *cube)
{
	cube->ply.dir.x = 0.0;
	cube->ply.dir.y = 0.0;
	if (cube->ply.orient == 'N')
		cube->ply.dir.y = -1.0;
	else if (cube->ply.orient == 'S')
		cube->ply.dir.y = 1.0;
	else if (cube->ply.orient == 'W')
		cube->ply.dir.x = -1.0;
	else if (cube->ply.orient == 'E')
		cube->ply.dir.x = 1.0;

}

void	orientation_init(t_cube *cube)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (cube->map[++i] && j < 0)
		j = ft_strstrchr(cube->map[i], "NSEW");
	cube->ply.pos.x = (double)(i - 1);
	cube->ply.pos.y = (double)j;
	cube->ply.orient = cube->map[(int)cube->ply.pos.x][(int)cube->ply.pos.y];
	set_orientation(cube);
}