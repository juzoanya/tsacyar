/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 21:22:22 by juzoanya          #+#    #+#             */
/*   Updated: 2022/11/11 00:14:35 by juzoanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../assets/cub3d.h"


int	ft_pixel_get(t_img *tex, int x, int y)
{
	return (*(unsigned int *)(tex->addr + (y * tex->line_len + x * (tex->bbp / 8))));
}

void	ft_pixel_put(t_img *tex, int x, int y, unsigned int color)
{
	char	*dest;

	dest = tex->addr + (y * tex->line_len + x * (tex->bbp / 8));
	*(unsigned int *)dest = color;
}

int	render_wall(t_cube *cube, int x)
{
	int	y;
	int	color;

	(void)x;
	y = cube->rays.draw_start;
	//y = cube->win.y / 2 - cube->rays.line_height / 2;
	cube->rays.tex_pos = 0.0;
	while (y <= cube->rays.draw_end)
	{	
		cube->rays.tex_y = (int)cube->rays.tex_pos;
		cube->rays.tex_pos += cube->rays.step;
		color = ft_pixel_get(&cube->texture[cube->tex_ind], cube->rays.tex_x, cube->rays.tex_y);
		ft_pixel_put(&cube->img, x, y, color);
		y++;
	}
	// while (y < cube->win.y / 2 + cube->rays.line_height / 2)
	// {	
	// 	cube->rays.tex_y = (int)cube->rays.tex_pos;
	// 	cube->rays.tex_pos += cube->rays.step;
	// 	color = ft_pixel_get(&cube->texture[cube->tex_ind], cube->rays.tex_x, cube->rays.tex_y);
	// 	if (y >= 0 && y < cube->win.y)
	// 		ft_pixel_put(&cube->img, x, y, color);
	// 	y++;
	// }
	return (0);
}

void	choose_texture(t_cube *cube)
{
	if (cube->rays.w_side == 0 && cube->rays.ray_dir.x < 0)
		cube->tex_ind = E;
	else if (cube->rays.w_side == 0 && cube->rays.ray_dir.x > 0)
		cube->tex_ind = W;
	else if (cube->rays.w_side == 1 && cube->rays.ray_dir.x < 0)
		cube->tex_ind = N;
	else if (cube->rays.w_side == 1 && cube->rays.ray_dir.x > 0)
		cube->tex_ind = S;
	
	if (cube->rays.w_side == 0)
		cube->rays.wall_x = cube->ply.pos.y + cube->rays.perp_wall_dist * cube->rays.ray_dir.y;
	else
		cube->rays.wall_x = cube->ply.pos.x + cube->rays.perp_wall_dist * cube->rays.ray_dir.x;
	cube->rays.wall_x -= (int)(cube->rays.wall_x);
	cube->rays.tex_x = (int)(cube->rays.wall_x * (double)(cube->tex_width[cube->tex_ind]));

	if (cube->rays.w_side == 0 && cube->rays.ray_dir.x > 0)
		cube->rays.tex_x = cube->tex_width[cube->tex_ind] - cube->rays.tex_x - 1;
	if (cube->rays.w_side == 1 && cube->rays.ray_dir.y < 0)
		cube->rays.tex_x = cube->tex_width[cube->tex_ind] - cube->rays.tex_x - 1;
	cube->rays.step = 1.0 * cube->tex_hght[cube->tex_ind] / cube->rays.line_height;
	cube->rays.tex_pos = (cube->rays.draw_start - cube->win.y / 2 + cube->rays.line_height) * cube->rays.step;
	//printf("tex_x-%d\n", cube->rays.tex_x);
	//printf("tex_p-%f\n", cube->rays.tex_pos);
}

void	compute_distance(t_cube *cube)
{
	if (cube->rays.w_side == 0)
	{
		cube->rays.perp_wall_dist = (cube->rays.side_dist.x - cube->rays.delta_dist.x); //(cube->rays.map.x - cube->ply.pos.x + (1 - cube->ply.step.x) / 2) / cube->rays.ray_dir.x;//(cube->rays.side_dist.x - cube->rays.delta_dist.x);
		printf("p_dist = %f\n", cube->rays.perp_wall_dist);
		cube->rays.wall_x = cube->rays.pos.y + cube->rays.perp_wall_dist * cube->rays.ray_dir.y;
		printf("wall_x = %f\n", cube->rays.wall_x);
		cube->rays.wall_x += cube->ply.pos.y;
		printf("+wall_x = %f\n", cube->rays.wall_x);
		if (cube->rays.ray_dir.x < 0)
			cube->tex_ind = E;
		else
			cube->tex_ind = W;
	}
	else
	{
		cube->rays.perp_wall_dist = (cube->rays.side_dist.y - cube->rays.delta_dist.y); //(cube->rays.map.y - cube->ply.pos.y + (1 - cube->ply.step.y) / 2) / cube->rays.ray_dir.y; //(cube->rays.side_dist.y - cube->rays.delta_dist.y);
		printf("p_dist = %f (sd_y %f - dd_y %f)\n", cube->rays.perp_wall_dist, cube->rays.side_dist.y, cube->rays.delta_dist.y);
		cube->rays.wall_x = cube->rays.pos.x + cube->rays.perp_wall_dist * cube->rays.ray_dir.x;
		cube->rays.wall_x += cube->ply.pos.x;
		if (cube->rays.ray_dir.y < 0)
			cube->tex_ind = N;
		else
			cube->tex_ind = S;
	}
	cube->rays.wall_x -= (int)(cube->rays.wall_x);
	cube->rays.tex_x = (int)(cube->rays.wall_x * (double)(cube->tex_width[cube->tex_ind]));
	if ((cube->rays.w_side == 0 && cube->rays.ray_dir.x > 0) || (cube->rays.w_side == 1 && cube->rays.ray_dir.y < 0))
		cube->rays.tex_x = cube->tex_width[cube->tex_ind] - cube->rays.tex_x - 1;
	cube->rays.line_height = (int)(cube->win.y / cube->rays.perp_wall_dist);			//height of line to be drawn is the inverse of perpendicular wall distance.
	cube->rays.step = (double)(cube->tex_hght[cube->tex_ind] / cube->rays.line_height);
	cube->rays.draw_start = cube->win.y / 2 - cube->rays.line_height / 2;
	if (cube->rays.draw_start < 0)
		cube->rays.draw_start = 0;
	cube->rays.draw_end = cube->rays.line_height / 2 + cube->win.y / 2;
	if (cube->rays.draw_end >= cube->win.y)
		cube->rays.draw_end = cube->win.y - 1;
	//***************************************************************************************//
	// draw_wall(cube);
	// cube->rays.draw_start = cube->rays.line_height / 2 + cube->win.y / 2;
	// if (cube->rays.draw_start < 0)
	// 	cube->rays.draw_start = 0;
	// cube->rays.draw_end = cube->rays.line_height / 2 + cube->win.y / 2;
	// if (cube->rays.draw_end >= cube->win.y)
	// 	cube->rays.draw_end = cube->win.y - 1;
	// printf("wall_x = %f | tex_x = %d\n", cube->rays.wall_x, cube->rays.tex_x);
	// printf("wall_h = %f\n", cube->rays.line_height);
	// printf("tex_idx = %d\n", cube->tex_ind);
	//printf("side_dist-%f\n", cube->rays.side_dist.x);
	//printf("d_dist-%f\n", cube->rays.delta_dist.x);
	//printf("p_dist-%f\n", cube->rays.perp_wall_dist);
	//---------------------------------------------------------------------------------------//
	printf("wallSide = %d\n", cube->rays.w_side);
	printf("p_dist = %f\n", cube->rays.perp_wall_dist);
	printf("wall_x = %f\n", cube->rays.wall_x);
	printf("idx = %d\n", cube->tex_ind);
	printf("tex_x = %d\n", cube->rays.tex_x);
	printf("wall_h = %f\n", cube->rays.line_height);
	printf("step = %f\n", cube->rays.step);
}

