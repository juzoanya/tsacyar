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

int	close_cube(t_cube *cube)
{
	//destroy_map(cube);
	//destroy_image(cube);
	//mlx_clear_window(cube->mlx, cube->window);
	//mlx_loop_end(cube->mlx);
	//mlx_destroy_window(cube->mlx, cube->window);
	//mlx_destroy_display(cube->mlx);
	//free(cube->mlx);
	(void)cube;
	exit(0);
	return (1);
}

void	set_orientation(t_cube *cube)
{
	cube->rays.dir.x = 0;
	cube->rays.dir.y = 0;
	if (cube->ply.orient == 'N')
		cube->rays.dir.y = -1;
	else if (cube->ply.orient == 'S')
		cube->rays.dir.y = 1;
	else if (cube->ply.orient == 'W')
		cube->rays.dir.x = -1;
	else if (cube->ply.orient == 'E')
		cube->rays.dir.x = 1;

}

void	orientation_init(t_cube *cube)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (cube->map[++i] && j < 0)
		j = ft_strstrchr(cube->map[i], "NSEW");
	cube->ply.pos.x = (i - 1) + 0.5;
	cube->ply.pos.y = j + 0.5;
	cube->ply.orient = cube->map[(int)cube->ply.pos.x][(int)cube->ply.pos.y];
	//set_orientation(cube);
	printf("x = %f | y = %f\n", cube->ply.pos.x, cube->ply.pos.y);
}

unsigned int	hex_from_rgb(int *rgb)
{
	int	hex;

	hex = 0;
	hex += rgb[0] * 65536;
	hex += rgb[1] * 256;
	hex += rgb[2];
	return (hex);
}

// void	draw_bg(t_cube *cube)
// {
// 	int	x;
// 	int	y;

// 	y = -1;
// 	while (++y < cube->win.y / 2)
// 	{
// 		x = -1;
// 		while (++x < cube->win.x)
// 			ft_pixel_put(&cube->img, x, y, hex_from_rgb(cube->col_c));
// 	}
// 	while (++y < cube->win.y)
// 	{
// 		x = -1;
// 		while (++x < cube->win.x)
// 			ft_pixel_put(&cube->img, x, y, hex_from_rgb(cube->col_f));
// 	}
// }

/*void	texture_init(t_cube *cube)
{
	char	*texture[4];
	int		i;

	texture[N] = cube->tex_n;
	texture[S] = cube->tex_s;
	texture[W] = cube->tex_w;
	texture[E] = cube->tex_e;
	cube->img.img = mlx_new_image(cube->mlx, cube->win.x, cube->win.y);
	i = 0;
	while (i < 4)
	{
		cube->texture[i].img = mlx_xpm_file_to_image(cube->mlx, texture[i],
				&cube->tex_width[i], &cube->tex_hght[i]);
		if (!cube->texture[i].img)
			perror("Texture error detected!");
		cube->texture[i].addr = mlx_get_data_addr(cube->texture[i].img,
				&cube->texture[i].bbp, &cube->texture[i].line_len,
				&cube->texture[i].endian);
		if (!cube->texture[i].addr)
			perror("Texture address error detected!");
		i++;
	}
	cube->img.addr = mlx_get_data_addr(cube->img.img, &cube->img.bbp,
			&cube->img.line_len, &cube->img.endian);
}*/

// **************************************************************
// function to run cube. It initializes mlx and new window. it	*
// also initializes the textures and orientation				*
// **************************************************************
void	run_cube(t_cube *cube)
{
	cube->win.x = WIN_WIDTH;
	cube->win.y = WIN_HEIGHT;
	cube->mlx = mlx_init();
	if (!cube->mlx)
		perror("MLX initialization error!");
	cube->window = mlx_new_window(cube->mlx, cube->win.x, cube->win.y, "Cub3D");
	if (!cube->window)
		perror("MLX new window instantiation error!");
	orientation_init(cube);
	//printf("Orientation init Done");
	//texture_init(cube);
	//printf("Textures Init Done");
	raycast(cube);
	printf("%d\n", cube->ply.orient);
	mlx_hook(cube->window, 17, 0L, close_cube, (void *)cube);
	//mlx_hook(cube->window, 2, 1L << 0, play_input, (void *)cube);
	//mlx_loop_hook(cube->mlx, raycast, cube);
	mlx_loop(cube->mlx);
}