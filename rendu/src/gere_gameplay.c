/*
** gere_gameplay.c for bomberman in /home/infitek/Documents/Etna/Bomberman
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Sat May  5 14:55:43 2018 REBOUL Simon
** Last update Thu Feb 14 14:22:08 2019 REBOUL Simon
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/functions.h"
#include "../headers/bomberman.h"

void	gere_gameplay(t_game *game)
{
  t_player	*player;
  int		dead = game->playersPtr->nb_players;
  int		winner = 0;
  printf("before dead = %d\n", dead);
  for (int i = 0; game->players[i] != NULL; i++)
    {
      if (game->players[i]->is_dead == 0)
	{
	  winner = game->players[i]->id;
	  dead--;
	}
    }
  printf("after dead = %d\n", dead);
  if (dead >= game->playersPtr->nb_players)
    {
      SDL_Delay(2000);
      char *path = malloc(sizeof(char*) * 15);
      char *win = malloc(sizeof(char) * 2);
      strcpy(path,"./images/winplayer");
      sprintf(win, "%d", winner);
      path = my_strcat(path, win);
      path = my_strcat(path, ".png");
      printf("%s\n", path);
      game->start_screen->winPlayerSprite = IMG_Load(path);
      game->start_screen->winPlayerTexture = SDL_CreateTextureFromSurface(game->start_screen->screen, game->start_screen->winPlayerSprite);
      game->start_screen->audio->music = Mix_LoadMUS("./music/menu.wav");
      Mix_VolumeMusic(MIX_MAX_VOLUME/2);
      Mix_PlayMusic(game->start_screen->audio->music, -1);
      game->start_screen->has_started = 0;
      game->start_screen->victory_screen = 1;
      close(game->socket);
    }
  
  for (int i = 0; game->players[i] != NULL; i++)
    {

      player = game->players[i];
      player->position->hitbox->x = player->position->pos->x + 6;
      player->position->hitbox->y = player->position->pos->y + (29 * SCALE);
      if (player->is_ia == 1 && player->is_dead == 0)
	gere_ia(game, player);
      if (player->position->last_dir != player->position->dir)
	player->position->last_dir = player->position->dir;
      if (player->position->dir != player->position->last_dir && player->anim_tick > 0)
	player->anim_tick = 0;
      player->walking = 0;
      

      if (player->position->hitbox->x > 15 * MAP_MARGIN_X)
	player->position->pos->x = -player->position->hitbox->w;
	if (player->position->hitbox->x + player->position->hitbox->w < 0)
	player->position->pos->x = 15 * MAP_MARGIN_X - 6;
	if (player->position->hitbox->y > 16 * MAP_MARGIN_Y)
	player->position->pos->y = -player->position->hitbox->h - 16;
	if (player->position->hitbox->y + player->position->hitbox->h < 0)
	player->position->pos->y = 15 * MAP_MARGIN_Y - 6;
      
      
      if (((player->position->xvel > 0 /*&& player->position->hitbox->x < ((14 * MAP_MARGIN_X) - player->position->hitbox->w*/)
	   || (player->position->xvel < 0 /*&& player->position->hitbox->x > MAP_MARGIN_X*/)) && player->is_sliding == 0)
	{
	  player->walking = 1;
	  player->position->pos->x += player->position->xvel;
	}
      if (((player->position->yvel > 0 /*&& player->position->hitbox->y < ((14 * MAP_MARGIN_Y) - player->position->hitbox->h*/)
	   || (player->position->yvel < 0 /*&& player->position->hitbox->y > (MAP_MARGIN_Y)*/ )) && player->is_sliding == 0)
	{
	  player->walking = 1;
	  player->position->pos->y += player->position->yvel;
	}
      check_bomb_collision(game, player);
    }
  check_timers(game);
}

