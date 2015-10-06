#include "Wolf3D.h"

t_env init(char *title, int height, int width)
{
	t_env env;

	env.screen = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_SHOWN);
	env.renderer = SDL_CreateRenderer(env.screen, -1, SDL_RENDERER_PRESENTVSYNC);
	if (env.screen == NULL || env.renderer == NULL)
	{
		printf("Impossible d'initialiser le mode écran à %d x %d: %s\n", width, 
				height, SDL_GetError());
		exit(1);
	}
	return (env);
}


void cleanup(t_env env)
{
	SDL_DestroyRenderer(env.renderer);
	env.renderer = NULL;
	SDL_DestroyWindow(env.screen);
	env.screen = NULL;
	SDL_Quit();
}


double	tcos(double angle)
{
	return (cos(angle * PI/180));
}

double	tsin(double angle)
{
	return (sin(angle * PI/180));
}

double	ttan(double angle)
{
	return (tan(angle * PI/180));
}

t_map 	*init_map(int height, int width)
{
	t_map 	*map;
	int 	i;
	int 	j;

	i = 0;
	map = (t_map *)malloc(sizeof(t_map));
	map->map = (char **)malloc(height * sizeof(char *));
	while (i < height)
	{
		j = 0;
		map->map[i] = (char *)malloc(width * sizeof(char));
		while (j < width)
		{
			map->map[i][j] = 0;
			j++;
		}
		i++;
	}
	map->height = height;
	map->width = width;
	return (map);
}
/*
t_point 	look_for_horizontal_intersection(t_map *map, t_player player, double angle)
{
	t_point	first_intersection;
	t_point next_intersection;
	int xA;
	int yA;



	if (tsin(angle) > 0)
	{
		yA = -(1 << CUBE_SIZE);
		first_intersection.y = (((int)player.position.y >> CUBE_SIZE) << CUBE_SIZE) - 1;
	}
	else
	{
		yA = 1 << CUBE_SIZE;
		first_intersection.y = (((int)player.position.y >> CUBE_SIZE) << CUBE_SIZE) + (1 << CUBE_SIZE);
	}
	if (tcos(angle) > 0)
		xA = (1 << CUBE_SIZE) / ttan(angle);
	else
		xA = -((1 << CUBE_SIZE) / ttan(angle));
	//printf("position.x:%d position.y:%d A.y: %d\n", player.position.x, player.position.y, first_intersection.y);
	//	printf("Xa:%d Xy:%d\n", xA, yA);
	first_intersection.x = player.position.x + (player.position.y - first_intersection.y) / ttan(angle);
	//printf("A.x:%d\n", first_intersection.x);
	next_intersection = first_intersection;
	while ((next_intersection.x >> 6) < map->width && (next_intersection.y >> 6) < map->height
			&& next_intersection.x >= 0 && next_intersection.y >= 0)
	{
		//printf("n.x:%d n.y:%d\n", next_intersection.x >> 6, next_intersection.y >> 6);
		if (map->map[next_intersection.y >> 6][next_intersection.x >> 6] != 0)
			return (next_intersection);
		next_intersection.x += xA;
		next_intersection.y += yA;
	}
	return (PT(42424242, 42424242));
}

t_point 	look_for_vertical_intersection(t_map *map, t_player player, double angle)
{
	t_point	first_intersection;
	t_point next_intersection;
	int xA;
	int yA;

	if (tcos(angle) > 0)
	{
		xA = (1 << CUBE_SIZE);
		first_intersection.x = (((int)player.position.x >> CUBE_SIZE) << CUBE_SIZE) + (1 << CUBE_SIZE);
	}
	else
	{
		xA = -(1 << CUBE_SIZE);
		first_intersection.x = (((int)player.position.y >> CUBE_SIZE) << CUBE_SIZE) - 1;
	}
	if (tsin(angle) > 0)
		yA = -((1 << CUBE_SIZE) * ttan(angle));
	else
		yA = (1 << CUBE_SIZE) * ttan(angle);
//	printf("position.x:%d position.y:%d A.x: %d\n", player.position.x, player.position.y, first_intersection.x);
	
	first_intersection.y = player.position.y + (player.position.x - first_intersection.x) * ttan(angle);
//	printf("x:%d y:%d\n", first_intersection.x, first_intersection.y);
	first_intersection.x = player.position.x + (player.position.y - first_intersection.y) / ttan(angle);
	next_intersection = first_intersection;
//	printf("n.x:%d n.y:%d\n", next_intersection.x >> 6, next_intersection.y >> 6);
	while ((next_intersection.x >> 6) < map->width && (next_intersection.y >> 6) < map->height
			&& next_intersection.x >= 0 && next_intersection.y >= 0)
	{
//		printf("n.x:%d n.y:%d\n", next_intersection.x >> 6, next_intersection.y >> 6);
		if (map->map[next_intersection.y >> 6][next_intersection.x >> 6] != 0)
			return (next_intersection);
		next_intersection.x += xA;
		next_intersection.y += yA;
	}
	return (PT(42424242, 42424242));
}

double		get_closest_intersection(t_map *map, t_player player, double angle)
{
	t_point horizontal_intersection;
	t_point vertical_intersection;
	double 	horizontal_distance;
	double	vertical_distance;

	horizontal_intersection = look_for_horizontal_intersection(map, player, angle);
	vertical_intersection = look_for_vertical_intersection(map, player, angle);
	if (horizontal_intersection.x == 42424242)
	{ 
		if (vertical_intersection.x == 42424242)
			return (-1);
		return (DISTANCE(player.position, vertical_intersection));
	}
	else if (vertical_intersection.x == 42424242)
		return (DISTANCE(player.position, horizontal_intersection));
	horizontal_distance = DISTANCE(player.position, horizontal_intersection);
	vertical_distance = DISTANCE(player.position, vertical_intersection);
	return (MIN(horizontal_distance, vertical_distance));
}

int		draw_column(t_env env, t_viewplane viewplane, int column, double distance, int player_viewplane_distance)
{
	int wall_height;
	int start;
	int i;

	wall_height = (int)(64 / distance * player_viewplane_distance);

	start = (viewplane.height >> 1) - (wall_height >> 1);
	i = 0;

	while (i < viewplane.height)
	{
		if (i >= start && i <= start + wall_height)
			SDL_SetRenderDrawColor(env.renderer, 0, 0, 255, 0);
		else
			SDL_SetRenderDrawColor(env.renderer, 255, 255, 255, 0);
		SDL_RenderDrawPoint(env.renderer, column, i);
		i++;
	}
	return (start + wall_height);
}

double	find_distance_to_floor(t_player player, t_viewplane viewplane, int player_viewplane_distance, int row)
{
	double distance;

	//distance = player_viewplane_distance * (player.height / MIN(row - (viewplane.height >> 1), 1));
	return (0);
}

void	draw_floor(t_env env, t_viewplane viewplane, int column, int start)
{
	int i;

	i = start;
	while (i < viewplane.height)
	{
		SDL_SetRenderDrawColor(env.renderer, 0, 255, 0, 0);
		SDL_RenderDrawPoint(env.renderer, column, i);
		i++;
	}
}

int		main(void)
{
	t_map			*map;
	t_player 		player;
	t_viewplane 	viewplane;
	int 			player_viewplane_distance;
	double			ray_angle;
	double			angle_increment;
	int				i;
	double distance;
	SDL_Event e;
	
	map = init_map(10, 10);
	player = PLAYER(PT(64, 128 + 64 * 5), 90, 60, ((6 << CUBE_SIZE) >> 1));
	map->map[0][0] = 1;
	map->map[0][1] = 1;
	map->map[0][2] = 1;
	map->map[0][3] = 1;
//	map->map[1][2] = 1;
//	map->map[2][2] = 1;
//	map->map[2][3] = 1;
//	map->map[3][3] = 1;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	viewplane = VIEWPLANE(320, 320);
	t_env env = init("Wolf3D", viewplane.height, viewplane.width);
	player_viewplane_distance = (viewplane.width / 2) / ttan(player.fov >> 1);
	

	char continuer = 1;
	while (continuer)
	{
//	SDL_SetRenderDrawColor(env.renderer, 0, 255, 255, 255);
//	SDL_RenderClear(env.renderer);
	ray_angle = player.direction - (player.fov >> 1);
	angle_increment = ((double)player.fov / viewplane.width);
	
	i = 0;
	ray_angle = player.direction - (player.fov >> 1);
	while (i <= viewplane.width)
	{
		printf("angle:%lf x:%d y:%d\n", player.direction, player.position.x, player.position.y);
		int start;
		if ((distance = get_closest_intersection(map, player, ray_angle)) > 0)
		{
//			distance *= tcos(player.direction - ray_angle);
			start = draw_column(env, viewplane, i, distance, player_viewplane_distance);
		}
		else	draw_column(env, viewplane, i, 0, player_viewplane_distance);
		i++;
		ray_angle += angle_increment;
	}
	SDL_RenderPresent(env.renderer);

		SDL_WaitEvent(&e);
		if(e.window.event == SDL_WINDOWEVENT_CLOSE)
			continuer = 0;
		if (e.type == SDL_KEYDOWN)
		{
			if (keys[SDL_SCANCODE_DOWN])
			{
				player.position.x += (int)(10 * tcos(player.direction));
				player.position.y += (int)(10 * tsin(player.direction));
			}
			if (keys[SDL_SCANCODE_UP])
			{
				player.position.x -= (int)(10 * tcos(player.direction));
				player.position.y -= (int)(10 * tsin(player.direction));

			}
			if (keys[SDL_SCANCODE_LEFT])
				player.direction -= 2;
			if (keys[SDL_SCANCODE_RIGHT])
				player.direction += 2;


				
		//	else
		//		continuer = 0;
		}

	}
	cleanup(env);
	return (0);
}*/

