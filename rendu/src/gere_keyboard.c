/*
** gere_keyboard.c for bomberman in /home/infitek/Documents/Etna/Bomberman/rendu
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Fri Jun 29 10:19:44 2018 REBOUL Simon
** Last update Thu Feb 14 13:18:21 2019 REBOUL Simon
*/

#include "../headers/functions.h"

void	gere_keyboard(t_game *game, t_player *player, SDL_Event event)
{
   if (player->is_dead == 0)
    {
      switch( event.type ){
      case SDL_MOUSEMOTION:
	break;
      case SDL_KEYDOWN:
	switch( event.key.keysym.sym ){
	case SDLK_x:
	  if (event.key.repeat == 0 && player->actual_bombs < player->nb_bombs)
	    add_bomb(game, player);
	  break;
	case SDLK_LEFT:
	  if (player->is_sliding == 0 && player->position->pos->y + player->position->hitbox->h >=  0 && player->position->pos->y <= 13 * TILE_SIZE)
	    {
	      if (player->position->yvel == 0)
		player->position->dir = 1;
	      player->position->second_dir = 1;
	      player->position->xvel = -(WALK_SPEED + player->speed);
	    }
	  break;
	case SDLK_RIGHT:
	  if (player->is_sliding == 0 && player->position->pos->y + player->position->hitbox->h  >=  0 && player->position->pos->y  <= 13 * TILE_SIZE)
	    {
	      if (player->position->yvel == 0)
		player->position->dir = 3;
	      player->position->second_dir = 3;
	      player->position->xvel = (WALK_SPEED + player->speed);
	    }
	  break;
	case SDLK_UP:
	  if (player->is_sliding == 0 && player->position->pos->x >=  0 && player->position->pos->x <= 13 * TILE_SIZE)
	    {
	      if (player->position->xvel == 0)
		player->position->dir = 5;
	      player->position->second_dir = 5;
	      player->position->yvel = -(WALK_SPEED + player->speed);
	    }
	  break;
	case SDLK_DOWN:
	  if (player->is_sliding == 0 && player->position->pos->x >=  0 && player->position->pos->x <= 13 * TILE_SIZE)
	    {
	      if (player->position->xvel == 0)
		player->position->dir = 2;
	      player->position->second_dir = 2;
	      player->position->yvel = (WALK_SPEED + player->speed);
	    }
	  break;
	default:
	  break;
	}
	break;
      case SDL_KEYUP:
	switch( event.key.keysym.sym ){
	case SDLK_LEFT:
	  if( player->position->xvel < 0)
	    player->position->xvel = 0;
	  break;
	case SDLK_RIGHT:
	  if( player->position->xvel > 0)
	    player->position->xvel = 0;
	  break;
	case SDLK_UP:
	  if( player->position->yvel < 0)
	    player->position->yvel = 0;
	  break;
	case SDLK_DOWN:
	  if( player->position->yvel > 0)
	    player->position->yvel = 0;
	  break;
	default:
	  break;
	}
	break;
	
      default:
	break;
      }
    }
   return;
}


void	gere_keyboard_network(t_game *game, t_player *player, int dir)
{
  printf("receveid move signal from player %d in direction %d\n", player->id, dir);
  if (player->is_dead == 0)
    {
      switch(dir)
	{
	case 1:
	  if (player->is_sliding == 0 && player->position->pos->y + player->position->hitbox->h >=  0 && player->position->pos->y <= 13 * TILE_SIZE)
	    {
	      if (player->position->yvel == 0)
		player->position->dir = 1;
	      player->position->second_dir = 1;
	      player->position->xvel = -(WALK_SPEED + player->speed);
	    }
	  break;
	case 3:
	  if (player->is_sliding == 0 && player->position->pos->y + player->position->hitbox->h  >=  0 && player->position->pos->y  <= 13 * TILE_SIZE)
	    {
	      if (player->position->yvel == 0)
		player->position->dir = 3;
	      player->position->second_dir = 3;
	      player->position->xvel = (WALK_SPEED + player->speed);
	    }
	  break;
	case 5:
	  if (player->is_sliding == 0 && player->position->pos->x >=  0 && player->position->pos->x <= 13 * TILE_SIZE)
	    {
	      if (player->position->xvel == 0)
		player->position->dir = 5;
	      player->position->second_dir = 5;
	      player->position->yvel = -(WALK_SPEED + player->speed);
	    }
	  break;
	case 2:
	  if (player->is_sliding == 0 && player->position->pos->x >=  0 && player->position->pos->x <= 13 * TILE_SIZE)
	    {
	      if (player->position->xvel == 0)
		player->position->dir = 2;
	      player->position->second_dir = 2;
	      player->position->yvel = (WALK_SPEED + player->speed);
	    }
	  break;
	default:
	  break;
	}
    }
  return;
}