void	check_bomb_collision(t_game *game, t_player *player)
{
 player->is_sliding = 0;
  t_object *next_obj = NULL;
  for (t_object *object = game->container->first; object != NULL;)
    {
      next_obj = object->next;
      if (object->is_solid == 1 && Collision(player->position->hitbox, object->hitbox) == 1 && object->is_falling == 0)
	{
	  if (object->is_bonus == 1)
	    {
	      if (object->nbBombsEffect < 0 || object->rangeEffect < 0 || object->speedEffect < 0)
		Mix_PlayChannel(-1, game->audio->malusGet, 0);
	      else
		Mix_PlayChannel(-1, game->audio->bonusGet, 0);
	      if (player->nb_bombs + object->nbBombsEffect > 0 && player->nb_bombs + object->nbBombsEffect <= 10)
		player->nb_bombs += object->nbBombsEffect;
	      if (player->flame_range + object->rangeEffect >= 2 && player->flame_range + object->rangeEffect <= 10)
		player->flame_range += object->rangeEffect;
	      if (player->speed + object->speedEffect >= -1 && player->speed + object->speedEffect <= 2)
		player->speed += object->speedEffect;
	      printf("Bonus got, new stats :\nnbbombs = %d\nrange = %d\nspeed = %f\n", player->nb_bombs, player->flame_range, player->speed);
	      del_object_from_container(game, object);
	    }
	  if (player->position->hitbox->x > object->hitbox->x + object->hitbox->w / 2
	      && player->position->hitbox->y + player->position->hitbox->h > object->hitbox->y + (WALK_SPEED + player->speed)
	      && player->position->hitbox->y < (object->hitbox->y + object->hitbox->h) - (WALK_SPEED - player->speed))
	    {
	      player->position->pos->x += (WALK_SPEED + player->speed);
	    }
	  else if (player->position->hitbox->x + player->position->hitbox->w <= object->hitbox->x + object->hitbox->w 
		   && player->position->hitbox->y + player->position->hitbox->h > object->hitbox->y + (WALK_SPEED + player->speed)
		   && player->position->hitbox->y < (object->hitbox->y + object->hitbox->h) - (WALK_SPEED + player->speed))
	    {
	      player->position->pos->x -= (WALK_SPEED + player->speed);
	    }
	  if (player->position->hitbox->y + player->position->hitbox->h < object->hitbox->y + object->hitbox->h / 2
	      && player->position->hitbox->x + player->position->hitbox->w > object->hitbox->x + (WALK_SPEED + player->speed)
	      && player->position->hitbox->x < (object->hitbox->x + object->hitbox->w) - (WALK_SPEED - player->speed))
	    {
	      player->position->pos->y -= (WALK_SPEED + player->speed);
	    }
	  else if (player->position->hitbox->y > object->hitbox->y + object->hitbox->h / 2
		   && player->position->hitbox->x + player->position->hitbox->w > object->hitbox->x + (WALK_SPEED + player->speed)
		   && player->position->hitbox->x < (object->hitbox->x + object->hitbox->w) - (WALK_SPEED - player->speed))
	    {
	      player->position->pos->y += (WALK_SPEED + player->speed);
	    }
	}
      if (object->is_bomb == 1)
	{
	  if (object->is_solid == 0 && Collision(player->position->hitbox, object->hitbox) == 0 && player->id == object->player_id)
	    object->is_solid = 1;
	}
      else if (object->deadly == 1 && object->is_falling == 0)
	{
	  if (Collision(player->position->hitbox, object->hitbox) == 1 && player->is_dead == 0)
	    {
	      Mix_PlayChannel(-1, game->audio->playerDead, 0);
	      player->position->yvel = 0;
	      player->position->xvel = 0;
	      player->anim_index = 0;
	      player->anim_tick = -30;
	      player->walking = 0;
	      player->is_dead = 1;
	    }
	}
      else if (object->is_flame == 1)
	{
	  SDL_Rect *hitbox_flame = malloc(sizeof(SDL_Rect));;
	  hitbox_flame->w = 30;
	  hitbox_flame->h = 28;
	  hitbox_flame->x = object->hitbox->x;
	  hitbox_flame->y = object->hitbox->y + 2;
	  if (Collision(player->position->hitbox, hitbox_flame) == 1 && player->is_dead == 0)
	    {
	      game->ui->playerPortraitTilePos[player->id - 1]->y = 32;
	      Mix_PlayChannel(-1, game->audio->playerDead, 0);
	      player->position->yvel = 0;
	      player->position->xvel = 0;
	      player->anim_index = 0;
	      player->anim_tick = -30;
	      player->walking = 0;
	      player->is_dead = 1;
	    }
	}
      if (object->is_ice == 1 && Collision(player->position->hitbox, object->hitbox) == 1)
	{
	  if (player->position->second_dir == 1)
	    {
	      player->is_sliding = 1;
	      player->position->xvel = 0;
	      player->position->yvel = 0;
	      player->position->pos->x -= 2;
	    }
	  else if (player->position->second_dir == 3 && player->position->hitbox->y  + player->position->hitbox->h / 2 >= object->hitbox->y)
	    {
	      player->is_sliding = 1;
	      player->position->xvel = 0;
	      player->position->yvel = 0;
	      player->position->pos->x += 2;
	    }
	  else if (player->position->second_dir == 2)
	    {
	      player->is_sliding = 1;
	      player->position->xvel = 0;
	      player->position->yvel = 0;
	      player->position->pos->y += 2;
	    }
	  
	  else if (player->position->second_dir == 5)
	    {
	      player->is_sliding = 1;
	      player->position->xvel = 0;
	      player->position->yvel = 0;
	      player->position->pos->y -= 2;
	    }
	}
      object = next_obj;
    }
}