/*This function increments ray by 1 square until a wall is hit. If ray is in x-dir
  then only sqaures in x-dir are jump and vise-versa. With every jump in the dir
  of side_dist vectors, the vactors are incresed by the value of delta_dist. Aslo 
  with every jump in direction of squares on map, map vector are increased by the 
  values of step vectors.*/
void	find_wall(t_cube *cube)
{
	printf("B-hit = %d\n", cube->rays.hit);
	int i = -1;
	while (cube->rays.hit == 0)
	{
		printf("(%d) side_x=%f | side_y=%f\n", ++i, cube->rays.side_dist.x, cube->rays.side_dist.y);
		if (cube->rays.side_dist.x < cube->rays.side_dist.y) //temp switch to >. originally <
		{
			cube->rays.side_dist.x += cube->rays.delta_dist.x;
			printf("    (%d) side_x=%f | side_y=%f\n", i, cube->rays.side_dist.x, cube->rays.side_dist.y);
			cube->rays.pos.x += cube->ply.step.x;
			printf("    (%d) r_pos_x=%f | r_pos_y=%f\n", i, cube->rays.pos.x, cube->rays.pos.y);
			cube->rays.w_side = 0;
		}
		else
		{
			cube->rays.side_dist.y += cube->rays.delta_dist.y;
			cube->rays.pos.y += cube->ply.step.y;
			cube->rays.w_side = 1;
		}
		if (cube->map[(int)cube->rays.pos.y][(int)cube->rays.pos.x] == '1') //checks if ray hits a wall
			cube->rays.hit = 1;
	}
	printf("E-hit = %d\n", cube->rays.hit);
	printf("wallSide = %d\n", cube->rays.w_side);
}

