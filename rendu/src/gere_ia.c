/*
** gere_ia.c for bomberman in /home/infitek/Documents/Etna/Bomberman/rendu
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Tue Jul  3 12:18:03 2018 REBOUL Simon
** Last update Thu Jul  5 18:34:30 2018 REBOUL Simon
*/

#include "../headers/functions.h"

int	check_if_add_bomb_is_safe(t_game *game, t_player *player)
{
  if (player->position->y > 0 && player->position->x > 0 &&
      player->position->x < 16 && player->position->y < 16 &&
      game->map[player->position->y - 1][player->position->x - 1] != 'B' &&
      game->map[player->position->y - 1][player->position->x] != 'B' &&
      game->map[player->position->y - 1][player->position->x + 1] != 'B' &&
      game->map[player->position->y][player->position->x - 1] != 'B' &&
      game->map[player->position->y][player->position->x + 1] != 'B' &&
      game->map[player->position->y + 1][player->position->x - 1] != 'B' &&
      game->map[player->position->y + 1][player->position->x] != 'B' &&
      game->map[player->position->y + 1][player->position->x + 1] != 'B')
	{
	  return 1;
	}
  return 0;
}


void	gere_ia(t_game *game, t_player *player)
{
  if (player->waitTimer > 0)
    {
      player->waitTimer--;
      return;
    }
  //printf("player position x = %d, y = %d\n", player->position->hitbox->x, player->position->hitbox->y);
  player->position->x = my_ceil((player->position->hitbox->x) / TILE_SIZE);
  player->position->y = my_ceil((player->position->hitbox->y  + (player->position->hitbox->h)) / TILE_SIZE);
  if ((player->position->y - 1 > 0 && game->map[player->position->y - 1][player->position->x] == 'B') ||
      (player->position->y < 16 && game->map[player->position->y + 1][player->position->x] == 'B') ||
      (player->position->x < 16 && game->map[player->position->y][player->position->x + 1] == 'B') ||
      (player->position->x > 0 && game->map[player->position->y][player->position->x - 1] == 'B') ||
      (player->position->y - 1 > 0 && game->map[player->position->y - 1][player->position->x - 1] == 'B') ||
      (player->position->y < 16 && game->map[player->position->y - 1][player->position->x + 1] == 'B') ||
      (player->position->x < 16 && game->map[player->position->y + 1][player->position->x + 1] == 'B') ||
      (player->position->x > 0 && game->map[player->position->y + 1][player->position->x - 1] == 'B') ||

      /*(player->position->y > 1 && game->map[player->position->y - 2][player->position->x] == 'B') ||
      (player->position->y < 14 && game->map[player->position->y + 2][player->position->x] == 'B') ||
      (player->position->x < 14 && game->map[player->position->y][player->position->x + 2] == 'B') ||
      (player->position->x > 1 && game->map[player->position->y][player->position->x - 2] == 'B') ||
      (player->position->y > 1 && game->map[player->position->y - 2][player->position->x - 2] == 'B') ||
      (player->position->y < 14 && game->map[player->position->y - 2][player->position->x + 2] == 'B') ||
      (player->position->x < 14 && game->map[player->position->y + 2][player->position->x + 2] == 'B') ||
      (player->position->x > 1 && game->map[player->position->y + 2][player->position->x - 2] == 'B') ||*/
      (game->map[player->position->y][player->position->x] == 'B'))
    {
      //printf("has destination = %d\n", player->has_destination);
      if (player->has_destination == 0)
	{
	  player->is_hiding = 1;
	  //printf("Hiding\n");
	}
    }
  else if (player->lastBombHasExploded == 1)
    {
      player->is_hiding = 0;
      //printf("Chasing\n");
    }
  if (player->is_hiding == 0)
    {
      for (int i = 0; game->players[i] != NULL; i++)
	if (game->players[i]->is_ia == 0 && game->players[i]->is_dead == 0)
	  {
	    player->destination_x = game->players[i]->position->hitbox->x;
	    player->destination_y = game->players[i]->position->hitbox->y;
	  }
      if ((player->position->y - 1 > 0 && game->map[player->position->y - 1][player->position->x] == '1'  && player->position->yvel < 0) ||
	  (player->position->y < 16 && game->map[player->position->y + 1][player->position->x] == '1'  && player->position->yvel > 0) ||
	  (player->position->x < 16 && game->map[player->position->y][player->position->x + 1] == '1'  && player->position->xvel > 0) ||
	  (player->position->x > 0 && game->map[player->position->y][player->position->x - 1] == '1'  && player->position->xvel < 0) ||
	  (player->position->hitbox->x == player->destination_x && player->position->hitbox->y == player->destination_y))
	if (check_if_add_bomb_is_safe(game, player) == 1)
	  {
	    add_bomb(game, player);
	    player->has_destination = 0;
	  }
    }
  else if (player->has_destination == 0)
    {
      //printf("Looking for destination\n");
      player->has_destination = 1;
      if (player->position->y > 0 && player->position->x > 0 &&
	  game->map[player->position->y - 1][player->position->x] != 'X' && game->map[player->position->y - 1][player->position->x] != '1' &&
	  game->map[player->position->y - 1][player->position->x - 1] != 'X' && game->map[player->position->y - 1][player->position->x - 1] != '1')
	{
	  player->destination_x = (player->position->x - 1) * TILE_SIZE;
	  player->destination_y = (player->position->y - 1) * TILE_SIZE;
	}
      else if (player->position->y > 0 && player->position->x < 16 &&
	       game->map[player->position->y -1][player->position->x] != 'X' && game->map[player->position->y -1][player->position->x] != '1' &&
	       game->map[player->position->y - 1][player->position->x + 1] != 'X' && game->map[player->position->y - 1][player->position->x + 1] != '1')
	{

	  player->destination_x = (player->position->x + 1) * TILE_SIZE;
	  player->destination_y = (player->position->y - 1) * TILE_SIZE;
	}
      else if (player->position->y < 16 && player->position->x > 0 &&
	       game->map[player->position->y + 1][player->position->x] != 'X' && game->map[player->position->y + 1][player->position->x] != '1' &&
	       game->map[player->position->y + 1][player->position->x - 1] != 'X' && game->map[player->position->y + 1][player->position->x - 1] != '1')
	{
	  player->destination_x = (player->position->x - 1) * TILE_SIZE;
	  player->destination_y = (player->position->y + 1) * TILE_SIZE;
	}
      else if (player->position->y < 16 && player->position->x < 16 &&
	       game->map[player->position->y + 1][player->position->x] != 'X' && game->map[player->position->y + 1][player->position->x] != '1' &&
	       game->map[player->position->y + 1][player->position->x + 1] != 'X' && game->map[player->position->y + 1][player->position->x + 1] != '1')
	{
	  player->destination_x = (player->position->x + 1) * TILE_SIZE;
	  player->destination_y = (player->position->y + 1) * TILE_SIZE;
	}
      else
	{
	  player->destination_x = player->start_pos_x;
	  player->destination_y = player->start_pos_y;
	};
    }
  if (player->destination_x > player->position->hitbox->x)
    player->position->xvel = 1;
  else if (player->destination_x < player->position->hitbox->x)
    player->position->xvel = -1;
  else
    player->position->xvel = 0;
  if (player->destination_y > player->position->hitbox->y)
    player->position->yvel = 1;
  else if (player->destination_y < player->position->hitbox->y)
    player->position->yvel = -1;
  else
    player->position->yvel = 0;
  if (player->position->yvel != 0 || player->position->xvel != 0)
    player->walking = 1;
  else
    player->walking = 0;
  //printf("destination x = %d, y = %d\n",player->destination_x, player->destination_y);
}