void	check_timers(t_game *game)
{
  t_object *next_obj = NULL;
  t_player *player;
  int      index;

  for (int j = 0; game->players[j]; j++)
    {
      if ((j < 4 && game->players[j + 1] && Collision(game->players[j]->position->pos, game->players[j + 1]->position->pos) == 1)
	  || (j < 3 && game->players[j + 2]&& Collision(game->players[j]->position->pos, game->players[j + 2]->position->pos) == 1)
	  || (j < 2 && game->players[j + 3] && Collision(game->players[j]->position->pos, game->players[j + 3]->position->pos) == 1))
	{
	  game = recheck_z_index(game);
	}
    }
  
  for (t_object *object = game->container->first; object != NULL;)
    {
      next_obj = object->next;

      if (object->is_bomb == 1)
	{
	  if (object->timer > 0)
	    object->timer--;
	  else if (object->timer == 0)
	    {
	      check_flames_position(game, object);
	      object->timer--;
	      del_object_from_container(game, object);
	    }
	}
       else if (object->is_flame == 1)
	{
	  if (object->timer > 0)
	    object->timer--;
	  else if (object->timer == 0)
	    {
	      if (object->distance_from_origin == 0)
		{
		  for (index = 0; game->players[index]->id != object->player_id; index++);
		  player = game->players[index];
		  player->actual_bombs--;
		  player->lastBombHasExploded = 1;
		}
	      object->timer--;
	      game->map[object->y][object->x] = '0';
	      if (object->y < 15 && game->map[object->y + 1][object->x] == 'B')
		game->map[object->y + 1][object->x] = '0';
	      if (object->y > 0 && game->map[object->y - 1][object->x] == 'B')
		game->map[object->y - 1][object->x] = '0';
	      if (object->x < 15 && game->map[object->y][object->x + 1] == 'B')
		game->map[object->y][object->x + 1] = '0';
	      if (object->x > 0 && game->map[object->y][object->x - 1] == 'B')
		game->map[object->y][object->x - 1] = '0';

	      if (object->y < 14 && game->map[object->y + 2][object->x] == 'B')
		game->map[object->y + 2][object->x] = '0';
	      if (object->y > 1 && game->map[object->y - 2][object->x] == 'B')
		game->map[object->y - 2][object->x] = '0';
	      if (object->x < 14 && game->map[object->y][object->x + 2] == 'B')
		game->map[object->y][object->x + 2] = '0';
	      if (object->x > 1 && game->map[object->y][object->x - 2] == 'B')
		game->map[object->y][object->x - 2] = '0';

	      if (object->y < 13 && game->map[object->y + 3][object->x] == 'B')
		game->map[object->y + 3][object->x] = '0';
	      if (object->y > 2 && game->map[object->y - 3][object->x] == 'B')
		game->map[object->y - 3][object->x] = '0';
	      if (object->x < 13 && game->map[object->y][object->x + 3] == 'B')
		game->map[object->y][object->x + 3] = '0';
	      if (object->x > 2 && game->map[object->y][object->x - 3] == 'B')
		game->map[object->y][object->x - 3] = '0';
	      del_object_from_container(game, object);
	    }
	}
      else if (object->is_destruction_effect == 1)
	{
	  if (object->timer > 0)
	    object->timer--;
	  else if (object->timer == 0)
	    {
	      object->timer--;
	      SDL_Rect *tmp = malloc(sizeof(SDL_Rect));
	      tmp->x = object->hitbox->x;
	      tmp->y = object->hitbox->y;
	      tmp->w = TILE_SIZE;
	      tmp->h = TILE_SIZE;
	      if (object->canSpawnBonus == 1)
		check_if_bonus(game, tmp, object);
	      else
		del_object_from_container(game, object);
	    }
	}
      object = next_obj;
    }
}

void	check_if_bonus(t_game *game, SDL_Rect *position, t_object *toDelete)
{
  int shouldAppear = rand()%(3);
  int index = rand()%(6);

  if (shouldAppear == 1)
    add_bonus(game, index, position);
  del_object_from_container(game, toDelete);
}

void	add_bonus(t_game *game, int index, SDL_Rect *position)
{
  t_object	*bonus = malloc(sizeof(t_object) + 1);
  
  Mix_PlayChannel(-1, game->audio->dropBomb, 0);
  bonus->is_bonus = 1;
  bonus->sprite_tex = game->items_tex;
  bonus->tile_rect = malloc(sizeof(SDL_Rect) + 1);
  bonus->hitbox = malloc(sizeof(SDL_Rect) + 1);
  bonus->tile_rect->x = index * TILE_SIZE;
  bonus->tile_rect->y = 0;
  bonus->tile_rect->w = TILE_SIZE;
  bonus->tile_rect->h = TILE_SIZE;
  bonus->hitbox->x = position->x;
  bonus->hitbox->y = position->y;
  bonus->hitbox->w = TILE_SIZE;
  bonus->hitbox->h = TILE_SIZE;
  bonus->nbBombsEffect = 0;
  bonus->rangeEffect = 0;
  bonus->speedEffect = 0;
  bonus->is_falling = 0;
  bonus->deadly = 0;
  switch (index)
    {
    case 0:
      bonus->nbBombsEffect = 1;
      break;
    case 1:
      bonus->speedEffect = 1;
      break;
    case 2:
      bonus->rangeEffect = 1;
      break;
    case 3:
      bonus->nbBombsEffect = -1;
      break;
    case 4:
      bonus->speedEffect = -1;
      break;
    case 5:
      bonus->rangeEffect = -1;
      break;
    default:
      bonus->nbBombsEffect = 0;
      bonus->rangeEffect = 0;
      bonus->speedEffect = 0;
      break;
    }
  bonus->is_over_player = 0;
  bonus->is_solid = 1;
  bonus->is_destructible = 1;
  bonus->is_bomb = 0;
  bonus->is_flame = 0;
  bonus->anim_index = 0;
  bonus->anim_tick = 0;
  bonus->anim_duration = ANIM_SPEED / 3;
  bonus->canSpawnBonus = 0;
  bonus->next = NULL;
  bonus->prev = NULL;
  bonus->parent = NULL;
  add_object_to_container(game, bonus);
}

