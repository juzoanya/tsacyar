#include "./oct29/assets/cub3d.h"
/*#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "./assets/gcamerli/mlx.h"

#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24
#define width 640
#define height 480

typedef struct	s_img
{
	void	*img;
	int		*data;

	int		size_l;
	int		bpp;
	int		endian;
	int		img_width;
	int		img_height;
}				t_img;

typedef struct	s_cube->info
{
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	void	*mlx;
	void	*win;
	t_img	img;
	int		buf[height][width];
	int		**texture;
	double	moveSpeed;
	double	rotSpeed;
	int		re_buf;

}				t_cube->info;*/

/*int	worldMap[mapWidth][mapHeight] =
						{
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
							{1,0,3,0,0,0,0,2,2,2,2,2,2,2,2,2,1,1,0,1,1,1,1,1},
							{1,0,2,0,0,0,0,2,0,2,0,2,0,2,0,2,1,0,0,0,1,1,1,1},
							{1,0,3,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
							{1,0,8,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,2,0,0,2,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,3,3,0,3,2,0,0,0,0,0,2,2,0,2,1},
							{1,0,3,0,3,0,0,0,0,3,3,0,0,0,0,0,2,0,0,0,0,0,0,1},
							{1,0,0,2,0,0,0,0,0,3,3,0,3,2,0,0,0,0,0,2,2,0,2,1},
							{1,0,3,0,3,0,0,0,0,3,3,0,3,2,0,0,2,0,0,2,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
						};*/

unsigned int	rgb_to_hex(int *rgb)
{
	int	hex;

	hex = 0;
	hex += rgb[0] * 65536;
	hex += rgb[1] * 256;
	hex += rgb[2];
	return (hex);
}

void	ft_pixel_put(t_img *tex, int x, int y, unsigned int color)
{
	int	*dest;

	dest = tex->data + (y * tex->size_l + x * (tex->bpp / 8));
	*(unsigned int *)dest = color;
}

void	draw(t_cube *cube)
{
	int	x;
	int	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			cube->info.img.data[y * width + x] = cube->info.buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(cube->info.mlx, cube->info.win, cube->info.img.img, 0, 0);
}

void	calc(t_cube *cube)
{
	int	x;
	int	y;
	int	i;
	int	j;

	x = 0;
	if (cube->info.re_buf == 1)
	{
		i = 0;
		while (i < height / 2)
		{
			j = 0;
			while (j < width)
			{
				cube->info.buf[i][j] = rgb_to_hex(cube->col_c);
				j++;
			}
			i++;
		}
		while (i < height)
		{
			j = 0;
			while (j < width)
			{
				cube->info.buf[i][j] = rgb_to_hex(cube->col_f);
				j++;
			}
			i++;
		}
	}

	while (x < width)
	{
		double cameraX = 2 * x / (double)width - 1;
		double rayDirX = cube->info.dirX + cube->info.planeX * cameraX;
		double rayDirY = cube->info.dirY + cube->info.planeY * cameraX;
		
		int mapX = (int)cube->info.posX;
		int mapY = (int)cube->info.posY;

		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;
		
		 //length of ray from one x or y-side to next x or y-side
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;
		
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		
		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (cube->info.posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - cube->info.posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (cube->info.posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - cube->info.posY) * deltaDistY;
		}

		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (cube->map[mapX][mapY] == '1')
				hit = 1;
		}
		if (side == 0)
			perpWallDist = (mapX - cube->info.posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - cube->info.posY + (1 - stepY) / 2) / rayDirY;

		//Calculate height of line to draw on screen
		int lineHeight = (int)(height / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + height / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + height / 2;
		if(drawEnd >= height)
			drawEnd = height - 1;

		// texturing calculations

		// choose texture
		if (side == 0)
		{
			if (rayDirX < 0)
				cube->tex_ind = 3;
			else
				cube->tex_ind = 2;
		}
		else
		{
			if (rayDirX < 0)
				cube->tex_ind = 0;
			else if (rayDirX > 0)
				cube->tex_ind = 1;
		}

		// calculate value of wallX
		double wallX;
		if (side == 0)
			wallX = cube->info.posY + perpWallDist * rayDirY;
		else
			wallX = cube->info.posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);

		// x coordinate on the texture
		int texX = (int)(wallX * (double)texWidth);
		if (side == 0 && rayDirX > 0)
			texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0)
			texX = texWidth - texX - 1;

		// How much to increase the texture coordinate perscreen pixel
		double step = 1.0 * texHeight / lineHeight;
		// Starting texture coordinate
		double texPos = (drawStart - height / 2 + lineHeight / 2) * step;
		y = drawStart;
		while ( y < drawEnd)
		{
			// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
			int texY = (int)texPos & (texHeight - 1);
			texPos += step;
			int color = cube->info.texture[cube->tex_ind][texHeight * texY + texX];
			// make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			//if (side == 1)
			//	color = (color >> 1) & 8355711;
			cube->info.buf[y][x] = color;
			cube->info.re_buf = 1;
			y++;
		}
		x++;
	}
}

