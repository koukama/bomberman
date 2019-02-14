/*
** gere_display.c for bomberman in /home/infitek/Documents/Etna/Bomberman
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Sat May  5 14:36:09 2018 REBOUL Simon
** Last update Thu Feb 14 11:21:35 2019 REBOUL Simon
*/

#include "../headers/bomberman.h"
#include "../headers/functions.h"

t_game	*recheck_z_index(t_game *game)
{
  return game;
  int	recheck = 1;
  //players_sorted = game->players;
  while (recheck != 0)
    {
      recheck = 0;
      for (int i = 0; game->players[i] != NULL; i++)
	{
	  if (game->players[i + 1] != NULL &&
	      game->players[i]->position->pos->y > game->players[i + 1]->position->pos->y)
	    {
	      t_player *tmp = game->players[i];
	      game->players[i] = game->players[i + 1];
	      game->players[i + 1] = tmp;
	      recheck = 1;
	    } 
	}
    }
  return game;
}

void	gere_display(t_game *game)
{
  SDL_Rect screen_offset;
  SDL_Rect adjustedPosition;
  
  screen_offset.x = TILE_SIZE * 3;
  screen_offset.y = 0;
  screen_offset.w = 480;
  screen_offset.h = 480;
  
  //SDL_RenderCopy(game->screen, game->background_tex, NULL, &screen_offset);
  if (game->container== NULL)
    return;
  for (t_object *tmp = game->container->first; tmp != NULL; tmp = tmp->next)
    {
      //if (tmp->is_flame == 1 && tmp->timer > 0)
      //tmp->tile_rect = gere_anim_flames(tmp);+
      if (tmp->is_bonus== 1)
	gere_anim_bonus(tmp);
      if (tmp->is_bomb == 1 && tmp->timer > 0)
	gere_anim_bomb(tmp);
      if (tmp->is_destruction_effect == 1 && tmp->timer > 0)
	gere_anim_destruction_effect(tmp);
      if (tmp->is_over_player == 0)
	{
	  adjustedPosition = *tmp->hitbox;
	  adjustedPosition.x += screen_offset.x;
	  if (SDL_RenderCopy(game->screen, tmp->sprite_tex, tmp->tile_rect, &adjustedPosition) != 0)
	    printf("Error : %s\n", SDL_GetError());
	}
    }
    for (t_object *tmp = game->container->first; tmp != NULL; tmp = tmp->next)
    {
      if (tmp->is_flame == 1 && tmp->timer > 0)
	{
	  adjustedPosition = *tmp->hitbox;
	  adjustedPosition.x += screen_offset.x;
	  gere_anim_flames(tmp);
	  SDL_RenderCopy(game->screen, tmp->sprite_tex, tmp->tile_rect, &adjustedPosition);
	}
    }
    for (int i = 0; game->players[i] != NULL; i++)
      {
	adjustedPosition = *game->players[i]->position->pos;
	adjustedPosition.x += screen_offset.x;
	SDL_RenderCopy(game->screen, game->players[i]->sprite_tex, gere_anim(game->players[i]), &adjustedPosition);
      }
    for (t_object *tmp = game->container->first; tmp != NULL; tmp = tmp->next)
      {
	if (tmp->deadly == 1)
	  {
	    if (tmp->is_falling == 1 && tmp->fall_offset > 0)
	      tmp->fall_offset -= 10;
	    else if (tmp->is_falling == 1)
	      {
		Mix_PlayChannel(-1, game->audio->dropBomb, 0);
		tmp->is_falling = 0;
		tmp->is_over_player = 0;
		for (t_object *tmp2 = game->container->first; tmp2 != NULL; tmp2 = tmp2->next)
		  {
		    if (Collision(tmp->hitbox, tmp2->hitbox) == 1 && tmp2->is_flame == 0 && tmp2->is_destruction_effect == 0 && tmp2->deadly == 0 && ((tmp2->is_bonus == 1)  || (tmp2->is_solid == 1) || (tmp2->is_over_player == 1)))
		      {
			add_destroy_effect(game, tmp2, 0);
			del_object_from_container(game, tmp2);
		      }
		  }
	      }
	  }
	else
	  tmp->fall_offset = 0;
	if (tmp->is_over_player == 1)
	  {
	    adjustedPosition = *tmp->hitbox;
	    adjustedPosition.x += screen_offset.x;
	    adjustedPosition.y = tmp->hitbox->y - tmp->fall_offset;
	    SDL_RenderCopy(game->screen, tmp->sprite_tex, tmp->tile_rect, &adjustedPosition);
	  }
      }
    gere_display_ui(game);
}

