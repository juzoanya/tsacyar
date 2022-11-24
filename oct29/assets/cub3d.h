/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juzoanya <juzoanya@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 15:57:18 by euyi              #+#    #+#             */
/*   Updated: 2022/11/03 22:54:49 by juzoanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define PATH_LEN	1000
# define MAX_LINES	10000
# define MAX_CHARS	10000

# define WIN_WIDTH	768
# define WIN_HEIGHT	480

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include "./gcamerli/mlx.h"

/*********************************************************************************/
#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17
#define texWidth 64
#define texHeight 64
#define mapWidth 14
#define mapHeight 33
#define width 1080
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

typedef struct	s_info
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

}				t_info;
/************************************************************************************/

enum e_texture
{
	N = 0,
	S,
	W,
	E
};

typedef struct	s_vect
{
	double	x;
	double	y;
}				t_vect;

typedef struct	s_axis
{
	int	x;
	int	y;
}				t_axis;

/*typedef struct s_img
{
	char	*img;
	char	*addr;
	int		bbp;
	int		line_len;
	int		endian;
	t_axis	size;
}				t_img;*/

typedef struct s_player
{
	char	orient;			//stores the initial orientation of the player
	t_vect	pos;			//player position and also position of camera
	t_vect	step;			//what x/y direction to step into (either +1 or -1)
//	t_vect	dir;
}				t_player;

typedef struct	s_rays
{
	int		w_side;			//checks if a side wall is hit
	int		hit;
	t_vect	pos;
	t_vect	dir;
	t_vect	plane;
	t_vect	ray_dir;
	t_vect	side_dist;		//distance a ray has to travel from start position to x or y side. It changes as player/camera moves
	t_vect	delta_dist;		//distance ray has to travel from one x/y-side to the next x/y-side
	t_axis	map;			//current coordinates of the square on the map where a ray is
	double	perp_wall_dist;
	double	line_height;
	int		draw_start;
	int		draw_end;
	int		tex_x;			//x-coordinate of texture
	int		tex_y;
	double	tex_pos;
	double	wall_x;			//exact value where wall was hit. Needed to calculate the tex_x. Its always x-cordinate of texture but if side == 1, itsthe y-coordinate of the wall
	double	step;
}				t_rays;

typedef struct	s_cube
{
	void		*mlx;
	void		*window;

	char		tex_n[PATH_LEN];	// For storage of North texture filepath
	char		tex_s[PATH_LEN];	// For storage of South texture filepath
	char		tex_w[PATH_LEN];	// For storage of West texture filepath
	char		tex_e[PATH_LEN];	// For storage of East texture filepath
	int			col_f[4];			//[0]=255, [1]=123, [2]=254, [3]=NULL
	int			col_c[4];			//[0]=255, [1]=123, [2]=254, [3]=NULL
	int			tex_ind;			//value of texture index to be used.
	t_axis		win;				//MLX window width and height sizes
	t_img		img;
	t_img		texture[4];			//holds the strings for the file path of NO, SO, WE, EA textures.
	int			tex_width[4];
	int			tex_hght[4];
	t_player	ply;
	t_rays		rays;
	char		**map;				// 2D array holding full map info
	t_info		info;
}				t_cube;

// libft functions in libft_tools.c file
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *s, int c);
int		ft_atoi(const char *nptr);
int		ft_strstrchr(char *str, char *find);

// check_valid_scene.c, check_valid_scene_utils.c, check_valid_scene_utils_X.c
int		check_valid_scene(t_cube *cube, char *argv);
int		check_for(void *dest, char **rawdata, char *ptr, int *arr_pos);
int		verify_path(char *txt, const char *rdata);
int		verify_rgb(int *color, const char *rdata);
int		copy_map(t_cube *cube, char **rawdata);

int		line_is_empty(char *rdata);
int		exist_in_start(char *string, char *word);
void	copy_to_rawdata(char **rawdata, char *tmp);
void	mem_switch(char **rawdata, int flag);
int		valid_middle_map(char *rdata, int *nsew);
int		nsew_val_overflow(int *nsew);
int		wall_char_is_valid(const char *s);
int		no_wrong_element(char **rawdata, int arr_pos);
int		valid_top_bottom_map(char *rdata);
int		next_dec_point(const char *rdata);
int		valid_walls(char **rawdata, int max_array);
int		valid_surround(char **rawdata, int i, int j);
int		map_chars(char c);
int		map_is_valid(t_cube *cube, char **rawdata);
int		scene_is_valid(t_cube *cube, char **rawdata);
int		data_from_infile(t_cube *cube, char **rawdata, int fd);

void	run_cube(t_cube *cube);
void	run_cube2(t_cube *cube);

int		raycast(t_cube *cube);

void	ft_pixel_put(t_img *tex, int x, int y, unsigned int color);

void	draw_bg(t_cube *cube);

void	orientation_init(t_cube *cube);

int		close_cube(t_cube *cube);

#endif