int	main_loop(t_cube *cube)
{
	calc(cube);
	draw(cube);
	return (0);
}

int	key_press(int key, t_cube *cube)
{
	if (key == 87 + 32) //W
	{
		if (cube->map[(int)(cube->info.posX + cube->info.dirX * cube->info.moveSpeed)][(int)(cube->info.posY)] != '1')
			cube->info.posX += cube->info.dirX * cube->info.moveSpeed;
		if (cube->map[(int)(cube->info.posX)][(int)(cube->info.posY + cube->info.dirY* cube->info.moveSpeed)] != '1')
			cube->info.posY += cube->info.dirY * cube->info.moveSpeed;
	}
	//move backwards if no wall behind you
	if (key == 83 + 32) //S
	{
		if (cube->map[(int)(cube->info.posX - cube->info.dirX * cube->info.moveSpeed)][(int)(cube->info.posY)] != '1')
			cube->info.posX -= cube->info.dirX * cube->info.moveSpeed;
		if (cube->map[(int)(cube->info.posX)][(int)(cube->info.posY - cube->info.dirY* cube->info.moveSpeed)] != '1')
			cube->info.posY -= cube->info.dirY* cube->info.moveSpeed;
	}
	if (key == 65 + 32) //A
	{
		if (cube->map[(int)(cube->info.posX - cube->info.dirX * cube->info.moveSpeed)][(int)(cube->info.posY)] != '1')
			cube->info.posX -= cube->info.dirX * cube->info.moveSpeed;
		if (cube->map[(int)(cube->info.posX)][(int)(cube->info.posY - cube->info.dirY * cube->info.moveSpeed)] != '1')
			cube->info.posY -= cube->info.dirY * cube->info.moveSpeed;
	}
	if (key == 68 + 32) //D
	{
		if (cube->map[(int)(cube->info.posX + cube->info.dirX * cube->info.moveSpeed)][(int)(cube->info.posY)] != '1')
			cube->info.posX += cube->info.dirX * cube->info.moveSpeed;
		if (cube->map[(int)(cube->info.posX)][(int)(cube->info.posY + cube->info.dirY * cube->info.moveSpeed)] != '1')
			cube->info.posY += cube->info.dirY* cube->info.moveSpeed;
	}
	//rotate to the right
	if (key == 65361)
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = cube->info.dirX;
		cube->info.dirX = cube->info.dirX * cos(-cube->info.rotSpeed) - cube->info.dirY* sin(-cube->info.rotSpeed);
		cube->info.dirY= oldDirX * sin(-cube->info.rotSpeed) + cube->info.dirY* cos(-cube->info.rotSpeed);
		double oldPlaneX = cube->info.planeX;
		cube->info.planeX = cube->info.planeX * cos(-cube->info.rotSpeed) - cube->info.planeY * sin(-cube->info.rotSpeed);
		cube->info.planeY = oldPlaneX * sin(-cube->info.rotSpeed) + cube->info.planeY * cos(-cube->info.rotSpeed);
	}
	//rotate to the left
	if (key == 65363)
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = cube->info.dirX;
		cube->info.dirX = cube->info.dirX * cos(cube->info.rotSpeed) - cube->info.dirY* sin(cube->info.rotSpeed);
		cube->info.dirY= oldDirX * sin(cube->info.rotSpeed) + cube->info.dirY* cos(cube->info.rotSpeed);
		double oldPlaneX = cube->info.planeX;
		cube->info.planeX = cube->info.planeX * cos(cube->info.rotSpeed) - cube->info.planeY * sin(cube->info.rotSpeed);
		cube->info.planeY = oldPlaneX * sin(cube->info.rotSpeed) + cube->info.planeY * cos(cube->info.rotSpeed);
	}
	if (key == 65307)
		close_cube(cube);
	mlx_clear_window(cube->info.mlx, cube->info.win);
	main_loop(cube);
	return (0);
}