void	gere_display_ui(t_game *game)
{
  game->timeCounter = TTF_RenderText_Blended(game->start_screen->police, get_time_left(game), game->start_screen->fontColor);
  SDL_RenderCopy(game->screen, game->panel, NULL, &game->panelLeft);
  SDL_RenderCopy(game->screen, game->panel, NULL, &game->panelRight);
  SDL_RenderCopy(game->screen, SDL_CreateTextureFromSurface(game->screen, game->timeCounter), NULL, &game->ui->text_pos);
  for (int i = 0; game->players[i] != NULL; i++)
    {
      SDL_RenderCopy(game->screen, game->ui->playerPortraitTexture, game->ui->playerPortraitTilePos[i], game->ui->playerPortraitPos[i]);
      if (!game->players[i]->is_dead)
	{
	  if (game->players[i]->nb_bombs <= 2)
	    game->ui->stars_pos.x = 0;
	  else if (game->players[i]->nb_bombs > 2 && game->players[i]->nb_bombs <= 4)
	    game->ui->stars_pos.x = 32;
	  else if (game->players[i]->nb_bombs > 4 && game->players[i]->nb_bombs <= 6)
	    game->ui->stars_pos.x = 64;
	  else if (game->players[i]->nb_bombs > 6 && game->players[i]->nb_bombs <= 8)
	    game->ui->stars_pos.x = 96;
	  else
	    game->ui->stars_pos.x = 128;
	  SDL_RenderCopy(game->screen, game->ui->starsTexture, &game->ui->stars_pos, &game->players[i]->statNbBombsPos);
	   if (game->players[i]->speed < 0)
	    game->ui->stars_pos.x = 0;
	  else if (game->players[i]->speed == 0)
	    game->ui->stars_pos.x = 32;
	  else if (game->players[i]->speed == 1)
	    game->ui->stars_pos.x = 64;
	  else if (game->players[i]->speed == 2)
	    game->ui->stars_pos.x = 96;
	  else
	    game->ui->stars_pos.x = 128;
	  SDL_RenderCopy(game->screen, game->ui->starsTexture, &game->ui->stars_pos, &game->players[i]->statSpeedPos);
	  if (game->players[i]->flame_range < 2)
	    game->ui->stars_pos.x = 0;
	  else if (game->players[i]->flame_range == 2)
	    game->ui->stars_pos.x = 32;
	  else if (game->players[i]->flame_range >= 3 && game->players[i]->flame_range < 5)
	    game->ui->stars_pos.x = 64;
	  else if (game->players[i]->flame_range >= 5 && game->players[i]->flame_range < 6)
	    game->ui->stars_pos.x = 96;
	  else
	    game->ui->stars_pos.x = 128;
	  SDL_RenderCopy(game->screen, game->ui->starsTexture, &game->ui->stars_pos, &game->players[i]->statRangePos);
	}
    }
}

char	*get_time_left(t_game *game)
{
  char	*str = malloc(sizeof(50));
  sprintf(str, "%d", game->time_left / 20);
  if (my_strlen(str) == 1 && str[0] == '0' && game->time_out == 0)
    {
      TimeOut(game);
      game->time_out = 1;
    }
  if (game->time_left < 0)
    str = "0";
  return (str);
}

void	drawBoundingBoxes(t_game *game, SDL_Rect *box)
{
  SDL_Surface	*hitbox;// = malloc(sizeof(SDL_Surface));
  SDL_Texture	*hitbox_tex;
  hitbox  = IMG_Load("./images/Red.png");
  box->w = 30;
  box->h = 24;
  
  SDL_FillRect(hitbox, box, 0x00FF00FF); 
  hitbox_tex = SDL_CreateTextureFromSurface(game->screen, hitbox);
  SDL_RenderCopy(game->screen, hitbox_tex, NULL, box);
  free(hitbox);
  }

void	gere_anim_bonus(t_object *bonus)
{
  if (bonus->tile_rect == NULL)
    bonus->tile_rect = malloc(sizeof(SDL_Rect*) + 1);
  bonus->tile_rect->w = TILE_SIZE;
  bonus->tile_rect->h = TILE_SIZE;

  if (bonus->anim_tick >= bonus->anim_duration)
    {
      if (bonus->anim_index == 1)
	bonus->anim_index = 0;
      else
	bonus->anim_index++;
      bonus->anim_tick = 0;
    }
  else
    bonus->anim_tick++;
  bonus->tile_rect->y = bonus->anim_index * TILE_SIZE;
  
  return;
}

void	gere_anim_bomb(t_object *bomb)
{
  if (bomb->tile_rect == NULL)
    bomb->tile_rect = malloc(sizeof(SDL_Rect*) + 1);
  
  bomb->tile_rect->y = 0;
  bomb->tile_rect->w = TILE_SIZE;
  bomb->tile_rect->h = TILE_SIZE;

  if (bomb->anim_tick >= bomb->anim_duration)
    {
      if (bomb->anim_index == 3)
	bomb->anim_index = 0;
      else
	bomb->anim_index++;
      bomb->anim_tick = 0;
    }
  else
    bomb->anim_tick++;
  bomb->tile_rect->x = bomb->anim_index * TILE_SIZE;
  
  return;
}