/*	Calculates the step and initial sideDistance. If ray direction has a negative component
	step.x is -1, if it has a positive component, step.y is +1. if */
void	calculate_steps(t_cube *cube)
{
	if (cube->rays.ray_dir.x < 0)
	{
		cube->ply.step.x = -1;
		cube->rays.side_dist.x = (cube->ply.pos.x - cube->rays.pos.x) * cube->rays.delta_dist.x;
	}
	else
	{
		cube->ply.step.x = 1;
		cube->rays.side_dist.x = (cube->rays.pos.x + 1.0 - cube->ply.pos.x) * cube->rays.delta_dist.x;
	}
	if (cube->rays.ray_dir.y < 0)
	{
		cube->ply.step.y = -1;
		cube->rays.side_dist.y = (cube->ply.pos.y - cube->rays.pos.y) * cube->rays.delta_dist.y;
	}
	else
	{
		cube->ply.step.y = 1;
		cube->rays.side_dist.y = (cube->rays.pos.y + 1.0 - cube->ply.pos.y) * cube->rays.delta_dist.y;
	}
	printf("r_dir_x=%f | r_dir_y=%f\n", cube->rays.ray_dir.x, cube->rays.ray_dir.y);
	printf("step_x=%f | step_y=%f\n", cube->ply.step.x, cube->ply.step.y);
	printf("side_x=%f | side_y=%f\n", cube->rays.side_dist.x, cube->rays.side_dist.y);
}

void	raycast_init(t_cube *cube)
{
	cube->rays.hit = 0;
	cube->rays.step = 0;
	cube->rays.delta_dist.x = 0;
	cube->rays.delta_dist.y = 0;
	cube->rays.pos.x = 0;
	cube->rays.pos.y = 0;
	cube->rays.ray_dir.x = 0;
	cube->rays.ray_dir.y = 0;
	cube->rays.wall_x = 0;
	cube->rays.plane.x = 0.66 * cube->rays.dir.y * -1;
	cube->rays.plane.y = 0.66 * cube->rays.dir.x;
	cube->rays.draw_start = 0;
	cube->rays.draw_end = 0;
}

double	check_and_fabs(double ray_dir)
{
	if (ray_dir == 0)
		ray_dir = 1e30;
	return (fabs(1 / ray_dir));
}

// **************************************************************
// This function initializes, calculates, and draw rays on the 	*
// mlx window													*
// **************************************************************
int	raycast(t_cube *cube)
{
	int		x;
	double	camera;

	x = -1;
	raycast_init(cube);
	//draw_bg(cube);
	while (++x < cube->win.x)
	{
		camera = 2 * x / (double)cube->win.x - 1; //x-coordinate of camera plane
		cube->rays.ray_dir.x = cube->rays.dir.x + cube->rays.plane.x * camera;
		cube->rays.ray_dir.y = cube->rays.dir.y + cube->rays.plane.y * camera;
		cube->rays.delta_dist.x = check_and_fabs(cube->rays.ray_dir.x); // fabs(1 / cube->rays.ray_dir.x); //consider a condition where ray_dir is 0. Will throw error.
		cube->rays.delta_dist.y = check_and_fabs(cube->rays.ray_dir.y); //fabs(1 / cube->rays.ray_dir.y);
		cube->rays.pos.x = (int)cube->ply.pos.x;
		cube->rays.pos.y = (int)cube->ply.pos.y;
		printf("cam_x=%f\n", camera);
		printf("p_pos_x=%f | p_pos_y=%f\n", cube->ply.pos.x, cube->ply.pos.y);
		printf("r_pos_x=%f | r_pos_y=%f\n", cube->rays.pos.x, cube->rays.pos.y);
		printf("dd_x=%f | dd_y=%f\n", cube->rays.delta_dist.x, cube->rays.delta_dist.y);
		calculate_steps(cube);
		find_wall(cube);
		compute_distance(cube);
		render_wall(cube, x);
	}
	mlx_put_image_to_window(cube->mlx, cube->window, cube->img.img, 0, 0);
	return (0);
}