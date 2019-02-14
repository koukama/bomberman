/*
** functions.h for bomberman in /home/infitek/Documents/Etna/Bomberman
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Fri May  4 16:41:15 2018 REBOUL Simon
** Last update Thu Feb 14 11:15:15 2019 REBOUL Simon
*/

#ifndef __FUNCTIONS_H__
# define __FUNCTIONS_H__
#include "network.h"
#include "bomberman.h"

void			my_putstr_color(const char *color, const char *str);
char			*my_strdup(const char *str);
int			my_strcmp(const char *s1, const char *s2);
char			*my_strcat(char *src, char *dest);
void			my_putstr(const char *str);
int			my_strlen(const char *str);
void			my_putchar(const char c);
char			*readline();
void			init_startup(t_start_screen *start_screen);
void			init_game(t_game *game, t_start_screen *start_screen);
t_player		*init_player(t_game *game, int socket);
void			init_graphics(t_game *game);
void			init_ui(t_game *game);
SDL_Renderer    	*init_SDL(t_start_screen *start_screen);
void			apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
void			gere_display(t_game *game);
void			gere_display_ui(t_game *game);
SDL_Rect		*gere_anim(t_player *player);
void			gere_anim_bomb(t_object *bomb);
void			gere_anim_bonus(t_object *bonus);
void			gere_anim_flames(t_object *flame);
void			gere_anim_destruction_effect(t_object *flame);
void			gere_gameplay(t_game *game);
void			gere_keyboard(t_game * game, t_player *player, SDL_Event event);
void			gere_keyboard_network(t_game * game, t_player *player, int dir);
void			gere_ia(t_game *game, t_player *ia);
int			gere_intro(t_start_screen *start_screen, t_game *game, SDL_Event event);
int			Collision(SDL_Rect *box1, SDL_Rect *box2);
void			displayText(t_game *game, char *str);
void			displayTextAt(t_game *game, SDL_Surface *obj, char *str, int x, int y);
void			my_put_nbr(int nb);
void			drawBoundingBoxes(t_game *game, SDL_Rect *box);
void			load_map(t_game *game, char *path, int layer);
void			add_objects(t_game *game, int tile_index, int x, int y, int layer);
void			add_object(t_game *game, int tile_index, int x, int y, int layer);
void			add_falling_object(t_game *game, int tile_index, int x, int y, int layer, int offset);
void			add_solid_object(t_game *game, int tile_index, int x, int y, int destructible);
int			get_x_pos(int width, int height, int tilesize, int tilenumber);
int			get_y_pos(int width, int height, int tilesize, int tilenumber);
int			my_ceil(double x);
t_game			*recheck_z_index(t_game *game);
void			add_bomb(t_game * game, t_player *player);
void			add_destroy_effect(t_game *game, t_object *object, int _canSpawnBonus);
int			add_flame(t_game * game, t_object *bomb, int flame_dir, int distance_from_origin);
void			check_flames_position(t_game *game, t_object *bomb);
int			abs(int nb);
int			add_container_to_game(t_game *game);
void			add_object_to_container(t_game *game, t_object *object);
void			del_object_from_container(t_game *game, t_object *object);
void			check_bomb_collision(t_game * game, t_player *player);
void			check_timers(t_game * game);
char			*get_time_left(t_game *game);
void			check_if_bonus(t_game *game, SDL_Rect *position, t_object *toDelete);
void			add_bonus(t_game *game, int index, SDL_Rect *position);
void			TimeOut(t_game *game);
void			displayMap(t_game *game);
int 			create_bind_listen();
void 			*accept_new_client(void *_game); // Threaded function
void			initiate_connection(t_game *game, char *server);
void 			*listen_client(void *_dude); // Threaded function
void 			*send_data_server(void *_game); // Threaded function
void			send_data_client(int socket, const char *com);
void 			*recieve_data_client(void *_cli_struct); // Threaded function
t_player 		*check_is_user_already_exist(t_game *game, int number);
void			cli_game_loop(t_game *game, int cli_sock);

#endif /*__FUNCTIONS_H__*/