void	add_bombs_on_map(t_game *game, t_object *bomb)
{
  game->map[bomb->y][bomb->x] = 'B';
  if (bomb->y > 0 && game->map[bomb->y - 1][bomb->x] == '0')
    game->map[bomb->y - 1][bomb->x] = 'B';
  if (bomb->y < 16 && game->map[bomb->y + 1][bomb->x] == '0')
    game->map[bomb->y + 1][bomb->x] = 'B';
    if (bomb->x > 0 && game->map[bomb->y][bomb->x - 1] == '0')
    game->map[bomb->y][bomb->x - 1] = 'B';
  if (bomb->x < 16 && game->map[bomb->y][bomb->x + 1] == '0')
    game->map[bomb->y][bomb->x + 1] = 'B';
  
    if (bomb->y > 1 && game->map[bomb->y - 2][bomb->x] == '0')
    game->map[bomb->y - 2][bomb->x] = 'B';
  if (bomb->y < 15 && game->map[bomb->y + 2][bomb->x] == '0')
    game->map[bomb->y + 2][bomb->x] = 'B';
    if (bomb->x > 1 && game->map[bomb->y][bomb->x - 2] == '0')
    game->map[bomb->y][bomb->x - 2] = 'B';
  if (bomb->x < 15 && game->map[bomb->y][bomb->x + 2] == '0')
    game->map[bomb->y][bomb->x + 2] = 'B';
  
  if (bomb->y > 2 && game->map[bomb->y - 3][bomb->x] == '0')
    game->map[bomb->y - 3][bomb->x] = 'B';
  if (bomb->y < 13 && game->map[bomb->y + 3][bomb->x] == '0')
    game->map[bomb->y + 3][bomb->x] = 'B';
    if (bomb->x > 2 && game->map[bomb->y][bomb->x - 3] == '0')
    game->map[bomb->y][bomb->x - 3] = 'B';
  if (bomb->x < 13 && game->map[bomb->y][bomb->x + 3] == '0')
    game->map[bomb->y][bomb->x + 3] = 'B';
  displayMap(game);
}

void		add_bomb(t_game *game, t_player *player)
{
  t_object	*bomb = malloc(sizeof(t_object) + 1);

  Mix_PlayChannel(-1, game->audio->dropBomb, 0);
  player->lastBombHasExploded = 0;
  bomb->sprite_tex = game->flames_tex;
  bomb->tile_rect = malloc(sizeof(SDL_Rect) + 1);
  bomb->tile_rect->x = 0;
  bomb->tile_rect->y = 0;
  bomb->tile_rect->w = TILE_SIZE;
  bomb->tile_rect->h = TILE_SIZE;
  bomb->is_falling = 0;
  bomb->player_id = player->id;
  bomb->deadly = 0;
  bomb->range = player->flame_range;
  player->actual_bombs++;
  bomb->owned_by = player->id;
  bomb->hitbox = malloc(sizeof(SDL_Rect) + 1);
  bomb->x = my_ceil((player->position->hitbox->x  + (player->position->hitbox->w / 2)) / TILE_SIZE);
  bomb->y = my_ceil((player->position->hitbox->y  + (player->position->hitbox->h / 2)) / TILE_SIZE);
  add_bombs_on_map(game, bomb);
  bomb->hitbox->x = my_ceil((player->position->hitbox->x  + (player->position->hitbox->w / 2)) / TILE_SIZE) * TILE_SIZE;
  bomb->hitbox->y = my_ceil((player->position->hitbox->y  + (player->position->hitbox->h / 2)) / TILE_SIZE) * TILE_SIZE;
  bomb->hitbox->w = TILE_SIZE;
  bomb->hitbox->h = TILE_SIZE;

  for (t_object *obj = game->container->first; obj != NULL; obj = obj->next)
    {
      if (obj->is_bomb == 1 && Collision(bomb->hitbox, obj->hitbox) == 1)
	{
	  free(bomb->tile_rect);
	  free(bomb->hitbox);
	  player->actual_bombs--;
	  return;
	}
    }
  bomb->canSpawnBonus = 0;
  bomb->is_solid = 0;
  bomb->is_over_player = 0;
  bomb->is_bomb = 1;
  bomb->is_flame = 0;
  bomb->anim_index = 0;
  bomb->anim_tick = 0;
  bomb->anim_duration = ANIM_SPEED;
  bomb->timer = 120;
  bomb->next = NULL;
  bomb->prev = NULL;
  bomb->parent = NULL;
  add_object_to_container(game, bomb);
}

void	check_flames_position(t_game *game, t_object *bomb)
{
  Mix_PlayChannel(-1, game->audio->bombExplode, 0);
  add_flame(game, bomb, 5, 0);
  for (int i = -1; i >= -bomb->range; i--)
    {
      if (add_flame(game, bomb, 4, i) == 1)
	i = -bomb->range - 2;
    }
  for (int i = 1; i <= bomb->range; i++) 
    {
	if (add_flame(game, bomb, 6, i) == 1)
	  i = bomb->range + 2;
    }
  for (int i = -1; i >= -bomb->range; i--) 
    {
      if (add_flame(game, bomb, 8, i) == 1)
	i = -bomb->range - 2;
    }
  for (int i = 1; i <= bomb->range; i++) 
    {  
      if (add_flame(game, bomb, 2, i) == 1)
	i = bomb->range + 2;
    }
}

