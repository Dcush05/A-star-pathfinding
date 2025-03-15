#include "SDL3/SDL_render.h"
#include <stddef.h>
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_FRect mouseposrect;
	

}App;
App app = {0};
#define TILE_SIZE 32
#define MAP_WIDTH 25
#define MAP_HEIGHT 19
typedef struct
{
	int x,y;
}vec2i;
typedef struct
{
	vec2i pos;
	int f,g, h;
	struct Node* parent;
}Node;

int heuristic(vec2i a, vec2i b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}
Node** A_Star(Node* start, Node* end, int tiles[MAP_HEIGHT][MAP_WIDTH], int* pathlength)
{
	Node* open_list[MAP_HEIGHT * MAP_WIDTH];
	bool closed_list[MAP_HEIGHT][MAP_WIDTH] = {false};

	int open_list_size = 0;

	open_list[open_list_size++] = start;
	int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

	while(open_list_size > 0)
	{
		int min_f_index = 0;

		for(int i = 1; i < open_list_size; ++i)
		{
			if(open_list[i]->f < open_list[min_f_index]->f)
				min_f_index = i;
		}

		Node* current = open_list[min_f_index];
//		printf("{%d}\n", current->pos.x);
		if(current->pos.x == end->pos.x && current->pos.y == end->pos.y)
		{
			
			printf("meow10\n");
			Node** path = (Node**)malloc(sizeof(Node*) * MAP_HEIGHT * MAP_WIDTH); 
			int index = 0;
		//	printf("Index count: %d", index);
			while(current != NULL)
			{
				path[index++] = current;
				current = (Node*)current->parent;
			}
			*pathlength = index;
			printf("meow1\n");
			return path;
		}
		open_list[min_f_index] = open_list[--open_list_size];
		closed_list[current->pos.y][current->pos.x] = true;
		printf("Open list size: %d\n", open_list_size);
		
		for(int i = 0; i < 4; i++)
		{
			int nx = current->pos.x + directions[i][0];
			int ny = current->pos.y + directions[i][1];
			printf("nx, ny: {%d,%d}\n",nx, ny);
			
			if(nx >= 0 && ny >= 0 && nx < MAP_WIDTH && ny < MAP_HEIGHT && tiles[ny][nx] != 1 && !closed_list[ny][nx])
			{
				printf("hi\n");
				Node* neighbor = (Node*)malloc(sizeof(Node));
				neighbor->pos.x = nx;
				neighbor->pos.y = ny;
				neighbor->g = current->g + 1;
				neighbor->h = heuristic(neighbor->pos, end->pos);
				neighbor->f = neighbor->g + neighbor->h;
				neighbor->parent = (struct Node*)current;
				open_list[open_list_size++] = neighbor;
				//free(neighbor);
				//printf("meow\n");
			}
		}
	}


	*pathlength = 0;
	printf("No paths have been found\n");
	return NULL; //temp
}
//1 walls,0 empty space, 2 start point, 3 end point
int tiles[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
SDL_FRect tile_rects[MAP_HEIGHT][MAP_WIDTH];
vec2i start_point;
vec2i end_point;
SDL_FRect path_rects[MAP_WIDTH * MAP_HEIGHT];
Node start_node;
Node end_node;
int path_length;
Node** path;
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("SDL Hello World Example", "1.0", "com.example.sdl-hello-world");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 800, 600, SDL_WINDOW_RESIZABLE, &app.window, &app.renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;

    }
	SDL_SetRenderVSync(app.renderer, 1);

    //app.mouseposrect.x = app.mouseposrect.y = -1000;  /* -1000 so it's offscreen at start */
    //app.mouseposrect.w = app.mouseposrect.h = 50;
	for (size_t y = 0; y < MAP_HEIGHT; ++y)
	{
		for(size_t x = 0 ; x < MAP_WIDTH; ++x)
		{
			tile_rects[y][x].x = x * TILE_SIZE;
			tile_rects[y][x].y = y * TILE_SIZE;
			tile_rects[y][x].w = TILE_SIZE;
			tile_rects[y][x].h = TILE_SIZE;
			if(tiles[y][x] == 2)
			{	start_point.x = x;
				start_point.y = y;
			} else if(tiles[y][x] == 3)
			{
				end_point.x = x;
				end_point.y = y;
			}
				
			
		}
	}
	start_node = (Node){start_point, 0,0,0,NULL};
	end_node = (Node){end_point,0,0,0,NULL};

	path = A_Star(&start_node, &end_node, tiles, &path_length);

	
	printf("Start position: {%d, %d}\n", start_point.x, start_point.y);

	printf("End position: {%d, %d}\n", end_point.x, end_point.y);
	printf("Path length: %d\n", path_length);
	

    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:  /* triggers on last window close and other things. End the program. */
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:  /* quit if user hits ESC key */
            if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
                return SDL_APP_SUCCESS;
            }
		
            break;

        case SDL_EVENT_MOUSE_MOTION:  /* keep track of the latest mouse position */
            /* center the square where the mouse is */
            app.mouseposrect.x = event->motion.x - (app.mouseposrect.w / 2);
            app.mouseposrect.y = event->motion.y - (app.mouseposrect.h / 2);
            break;
    }
    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint8 r;
	//update and render here
	
/*	if(path != NULL)
	{
		for(int i = 0 ; i < path_length; ++i)
		{
				path_rects[i].x = path[i]->pos.x * TILE_SIZE;
				path_rects[i].y = path[i]->pos.y * TILE_SIZE;
				path_rects[i].w = TILE_SIZE;
				path_rects[i].h = TILE_SIZE;
		}
	}*/
    //fade between shades of red every 3 seconds, from 0 to 255. 
  //  r = (Uint8) ((((float) (SDL_GetTicks() % 3000)) / 3000.0f) * 255.0f);
    SDL_SetRenderDrawColor(app.renderer, 0,0, 0, 255);

    /* you have to draw the whole window every frame. Clearing it makes sure the whole thing is sane. */
    SDL_RenderClear(app.renderer);  /* clear whole window to that fade color. */

    /* set the color to white */
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);

    /* draw a square where the mouse cursor currently is. */
    SDL_RenderFillRect(app.renderer, &app.mouseposrect);
	if(path != NULL)
	{

		SDL_SetRenderDrawColor(app.renderer, 255,255,0,255);
		for(size_t i = 0; i < path_length; ++i)
		{	
				path_rects[i].x = path[i]->pos.x * TILE_SIZE;
				path_rects[i].y = path[i]->pos.y * TILE_SIZE;
				path_rects[i].w = TILE_SIZE;
				path_rects[i].h = TILE_SIZE;

		SDL_RenderFillRect(app.renderer, &path_rects[i]);
		}
	}
	for(size_t y = 0 ; y < MAP_HEIGHT; ++y)
	{
		for(size_t x = 0; x < MAP_WIDTH; ++x)
		{
			if(tiles[y][x] == 1)
			{
				SDL_SetRenderDrawColor(app.renderer, 255,255,255,255);
				SDL_RenderFillRect(app.renderer, &tile_rects[y][x]);
			} else if(tiles[y][x] == 2)
			{
				SDL_SetRenderDrawColor(app.renderer, 0,255,0,255);
				SDL_RenderFillRect(app.renderer, &tile_rects[y][x]);

			} else if(tiles[y][x] == 3)
			{
				SDL_SetRenderDrawColor(app.renderer, 255,0, 0, 255);
				SDL_RenderFillRect(app.renderer, &tile_rects[y][x]);
			}
				
		}
	}
	

    /* put everything we drew to the screen. */
    SDL_RenderPresent(app.renderer);

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
	free(path);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
