#ifndef WOLF3D_H
# define WOLF3D_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define FPS								60
#define SKIP_TICKS						1000000 / FPS
#define PI 								3.14159265
#define CUBE_SIZE						6
#define PT(x, y)						((t_point){x, y})
#define VEC(x, y)						((t_vector){x, y})
//#define PLAYER(pos, dir, fov, height)	((t_player){pos, dir, fov, height})
#define PLAYER(pos, dir)				((t_player){pos, dir})
#define VIEWPLANE(camera, height, width)		((t_viewplane){camera, height, width})
#define RAY(pos, dir)					((t_ray){pos, dir})
#define DISTANCE(p1, p2)				sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2))

#define MIN(a, b)						(a < b ? a : b)

typedef struct s_env
{
	SDL_Window		*screen;
	SDL_Renderer	*renderer;
} t_env;


typedef struct 	s_map
{
	char 		**map;
	int 		height;
	int 		width;
}				t_map;

typedef struct 	s_vector
{
	double		x;
	double		y;
}				t_vector;

typedef struct 	s_point
{
	double		x;
	double		y;
}				t_point;

typedef struct 	s_ray
{
	t_point 	origin;
	t_vector	direction;
}				t_ray;

typedef struct 	s_player
{
	t_point		position;
	t_vector	direction;
}				t_player;

/*typedef struct 	s_player
{
	t_point		position;
	double 		direction;
	int 		fov;
	int			height;
}				t_player;
*/
typedef struct 	s_viewplane
{
	t_vector	camera;
	int 		height;
	int 		width;
}				t_viewplane;

#endif