int	add_flame(t_game *game, t_object *bomb, int flame_dir, int distance_from_origin)
{
  t_object	*flame = malloc(sizeof(t_object) + 1);

  flame->canSpawnBonus = 0;
  flame->is_falling = 0;
  flame->flame_dir = flame_dir;
  flame->distance_from_origin = distance_from_origin;
  flame->bomb_range = bomb->range;
  flame->parent = bomb;
  flame->sprite_tex = game->flames_tex;
  flame->is_solid = 0;
  flame->is_over_player = 0;
  flame->is_bomb = 0;
  flame->is_flame = 1;
  flame->x = bomb->x;
  flame->y = bomb->y;
  flame->deadly = 0;
  flame->player_id = bomb->player_id;
  flame->is_falling = 0;
  flame->tile_rect = malloc(sizeof(SDL_Rect) + 1);
  flame->tile_rect->x = TILE_SIZE;
  flame->tile_rect->y = TILE_SIZE * 5;
  flame->tile_rect->w = TILE_SIZE;
  flame->tile_rect->h = TILE_SIZE;
  flame->hitbox = malloc(sizeof(SDL_Rect) + 1);
  if (flame_dir == 2 || flame_dir == 8)
    {
      flame->hitbox->x = bomb->hitbox->x;
      flame->hitbox->y = bomb->hitbox->y + (distance_from_origin * TILE_SIZE);
    }
  else if (flame_dir == 4 || flame_dir == 6)
    {
      flame->hitbox->x = bomb->hitbox->x + (distance_from_origin * TILE_SIZE);
      flame->hitbox->y = bomb->hitbox->y;
    }
  else
    {
      flame->hitbox->x = bomb->hitbox->x;
      flame->hitbox->y = bomb->hitbox->y;
    }
  flame->hitbox->w = TILE_SIZE;
  flame->hitbox->h = TILE_SIZE;
  for (t_object *object = game->container->first; object != NULL; object = object->next)
    {
      if (object->deadly == 0 && object->is_flame == 0 && object->is_bomb == 0 && object->is_solid == 1
	  && Collision(object->hitbox, flame->hitbox) == 1)
	{
	  if (object->is_destructible == 1)
	    {
	      if (object->is_bonus == 0)
		{
		  game->map[object->y][object->x] = '0';
		  displayMap(game);
		  add_destroy_effect(game, object, 1);
		}
	      else
		add_destroy_effect(game, object, 0);
	      del_object_from_container(game, object);
	      
	    }
	  return 1;
	}
      else if (object->is_bomb && Collision(object->hitbox, flame->hitbox) == 1)
	{
	  if (object->is_bomb == 1)
	    object->timer = 4;
	}
    }
  flame->anim_index = 0;
  flame->anim_tick = 0;
  flame->anim_duration = 2;
  flame->timer = 25;
  flame->prev = NULL;
  flame->next = NULL;
  add_object_to_container(game, flame);
  return 0;
}

void	add_destroy_effect(t_game *game, t_object *bomb, int _canSpawnBonus)
{
  t_object	*flame = malloc(sizeof(t_object) + 1);

  flame->canSpawnBonus = _canSpawnBonus;
  flame->sprite_tex = game->flames_tex;
  flame->is_solid = 0;
  flame->is_over_player = 1;
  flame->is_bomb = 0;
  flame->is_flame = 0;
  flame->is_destruction_effect = 1;
  flame->tile_rect = malloc(sizeof(SDL_Rect) + 1);
  flame->tile_rect->x = TILE_SIZE;
  flame->tile_rect->y = TILE_SIZE * 5;
  flame->tile_rect->w = TILE_SIZE;
  flame->tile_rect->h = TILE_SIZE;
  flame->hitbox = malloc(sizeof(SDL_Rect) + 1);
  flame->hitbox->x = bomb->hitbox->x;
  flame->hitbox->y = bomb->hitbox->y;
  flame->hitbox->w = TILE_SIZE;
  flame->hitbox->h = TILE_SIZE;
  flame->anim_index = 0;
  flame->anim_tick = 0;
  flame->anim_duration = 2;
  flame->timer = 25;
  flame->prev = NULL;
  flame->next = NULL;
  add_object_to_container(game, flame);
  return;
}

int Collision(SDL_Rect *box1, SDL_Rect *box2)
{
  if((box2->x >= box1->x + box1->w)      // trop à droite
     || (box2->x + box2->w <= box1->x) // trop à gauche
     || (box2->y >= box1->y + box1->h) // trop en bas
     || (box2->y + box2->h <= box1->y))  // trop en haut
    return 0; 
  else
    return 1; 
}