int		draw_column(t_env env, t_viewplane viewplane, int column, double distance, int color)
{
	int wall_height;
	int start;


	wall_height = abs(viewplane.height / distance);

	start = (viewplane.height >> 1) - (wall_height >> 1);
	SDL_SetRenderDrawColor(env.renderer, (color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, color & 0x0000FF, 0);
	if (start < 0) start = 0;
    SDL_RenderDrawLine(env.renderer, column, start, column, start + wall_height + 1);
	return (start + wall_height);
}

t_vector		rotate_vector(t_vector v, double angle)
{
	t_vector 	new;
	double		cs;
	double 		sn;

	cs = tcos(angle);
	sn = tsin(angle);
	new.x = v.x * cs - v.y * sn;
	new.y = v.x * sn + v.y * cs;
	return (new);
}

int main(void)
{
	int map[24][24]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,1},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
  {4,0,4,0,0,0,0,2,2,2,2,2,2,2,2,2,1,1,0,1,1,1,1,1},
  {4,0,2,0,0,0,0,2,0,2,0,2,0,2,0,2,1,0,0,0,1,1,1,1},
  {4,0,3,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,4},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
  {4,0,4,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,4},
  {4,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,1,1,1,1},
  {4,0,0,0,0,0,0,2,2,2,2,0,2,2,2,2,1,1,1,1,1,1,1,1},
  {3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {3,3,3,3,3,3,0,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3},
  {4,4,4,4,4,4,0,4,4,4,3,0,3,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,3,0,3,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,2,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,3,0,3,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,3,0,3,0,0,0,0,4,3,0,0,0,0,0,2,0,0,0,0,0,0,2},
  {4,0,0,2,0,0,0,0,0,4,3,0,3,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,3,0,3,0,0,0,0,4,3,0,3,2,0,0,2,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,3,0,3,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};


	int 		i;
	t_ray		ray;
	t_env		env;
	SDL_Event 	e;
	t_player 	player;
	t_viewplane viewplane;
	t_vector 	initSideDist;
	t_vector	sideDistInc;
	t_point		mapPos;
	t_vector	rayDir;
	t_vector	incDir;
	double		cameraX;
	char		hit;
	char 		side;
	char		done;

	done = 0;
	player = PLAYER(PT(22, 12), VEC(-1, 0));
	viewplane = VIEWPLANE(VEC(0, 0.6f), 800, 800);
	env = init("Wolf3D", viewplane.height, viewplane.width);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	while (!done)
	{
		clock_t start = clock();
		SDL_SetRenderDrawColor(env.renderer, 255, 255, 255, 0);
		SDL_RenderClear( env.renderer );
		i = 0;
		while (i < viewplane.width)
		{
			t_vector	rayDir;
			cameraX = 2 * i / (double)viewplane.width - 1;
			rayDir = VEC(player.direction.x + viewplane.camera.x * cameraX, player.direction.y + viewplane.camera.y * cameraX);
			ray = RAY(player.position, rayDir);

			sideDistInc = VEC(sqrt(1 + pow(ray.direction.y, 2) / pow(ray.direction.x, 2)), sqrt(1 + pow(ray.direction.x, 2) / pow(ray.direction.y, 2)));
			mapPos = PT((int)ray.origin.x, (int)ray.origin.y);
   			if (ray.direction.x < 0)
     	 	{
    	 	   incDir.x = -1;
    		   initSideDist.x = (ray.origin.x - mapPos.x) * sideDistInc.x;
     		}
    	 	else
    		{
  		   		incDir.x = 1;
  		    	initSideDist.x = (mapPos.x + 1.0 - ray.origin.x) * sideDistInc.x;
 		    }
 		    if (ray.direction.y < 0)
 		    {
 		    	incDir.y = -1;
  		    	initSideDist.y = (ray.origin.y - mapPos.y) * sideDistInc.y;
    		}
  		    else
   		   	{
  		    	incDir.y = 1;
    	 		initSideDist.y = (mapPos.y + 1.0 - ray.origin.y) * sideDistInc.y;
    	 	}
    	 	hit = 0;
    	 	while (!hit && mapPos.x >= 0 && mapPos.x < 24 && mapPos.y >= 0 && mapPos.y < 24)
   		  	{
     			if (initSideDist.x < initSideDist.y)
     			{
     				initSideDist.x += sideDistInc.x;
     				mapPos.x += incDir.x;
     				side = 0;
     			}
     			else
    	 		{
     				initSideDist.y += sideDistInc.y;
     				mapPos.y += incDir.y;
     				side = 1;
     			}
     			if (map[(int)mapPos.x][(int)mapPos.y] > 0)
     				hit = 1;
     		}
     		double distance;
     		if (side == 0)
     			distance = fabs((mapPos.x - ray.origin.x + (1 - incDir.x) / 2) / ray.direction.x);
     		else
     			distance = fabs((mapPos.y - ray.origin.y + (1 - incDir.y) / 2) / ray.direction.y);
   
     		switch(map[(int)mapPos.x][(int)mapPos.y])
     		{
     			case 1: draw_column(env, viewplane, i, distance, 0xFF0000 >> (side)); break;
     			case 2: draw_column(env, viewplane, i, distance, 0x00FF00 >> (side)); break;
     			case 3: draw_column(env, viewplane, i, distance, 0x0000FF >> (side)); break;
    			case 4: draw_column(env, viewplane, i, distance, 0xF0F000 >> (side)); break;
     			default: draw_column(env, viewplane, i, distance, 0xFFFFFF >> (side)); break;
     		}
			i++;
		}
		SDL_RenderPresent(env.renderer);
		clock_t end = clock();
		int sleepTime = start - end + SKIP_TICKS;
		printf("SleepTime:%d\n", sleepTime);
		if (sleepTime > 0)
			usleep(sleepTime);
		SDL_WaitEvent(&e);
		if(e.window.event == SDL_WINDOWEVENT_CLOSE)
			done = 1;

		if (keys[SDL_SCANCODE_LEFT])
			{
				if(map[(int)(player.position.x - viewplane.camera.x / 5)][(int)player.position.y] == 0) 
					player.position.x -= viewplane.camera.x / 5;
				if(map[(int)player.position.x][(int)(player.position.y - viewplane.camera.y / 5)] == 0) 
				player.position.y -= viewplane.camera.y / 5;
			}
			if (keys[SDL_SCANCODE_RIGHT])
			{
				if(map[(int)(player.position.x + viewplane.camera.x / 5)][(int)player.position.y] == 0) 
					player.position.x += viewplane.camera.x / 5;
				if(map[(int)player.position.x][(int)(player.position.y + viewplane.camera.y / 5)] == 0)
					player.position.y += viewplane.camera.y / 5;
			}
			if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
			{
				if(map[(int)(player.position.x - player.direction.x / 5)][(int)player.position.y] == 0) 
					player.position.x -= player.direction.x / 5;
				if(map[(int)player.position.x][(int)(player.position.y - player.direction.y / 5)] == 0) 
				player.position.y -= player.direction.y / 5;
			}
			if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
			{
				if(map[(int)(player.position.x + player.direction.x / 5)][(int)player.position.y] == 0) 
					player.position.x += player.direction.x / 5;
				if(map[(int)player.position.x][(int)(player.position.y + player.direction.y / 5)] == 0)
					player.position.y += player.direction.y / 5;
			}
			if (keys[SDL_SCANCODE_D])
			{
				player.direction = rotate_vector(player.direction, -3);
				viewplane.camera = rotate_vector(viewplane.camera, -3);				
			}
			//	player.direction -= 2;
			if (keys[SDL_SCANCODE_A])
			{
				player.direction = rotate_vector(player.direction, 3);
				viewplane.camera = rotate_vector(viewplane.camera, 3);	
			}
			//	player.direction += 2;
			if (keys[SDL_SCANCODE_ESCAPE])
				done = 1;
			printf("%d %d\n", keys[SDL_SCANCODE_LEFT], keys[SDL_SCANCODE_RIGHT]);
		
	}






































	


	return (0);
}

