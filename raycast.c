#include "./assets/cub3d.h"

unsigned int	rgb_to_hex(int *rgb)
{
	int	hex;

	hex = 0;
	hex += rgb[0] * 65536;
	hex += rgb[1] * 256;
	hex += rgb[2];
	return (hex);
}

double	check_and_fabs(double ray_dir)
{
	if (ray_dir == 0)
		ray_dir = 1e30;
	return (fabs(1 / ray_dir));
}

void	draw(t_cube *cube)
{
	int	x;
	int	y;

	y = 0;
	while (y < cube->win.y)
	{
		x = 0;
		while (x < cube->win.x)
		{
			cube->img.addr[y * cube->win.x + x] = cube->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(cube->mlx, cube->window, cube->img.img, 0, 0);
}

void	render_bg(t_cube *cube)
{
	int	i;
	int	j;

	if (cube->re_buf == 1)
	{
		i = -1;
		while (++i < cube->win.y / 2)
		{
			j = -1;
			while (++j < cube->win.x)
				cube->buf[i][j] = rgb_to_hex(cube->col_c);
		}
		while (++i < cube->win.y)
		{
			j = -1;
			while (++j < cube->win.x)
				cube->buf[i][j] = rgb_to_hex(cube->col_f);
		}
	}
}

void	compute_steps(t_cube *cube)
{
	if (cube->rays.ray_dir.x < 0)
	{
		cube->ply.step.x = -1;
		cube->rays.side_dist.x = (cube->ply.pos.x - cube->rays.map.x) * cube->rays.delta_dist.x;
	}
	else
	{
		cube->ply.step.x = 1;
		cube->rays.side_dist.x = (cube->rays.map.x + 1.0 - cube->ply.pos.x) * cube->rays.delta_dist.x;
	}
	if (cube->rays.ray_dir.y < 0)
	{
		cube->ply.step.y = -1;
		cube->rays.side_dist.y = (cube->ply.pos.y - cube->rays.map.y) * cube->rays.delta_dist.y;
	}
	else
	{
		cube->ply.step.y = 1;
		cube->rays.side_dist.y = (cube->rays.map.y + 1.0 - cube->ply.pos.y) * cube->rays.delta_dist.y;
	}
}

void	compute_dda(t_cube *cube)
{
	while (cube->rays.hit == 0)
	{
		//jump to next map square, OR in x-direction, OR in y-direction
		if (cube->rays.side_dist.x < cube->rays.side_dist.y)
		{
			cube->rays.side_dist.x += cube->rays.delta_dist.x;
			cube->rays.map.x += cube->ply.step.x;
			cube->rays.w_side = 0;
		}
		else
		{
			cube->rays.side_dist.y += cube->rays.delta_dist.y;
			cube->rays.map.y += cube->ply.step.y;
			cube->rays.w_side = 1;
		}
		//Check if ray has hit a wall
		if (cube->map[cube->rays.map.x][cube->rays.map.y] == '1')
			cube->rays.hit = 1;
	}
	if (cube->rays.w_side == 0)
		cube->rays.perp_wall_dist = (cube->rays.map.x - cube->ply.pos.x + (1 - cube->ply.step.x) / 2) / cube->rays.ray_dir.x;
	else
		cube->rays.perp_wall_dist = (cube->rays.map.y - cube->ply.pos.y + (1 - cube->ply.step.y) / 2) / cube->rays.ray_dir.y;
}

void	select_texture(t_cube *cube)
{
	// choose texture
	if (cube->rays.w_side == 0)
	{
		if (cube->rays.ray_dir.x < 0)
			cube->tex_index = 3;// E;
		else
			cube->tex_index = 2;//W;
	}
	else
	{
		if (cube->rays.ray_dir.x < 0)
			cube->tex_index = 0;// N;
		else
			cube->tex_index = 1;//S;
	}
}

void	texture_calc(t_cube *cube)
{
	//Calculate height of line to draw on screen
	cube->rays.line_height = (int)(cube->win.y / cube->rays.perp_wall_dist);
	// calculate value of wallX
	if (cube->rays.w_side == 0)
		cube->rays.wall_x = cube->ply.pos.y + cube->rays.perp_wall_dist * cube->rays.ray_dir.y;
	else
		cube->rays.wall_x = cube->ply.pos.x + cube->rays.perp_wall_dist * cube->rays.ray_dir.x;
	cube->rays.wall_x -= floor(cube->rays.wall_x);
	// x coordinate on the texture
	cube->rays.tex.x = (int)(cube->rays.wall_x * (double)cube->tex_widt);
	if (cube->rays.w_side == 0 && cube->rays.ray_dir.x > 0)
		cube->rays.tex.x = cube->tex_widt - cube->rays.tex.x - 1;
	if (cube->rays.w_side == 1 && cube->rays.ray_dir.y < 0)
		cube->rays.tex.x = cube->tex_widt - cube->rays.tex.x - 1;
	// How much to increase the texture coordinate perscreen pixel
	cube->rays.step = 1.0 * cube->tex_hght / cube->rays.line_height;
}

void	compute_ray_color(t_cube *cube, int x)
{
	int	y;
	int	color;

	//calculate lowest and highest pixel to fill in current stripe
	cube->rays.draw_start = -cube->rays.line_height / 2 + cube->win.y / 2;
	if(cube->rays.draw_start < 0)
		cube->rays.draw_start = 0;
	cube->rays.draw_end = cube->rays.line_height / 2 + cube->win.y / 2;
	if(cube->rays.draw_end >= cube->win.y)
		cube->rays.draw_end = cube->win.y - 1;

	cube->rays.tex_pos = (cube->rays.draw_start - cube->win.y / 2 + cube->rays.line_height / 2) * cube->rays.step;
	y = cube->rays.draw_start;
	while ( y < cube->rays.draw_end)
	{
		// Cast the texture coordinate to integer, and mask with (cube->tex_hght - 1) in case of overflow
		cube->rays.tex.y = (int)cube->rays.tex_pos & (cube->tex_hght - 1);
		cube->rays.tex_pos += cube->rays.step;
		color = cube->textures[cube->tex_index][cube->tex_hght * cube->rays.tex.y + cube->rays.tex.x];
		//printf("->%d\n", cube->tex_hght * cube->rays.tex.y + cube->rays.tex.x);
		cube->buf[y][x] = color;
		cube->re_buf = 1;
		y++;
	}
}

void	calc(t_cube *cube)
{
	int	x;

	x = 0;
	//render_bg(cube);
	while (x < cube->win.x)
	{
		double	camera;

		cube->rays.hit = 0;
		camera = 2 * x / (double)cube->win.x - 1;
		cube->rays.ray_dir.x = cube->ply.dir.x + cube->rays.plane.x * camera;
		cube->rays.ray_dir.y = cube->ply.dir.y + cube->rays.plane.y * camera;
		cube->rays.map.x = (int)cube->ply.pos.x;
		cube->rays.map.y = (int)cube->ply.pos.y;
		cube->rays.delta_dist.x = fabs(1 / cube->rays.ray_dir.x);
		cube->rays.delta_dist.y = fabs(1 / cube->rays.ray_dir.y);
		compute_steps(cube);
		compute_dda(cube);
		select_texture(cube);
		texture_calc(cube);
		compute_ray_color(cube, x);
		x++;
	}
}

int	main_loop(t_cube *cube)
{
	render_bg(cube);
	calc(cube);
	draw(cube);
	return (0);
}

void	move_player_y(t_cube *cube, char *str)
{
	if ((ft_strncmp(str, "FORWARD", ft_strlen(str))) == 0)
	{
		if (cube->map[(int)(cube->ply.pos.x + cube->ply.dir.x * cube->rays.move_speed)][(int)(cube->ply.pos.y)] != '1')
			cube->ply.pos.x += cube->ply.dir.x * cube->rays.move_speed;
		if (cube->map[(int)(cube->ply.pos.x)][(int)(cube->ply.pos.y + cube->ply.dir.y* cube->rays.move_speed)] != '1')
			cube->ply.pos.y += cube->ply.dir.y * cube->rays.move_speed;
	}
	if ((ft_strncmp(str, "BACKWARD", ft_strlen(str))) == 0)
	{
		if (cube->map[(int)(cube->ply.pos.x - cube->ply.dir.x * cube->rays.move_speed)][(int)(cube->ply.pos.y)] != '1')
			cube->ply.pos.x -= cube->ply.dir.x * cube->rays.move_speed;
		if (cube->map[(int)(cube->ply.pos.x)][(int)(cube->ply.pos.y - cube->ply.dir.y* cube->rays.move_speed)] != '1')
			cube->ply.pos.y -= cube->ply.dir.y* cube->rays.move_speed;
	}
}

void	move_player_x(t_cube *cube, char *str)
{
	if ((ft_strncmp(str, "LEFT", ft_strlen(str))) == 0)
	{
		if (cube->map[(int)(cube->ply.pos.x - cube->rays.plane.x * cube->rays.move_speed)][(int)(cube->ply.pos.y)] != '1')
			cube->ply.pos.x -= cube->rays.plane.x * cube->rays.move_speed;
		if (cube->map[(int)(cube->ply.pos.x)][(int)(cube->ply.pos.y - cube->rays.plane.y * cube->rays.move_speed)] != '1')
			cube->ply.pos.y -= cube->rays.plane.y * cube->rays.move_speed;
	}
	if ((ft_strncmp(str, "RIGHT", ft_strlen(str))) == 0)
	{
		if (cube->map[(int)(cube->ply.pos.x + cube->rays.plane.x * cube->rays.move_speed)][(int)(cube->ply.pos.y)] != '1')
			cube->ply.pos.x += cube->rays.plane.x * cube->rays.move_speed;
		if (cube->map[(int)(cube->ply.pos.x)][(int)(cube->ply.pos.y + cube->rays.plane.y * cube->rays.move_speed)] != '1')
			cube->ply.pos.y += cube->rays.plane.y* cube->rays.move_speed;
	}
}

void	rotate_view(t_cube *cube, char *str)
{
	if ((ft_strncmp(str, "RIGHT", ft_strlen(str))) == 0)
	{
		cube->ply.old_dir.x = cube->ply.dir.x;
		cube->ply.dir.x = cube->ply.dir.x * cos(-cube->rays.rot_speed) - cube->ply.dir.y* sin(-cube->rays.rot_speed);
		cube->ply.dir.y= cube->ply.old_dir.x * sin(-cube->rays.rot_speed) + cube->ply.dir.y* cos(-cube->rays.rot_speed);
		cube->rays.old_plane.x = cube->rays.plane.x;
		cube->rays.plane.x = cube->rays.plane.x * cos(-cube->rays.rot_speed) - cube->rays.plane.y * sin(-cube->rays.rot_speed);
		cube->rays.plane.y = cube->rays.old_plane.x * sin(-cube->rays.rot_speed) + cube->rays.plane.y * cos(-cube->rays.rot_speed);
	}
	else if ((ft_strncmp(str, "LEFT", ft_strlen(str))) == 0)
	{
		cube->ply.old_dir.x = cube->ply.dir.x;
		cube->ply.dir.x = cube->ply.dir.x * cos(cube->rays.rot_speed) - cube->ply.dir.y* sin(cube->rays.rot_speed);
		cube->ply.dir.y= cube->ply.old_dir.x * sin(cube->rays.rot_speed) + cube->ply.dir.y* cos(cube->rays.rot_speed);
		cube->rays.old_plane.x = cube->rays.plane.x;
		cube->rays.plane.x = cube->rays.plane.x * cos(cube->rays.rot_speed) - cube->rays.plane.y * sin(cube->rays.rot_speed);
		cube->rays.plane.y = cube->rays.old_plane.x * sin(cube->rays.rot_speed) + cube->rays.plane.y * cos(cube->rays.rot_speed);
	}
}

int	key_press(int key, t_cube *cube)
{
	if (key == 119 || key == 119 - 32) //W
		move_player_y(cube, "FORWARD");
	if (key == 115 || key == 115 - 32) //S
		move_player_y(cube, "BACKWARD");
	if (key == 97 || key == 65) //A
		move_player_x(cube, "LEFT");
	if (key == 100 || key == 68) //D
		move_player_x(cube, "RIGHT");
	if (key == 65361)
		rotate_view(cube, "RIGHT");
	if (key == 65363)
		rotate_view(cube, "LEFT");
	if (key == 65307)
		close_cube(cube);
	mlx_clear_window(cube->mlx, cube->window);
	main_loop(cube);
	return (0);
}

void	load_image(t_cube *cube, int *texture, char *path, t_img *img)
{
	int	x;
	int	y;

	y = 0;
	img->img = mlx_xpm_file_to_image(cube->mlx, path, &img->img_width, &img->img_height);
	img->addr = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	while (y < img->img_height)
	{
		x = 0;
		while (x < img->img_width)
		{
			texture[img->img_width * y + x] = img->addr[img->img_width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(cube->mlx, img->img);
}

void	load_texture(t_cube *cube)
{
	t_img	img;

	load_image(cube, cube->textures[0], cube->tex_n, &img);
	load_image(cube, cube->textures[1], cube->tex_s, &img);
	load_image(cube, cube->textures[2], cube->tex_w, &img);
	load_image(cube, cube->textures[3], cube->tex_e, &img);
}

void	run_cube(t_cube *cube)
{
	int	i;
	int	j;
	
	cube->mlx = mlx_init();

	orientation_init(cube);
	//cube->ply.pos.x = cube->ply.pos.x; //15.0;
	//cube->ply.pos.y = cube->ply.pos.y; //11.5;
	cube->ply.dir.x = 1.0;
	cube->ply.dir.y = 0.0;
	cube->rays.plane.x = 0.0;
	cube->rays.plane.y = 0.66;
	cube->re_buf = 0;
	cube->tex_widt = 64;
	cube->tex_hght = 64;
	cube->win.x = WIN_WIDTH;
	cube->win.y = WIN_HEIGHT;

	i = 0;
	while (i < cube->win.y)
	{
		j = 0;
		while (j < cube->win.x)
		{
			cube->buf[i][j] = 0;
			j++;
		}
		i++;
	}

	cube->textures = (int **)malloc(sizeof(int *) * 4);
	if (!cube->textures)
		return ;
	i = 0;
	while ( i < 4)
	{
		cube->textures[i] = (int *)malloc(sizeof(int) * (cube->tex_hght * cube->tex_widt));
		if (!cube->textures[i])
			return ;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < cube->tex_hght * cube->tex_widt)
		{
			cube->textures[i][j] = 0;
			j++;
		}
		i++;
	}

	load_texture(cube);

	cube->rays.move_speed = 0.05;
	cube->rays.rot_speed = 0.05;
	
	cube->window = mlx_new_window(cube->mlx, cube->win.x, cube->win.y, "Cub3D");
	if (!cube->window)
		printf("Window initialization failed!\n");
	cube->img.img = mlx_new_image(cube->mlx, cube->win.x, cube->win.y);
	if (!cube->img.img)
		printf("Image initialization failed!\n");
	cube->img.addr = (int *)mlx_get_data_addr(cube->img.img, &cube->img.bpp, &cube->img.size_l, &cube->img.endian);
	mlx_loop_hook(cube->mlx, &main_loop, cube);
	mlx_hook(cube->window, 2, 1L << 0, &key_press, cube);
	mlx_hook(cube->window, 17, 0L, close_cube, cube);

	mlx_loop(cube->mlx);
}