void	TimeOut(t_game *game)
{
  add_falling_object(game, 111, 1, 1, 1, 100);
  add_falling_object(game, 111, 2, 1, 1, 200);
  add_falling_object(game, 111, 3, 1, 1, 300);
  add_falling_object(game, 111, 4, 1, 1, 400);
  add_falling_object(game, 111, 5, 1, 1, 500);
  add_falling_object(game, 111, 6, 1, 1, 600);
  add_falling_object(game, 111, 7, 1, 1, 700);
  add_falling_object(game, 111, 8, 1, 1, 800);
  add_falling_object(game, 111, 9, 1, 1, 900);
  add_falling_object(game, 111, 10, 1, 1, 1000);
  add_falling_object(game, 111, 11, 1, 1, 1100);
  add_falling_object(game, 111, 12, 1, 1, 1200);
  add_falling_object(game, 111, 13, 1, 1, 1300);
  
  add_falling_object(game, 111, 13, 2, 1, 1400 + TILE_SIZE);
  add_falling_object(game, 111, 13, 3, 1, 1500 + TILE_SIZE * 2);
  add_falling_object(game, 111, 13, 4, 1, 1600 + TILE_SIZE * 3);
  add_falling_object(game, 111, 13, 5, 1, 1700 + TILE_SIZE * 4);
  add_falling_object(game, 111, 13, 6, 1, 1800 + TILE_SIZE * 5);
  add_falling_object(game, 111, 13, 7, 1, 1900 + TILE_SIZE * 6);
  add_falling_object(game, 111, 13, 8, 1, 2000 + TILE_SIZE * 7);
  add_falling_object(game, 111, 13, 9, 1, 2100 + TILE_SIZE * 8);
  add_falling_object(game, 111, 13, 10, 1, 2200 + TILE_SIZE * 9);
  add_falling_object(game, 111, 13, 11, 1, 2300 + TILE_SIZE * 10);
  add_falling_object(game, 111, 13, 12, 1, 2400 + TILE_SIZE * 11);
  add_falling_object(game, 111, 13, 13, 1, 2500 + TILE_SIZE * 12);

  add_falling_object(game, 111, 12, 13, 1, 2600 + TILE_SIZE * 12);
  add_falling_object(game, 111, 11, 13, 1, 2700 + TILE_SIZE * 12);
  add_falling_object(game, 111, 10, 13, 1, 2800 + TILE_SIZE * 12);
  add_falling_object(game, 111, 9, 13, 1, 2900 + TILE_SIZE * 12);
  add_falling_object(game, 111, 8, 13, 1, 3000 + TILE_SIZE * 12);
  add_falling_object(game, 111, 7, 13, 1, 3100 + TILE_SIZE * 12);
  add_falling_object(game, 111, 6, 13, 1, 3200 + TILE_SIZE * 12);
  add_falling_object(game, 111, 5, 13, 1, 3300 + TILE_SIZE * 12);
  add_falling_object(game, 111, 4, 13, 1, 3400 + TILE_SIZE * 12);
  add_falling_object(game, 111, 3, 13, 1, 3500 + TILE_SIZE * 12);
  add_falling_object(game, 111, 2, 13, 1, 3600 + TILE_SIZE * 12);
  add_falling_object(game, 111, 1, 13, 1, 3700 + TILE_SIZE * 12);

  add_falling_object(game, 111, 1, 12, 1, 3800 + TILE_SIZE * 11);
  add_falling_object(game, 111, 1, 11, 1, 3900 + TILE_SIZE * 10);
  add_falling_object(game, 111, 1, 10, 1, 4000 + TILE_SIZE * 9);
  add_falling_object(game, 111, 1, 9, 1, 4100 + TILE_SIZE * 8);
  add_falling_object(game, 111, 1, 8, 1, 4200 + TILE_SIZE * 7);
  add_falling_object(game, 111, 1, 7, 1, 4300 + TILE_SIZE * 6);
  add_falling_object(game, 111, 1, 6, 1, 4400 + TILE_SIZE * 5);
  add_falling_object(game, 111, 1, 5, 1, 4500 + TILE_SIZE * 4);
  add_falling_object(game, 111, 1, 4, 1, 4600 + TILE_SIZE * 3);
  add_falling_object(game, 111, 1, 3, 1, 4700 + TILE_SIZE * 2);
  add_falling_object(game, 111, 1, 2, 1, 4800 + TILE_SIZE);

  add_falling_object(game, 111, 2, 2, 1, 4900 + TILE_SIZE);
  add_falling_object(game, 111, 3, 2, 1, 5000 + TILE_SIZE);
  add_falling_object(game, 111, 4, 2, 1, 5100 + TILE_SIZE);
  add_falling_object(game, 111, 5, 2, 1, 5200 + TILE_SIZE);
  add_falling_object(game, 111, 6, 2, 1, 5300 + TILE_SIZE);
  add_falling_object(game, 111, 7, 2, 1, 5400 + TILE_SIZE);
  add_falling_object(game, 111, 8, 2, 1, 5500 + TILE_SIZE);
  add_falling_object(game, 111, 9, 2, 1, 5600 + TILE_SIZE);
  add_falling_object(game, 111, 10, 2, 1, 5700 + TILE_SIZE);
  add_falling_object(game, 111, 11, 2, 1, 5800 + TILE_SIZE);
  add_falling_object(game, 111, 12, 2, 1, 5900 + TILE_SIZE);

  add_falling_object(game, 111, 12, 3, 1, 6000 + TILE_SIZE * 2);
  add_falling_object(game, 111, 12, 4, 1, 6100 + TILE_SIZE * 3);
  add_falling_object(game, 111, 12, 5, 1, 6200 + TILE_SIZE * 4);
  add_falling_object(game, 111, 12, 6, 1, 6300 + TILE_SIZE * 5);
  add_falling_object(game, 111, 12, 7, 1, 6400 + TILE_SIZE * 6);
  add_falling_object(game, 111, 12, 8, 1, 6500 + TILE_SIZE * 7);
  add_falling_object(game, 111, 12, 9, 1, 6600 + TILE_SIZE * 8);
  add_falling_object(game, 111, 12, 10, 1, 6700 + TILE_SIZE * 9);
  add_falling_object(game, 111, 12, 11, 1, 6800 + TILE_SIZE * 10);
  add_falling_object(game, 111, 12, 12, 1, 6900 + TILE_SIZE * 11);

  add_falling_object(game, 111, 11, 12, 1, 6900 + TILE_SIZE * 11);
  add_falling_object(game, 111, 10, 12, 1, 7000 + TILE_SIZE * 11);
  add_falling_object(game, 111, 9, 12, 1, 7100 + TILE_SIZE * 11);
  add_falling_object(game, 111, 8, 12, 1, 7200 + TILE_SIZE * 11);
  add_falling_object(game, 111, 7, 12, 1, 7300 + TILE_SIZE * 11);
  add_falling_object(game, 111, 6, 12, 1, 7400 + TILE_SIZE * 11);
  add_falling_object(game, 111, 5, 12, 1, 7500 + TILE_SIZE * 11);
  add_falling_object(game, 111, 4, 12, 1, 7600 + TILE_SIZE * 11);
  add_falling_object(game, 111, 3, 12, 1, 7700 + TILE_SIZE * 11);
  add_falling_object(game, 111, 2, 12, 1, 7800 + TILE_SIZE * 11);

  add_falling_object(game, 111, 2, 11, 1, 7900 + TILE_SIZE * 10);
  add_falling_object(game, 111, 2, 10, 1, 8000 + TILE_SIZE * 9);
  add_falling_object(game, 111, 2, 9, 1, 8100 + TILE_SIZE * 8);
  add_falling_object(game, 111, 2, 8, 1, 8200 + TILE_SIZE * 7);
  add_falling_object(game, 111, 2, 7, 1, 8300 + TILE_SIZE * 6);
  add_falling_object(game, 111, 2, 6, 1, 8400 + TILE_SIZE * 5);
  add_falling_object(game, 111, 2, 5, 1, 8500 + TILE_SIZE * 4);
  add_falling_object(game, 111, 2, 4, 1, 8600 + TILE_SIZE * 3);
  add_falling_object(game, 111, 2, 3, 1, 8700 + TILE_SIZE * 2);

  add_falling_object(game, 111, 3, 3, 1, 8700 + TILE_SIZE * 2);
  add_falling_object(game, 111, 4, 3, 1, 8800 + TILE_SIZE * 2);
  add_falling_object(game, 111, 5, 3, 1, 8900 + TILE_SIZE * 2);
  add_falling_object(game, 111, 6, 3, 1, 9000 + TILE_SIZE * 2);
  add_falling_object(game, 111, 7, 3, 1, 9100 + TILE_SIZE * 2);
  add_falling_object(game, 111, 8, 3, 1, 9200 + TILE_SIZE * 2);
  add_falling_object(game, 111, 9, 3, 1, 9300 + TILE_SIZE * 2);
  add_falling_object(game, 111, 10, 3, 1, 9400 + TILE_SIZE * 2);
  add_falling_object(game, 111, 11, 3, 1, 9500 + TILE_SIZE * 2);

  add_falling_object(game, 111, 11, 4, 1, 9600 + TILE_SIZE * 3);
  add_falling_object(game, 111, 11, 5, 1, 9700 + TILE_SIZE * 4);
  add_falling_object(game, 111, 11, 6, 1, 9800 + TILE_SIZE * 5);
  add_falling_object(game, 111, 11, 7, 1, 9900 + TILE_SIZE * 6);
  add_falling_object(game, 111, 11, 8, 1, 10100 + TILE_SIZE * 7);
  add_falling_object(game, 111, 11, 9, 1, 10200 + TILE_SIZE * 8);
  add_falling_object(game, 111, 11, 10, 1, 10300 + TILE_SIZE * 9);
  add_falling_object(game, 111, 11, 11, 1, 10400 + TILE_SIZE * 10);

  add_falling_object(game, 111, 10, 11, 1, 10500 + TILE_SIZE * 10);
  add_falling_object(game, 111, 9, 11, 1, 10600 + TILE_SIZE * 10);
  add_falling_object(game, 111, 8, 11, 1, 10700 + TILE_SIZE * 10);
  add_falling_object(game, 111, 7, 11, 1, 10800 + TILE_SIZE * 10);
  add_falling_object(game, 111, 6, 11, 1, 10900 + TILE_SIZE * 10);
  add_falling_object(game, 111, 5, 11, 1, 11000 + TILE_SIZE * 10);
  add_falling_object(game, 111, 4, 11, 1, 11200 + TILE_SIZE * 10);
  add_falling_object(game, 111, 3, 11, 1, 11300 + TILE_SIZE * 10);

  add_falling_object(game, 111, 3, 10, 1, 11400 + TILE_SIZE * 9);
  add_falling_object(game, 111, 3, 9, 1, 11500 + TILE_SIZE * 8);
  add_falling_object(game, 111, 3, 8, 1, 11600 + TILE_SIZE * 7);
  add_falling_object(game, 111, 3, 7, 1, 11700 + TILE_SIZE * 6);
  add_falling_object(game, 111, 3, 6, 1, 11800 + TILE_SIZE * 5);
  add_falling_object(game, 111, 3, 5, 1, 11900 + TILE_SIZE * 4);
  add_falling_object(game, 111, 3, 4, 1, 12000 + TILE_SIZE * 3);

  add_falling_object(game, 111, 4, 4, 1, 12100 + TILE_SIZE * 3);
  add_falling_object(game, 111, 5, 4, 1, 12200 + TILE_SIZE * 3);
  add_falling_object(game, 111, 6, 4, 1, 12300 + TILE_SIZE * 3);
  add_falling_object(game, 111, 7, 4, 1, 12400 + TILE_SIZE * 3);
  add_falling_object(game, 111, 8, 4, 1, 12500 + TILE_SIZE * 3);
  add_falling_object(game, 111, 9, 4, 1, 12600 + TILE_SIZE * 3);
  add_falling_object(game, 111, 10, 4, 1, 12700 + TILE_SIZE * 3);

  add_falling_object(game, 111, 10, 5, 1, 12800 + TILE_SIZE * 4);
  add_falling_object(game, 111, 10, 6, 1, 12900 + TILE_SIZE * 5);
  add_falling_object(game, 111, 10, 7, 1, 13000 + TILE_SIZE * 6);
  add_falling_object(game, 111, 10, 8, 1, 13100 + TILE_SIZE * 7);
  add_falling_object(game, 111, 10, 9, 1, 13200 + TILE_SIZE * 8);
  add_falling_object(game, 111, 10, 10, 1, 13300 + TILE_SIZE * 9);

  add_falling_object(game, 111, 9, 10, 1, 13400 + TILE_SIZE * 9);
  add_falling_object(game, 111, 8, 10, 1, 13500 + TILE_SIZE * 9);
  add_falling_object(game, 111, 7, 10, 1, 13600 + TILE_SIZE * 9);
  add_falling_object(game, 111, 6, 10, 1, 13700 + TILE_SIZE * 9);
  add_falling_object(game, 111, 5, 10, 1, 13800 + TILE_SIZE * 9);
  add_falling_object(game, 111, 4, 10, 1, 13900 + TILE_SIZE * 9);

  add_falling_object(game, 111, 4, 9, 1, 14000 + TILE_SIZE * 8);
  add_falling_object(game, 111, 4, 8, 1, 14100 + TILE_SIZE * 7);
  add_falling_object(game, 111, 4, 7, 1, 14200 + TILE_SIZE * 6);
  add_falling_object(game, 111, 4, 6, 1, 14300 + TILE_SIZE * 5);
  add_falling_object(game, 111, 4, 5, 1, 14400 + TILE_SIZE * 4);

  add_falling_object(game, 111, 5, 5, 1, 14500 + TILE_SIZE * 4);
  add_falling_object(game, 111, 6, 5, 1, 14600 + TILE_SIZE * 4);
  add_falling_object(game, 111, 7, 5, 1, 14700 + TILE_SIZE * 4);
  add_falling_object(game, 111, 8, 5, 1, 14800 + TILE_SIZE * 4);
  add_falling_object(game, 111, 9, 5, 1, 14900 + TILE_SIZE * 4);

  add_falling_object(game, 111, 9, 6, 1, 15000 + TILE_SIZE * 5);
  add_falling_object(game, 111, 9, 7, 1, 15100 + TILE_SIZE * 6);
  add_falling_object(game, 111, 9, 8, 1, 15200 + TILE_SIZE * 7);
  add_falling_object(game, 111, 9, 9, 1, 15300 + TILE_SIZE * 8);

  add_falling_object(game, 111, 8, 9, 1, 15400 + TILE_SIZE * 8);
  add_falling_object(game, 111, 7, 9, 1, 15500 + TILE_SIZE * 8);
  add_falling_object(game, 111, 6, 9, 1, 15600 + TILE_SIZE * 8);
  add_falling_object(game, 111, 5, 9, 1, 15700 + TILE_SIZE * 8);

  add_falling_object(game, 111, 5, 8, 1, 15800 + TILE_SIZE * 7);
  add_falling_object(game, 111, 5, 7, 1, 15900 + TILE_SIZE * 6);
  add_falling_object(game, 111, 5, 6, 1, 16000 + TILE_SIZE * 5);

  add_falling_object(game, 111, 6, 6, 1, 16200 + TILE_SIZE * 5);
  add_falling_object(game, 111, 7, 6, 1, 16300 + TILE_SIZE * 5);
  add_falling_object(game, 111, 8, 6, 1, 16400 + TILE_SIZE * 5);
  
  add_falling_object(game, 111, 8, 7, 1, 16500 + TILE_SIZE * 6);
  add_falling_object(game, 111, 8, 8, 1, 16600 + TILE_SIZE * 7);

  add_falling_object(game, 111, 7, 8, 1, 16700 + TILE_SIZE * 7);
  add_falling_object(game, 111, 6, 8, 1, 16800 + TILE_SIZE * 7);

  add_falling_object(game, 111, 6, 7, 1, 16900 + TILE_SIZE * 6);

  add_falling_object(game, 111, 14, 7, 1, 20500 + TILE_SIZE * 6);
  add_falling_object(game, 111, 0, 7, 1, 20500 + TILE_SIZE * 6);
  add_falling_object(game, 111, 7, 0, 1, 20500);
  add_falling_object(game, 111, 7, 14, 1, 20500 + TILE_SIZE * 6);
  
  add_falling_object(game, 111, 7, 7, 1, 21000 + TILE_SIZE * 6);
}