void	load_image(t_cube *cube, int *texture, char *path, t_img *img)
{
	int	x;
	int	y;

	y = 0;
	img->img = mlx_xpm_file_to_image(cube->info.mlx, path, &img->img_width, &img->img_height);
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	while (y < img->img_height)
	{
		x = 0;
		while (x < img->img_width)
		{
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
			x++;
		}
		y++;
	}
	mlx_destroy_image(cube->info.mlx, img->img);
}

void	load_texture(t_cube *cube)
{
	t_img	img;

	load_image(cube, cube->info.texture[0], "../textures/wood.xpm", &img);
	load_image(cube, cube->info.texture[1], "../textures/colorstone.xpm", &img);
	load_image(cube, cube->info.texture[2], "../textures/bluestone.xpm", &img);
	load_image(cube, cube->info.texture[3], "../textures/redbrick.xpm", &img);
}

void	run_cube2(t_cube *cube)
{
	int	i;
	int	j;
	
	cube->info.mlx = mlx_init();

	orientation_init(cube);
	cube->info.posX = cube->ply.pos.x; //15.0;
	cube->info.posY = cube->ply.pos.y; //11.5;
	cube->info.dirX = 1.0;
	cube->info.dirY = 0.0;
	cube->info.planeX = 0.0;
	cube->info.planeY = 0.66;
	cube->info.re_buf = 0;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			cube->info.buf[i][j] = 0;
			j++;
		}
		i++;
	}

	cube->info.texture = (int **)malloc(sizeof(int *) * 4);
	if (!cube->info.texture)
		return ;
	i = 0;
	while ( i < 4)
	{
		cube->info.texture[i] = (int *)malloc(sizeof(int) * (texHeight * texWidth));
		if (!cube->info.texture[i])
			return ;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < texHeight * texWidth)
		{
			cube->info.texture[i][j] = 0;
			j++;
		}
		i++;
	}

	load_texture(cube);

	cube->info.moveSpeed = 0.05;
	cube->info.rotSpeed = 0.05;
	
	cube->info.win = mlx_new_window(cube->info.mlx, width, height, "mlx");

	cube->info.img.img = mlx_new_image(cube->info.mlx, width, height);
	cube->info.img.data = (int *)mlx_get_data_addr(cube->info.img.img, &cube->info.img.bpp, &cube->info.img.size_l, &cube->info.img.endian);

	mlx_loop_hook(cube->info.mlx, &main_loop, cube);
	mlx_hook(cube->info.win, 2, 1L << 0, &key_press, cube);
	mlx_hook(cube->info.win, 17, 0L, close_cube, cube);

	mlx_loop(cube->info.mlx);
}