// Flame_dir : 5 = neutral, 4 = left, 6 = right, 8 = up, 2 = down
void	gere_anim_flames(t_object *flame)
{
  if (flame->tile_rect == NULL)
    flame->tile_rect = malloc(sizeof(SDL_Rect*) + 1);
  
  switch(flame->flame_dir)
    {
    case 5 :
      flame->tile_rect->x = TILE_SIZE;
      break;
    case 4 :
      if (-flame->distance_from_origin == flame->bomb_range)
	flame->tile_rect->x = TILE_SIZE * 7;
      else
        flame->tile_rect->x = TILE_SIZE * 4;
      break;
    case 6 :
       if (flame->distance_from_origin == flame->bomb_range)
	 flame->tile_rect->x = TILE_SIZE * 2;
       else
	 flame->tile_rect->x = TILE_SIZE * 4;
      break;
    case 2:
      if (flame->distance_from_origin == flame->bomb_range)
	flame->tile_rect->x = TILE_SIZE * 5;
      else
	flame->tile_rect->x = TILE_SIZE * 3;
      break;
    case 8:
      if (-flame->distance_from_origin == flame->bomb_range)
	flame->tile_rect->x = 0;
      else
	flame->tile_rect->x = TILE_SIZE * 3;
      break;
    }
  flame->tile_rect->w = TILE_SIZE;
  flame->tile_rect->h = TILE_SIZE;

  if (flame->anim_tick >= flame->anim_duration)
    {
      if (flame->anim_index == 4)
	{
	  flame->anim_forward = 0;
	}
      else if (flame->anim_index == 0)
	flame->anim_forward = 1;
      if (flame->anim_forward == 1)
        flame->anim_index++;
      else
	flame->anim_index--;
      flame->anim_tick = 0;
    }
  else
    flame->anim_tick++;
  flame->tile_rect->y = flame->anim_index * TILE_SIZE + TILE_SIZE;
  
  return;
}

void	gere_anim_destruction_effect(t_object *flame)
{
  if (flame->tile_rect == NULL)
    flame->tile_rect = malloc(sizeof(SDL_Rect*) + 1);

  flame->tile_rect->x = TILE_SIZE * 6;
  flame->tile_rect->w = TILE_SIZE;
  flame->tile_rect->h = TILE_SIZE;
  
  if (flame->anim_tick >= flame->anim_duration)
    {
        flame->anim_index++;
	flame->anim_tick = 0;
    }
  else
    flame->anim_tick++;
  flame->tile_rect->y = flame->anim_index * TILE_SIZE + TILE_SIZE;
  
  return;
}

int abs(int nb)
{
  if (nb < 0)
    return (nb * -1);
  else
    return nb;
}

SDL_Rect	*gere_anim(t_player *player)
{
  SDL_Rect	*rect;
  
  rect = malloc(sizeof(SDL_Rect) + 1);
  rect->w = 36 * SCALE;
  rect->h = 54 * SCALE;
  rect->y = (player->id - 1) * (54 * SCALE);
  if (player->walking == 1)
    {
      if (player->anim_tick >= player->anim_duration)
	{
	  if (player->anim_index == 2)
	    {
	      player->anim_forward = 0;
	    }
	  else if (player->anim_index == 0)
	    {
	      player->anim_forward = 1;
	    }
	  if (player->anim_forward == 1)
	    player->anim_index++;
	  else
	    player->anim_index--;
	  player->anim_tick = 0;
	}
      else
	player->anim_tick++;
      if (player->position->yvel > 0)
	{
	  player->last_anim_dir = 2;
	  rect->x = 0 + (rect->w * player->anim_index);
	}
      else if (player->position->yvel < 0)
	{
	  player->last_anim_dir = 5; 
	  rect->x = (324 * SCALE) + (rect->w * player->anim_index);
	}
      if (player->position->xvel < 0)
	{
	  player->last_anim_dir = 1;
	  rect->x =(108 * SCALE) + (rect->w * player->anim_index);
	}
      else if (player->position->xvel > 0)
	{
	  player->last_anim_dir = 3;
	  rect->x = (216 * SCALE) + (rect->w * player->anim_index);
	}
    }
  else if (player->is_dead == 0)
    {
      player->anim_index = 0; 
      switch (player->last_anim_dir)
      {
      case 1 :
	rect->x = 144 * SCALE;
	break;
      case 2 :
	rect->x = 36 * SCALE;
	break;
      case 3 :
	rect->x = 252 * SCALE;
	break;
      case 5 :
	rect->x = 360 * SCALE;
	break;
      default:
	rect->x = 36 * SCALE;
	break;
      }
    }
  else
    {
      if (player->anim_index >= 3)
	{
	  player->is_dead = 2;
	}
       if (player->anim_tick >= player->anim_duration && player->is_dead == 1)
	{
	  player->anim_index++;
	  player->anim_tick = -15;
	}
      else
	player->anim_tick++;
       rect->x = (575 * SCALE) + ((rect->w - 2) * player->anim_index);
    }
  return rect;
}
