/*
** linked_list.c for bomberman in /home/infitek/Documents/Etna/Bomberman
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Fri May 25 10:05:03 2018 REBOUL Simon
** Last update Fri May 25 19:51:02 2018 REBOUL Simon
*/

#include "../headers/bomberman.h"
#include "../headers/functions.h"

int		add_container_to_game(t_game *game)
{
  t_container	*container;

  if ((container = malloc(sizeof(t_container) + 1)) == NULL)
    {
      my_putstr("Cannot allocate memory for object container\n");
      return 0;
    }
  container->first = NULL;
  container->last = NULL;
  container->nb_elems = 0;
  game->container = container;
  return 1;
}

void	add_object_to_container(t_game *game, t_object *object)
{
  t_object	*tmp;
  
  tmp = game->container->first;
  if (tmp == NULL)
    game->container->first = object;
  else
    {
      while (tmp->next != NULL)
	{
	  tmp = tmp->next;
	}
      tmp->next = object;
      object->prev = tmp;
    }
  game->container->last = object;
  game->container->nb_elems++;
}

void   del_object_from_container(t_game *game, t_object *object)
{
  t_object	*elem;

  elem = game->container->first;
  game->container->nb_elems--;
  while (elem != NULL && elem != object)
    elem = elem->next;
  if (elem->next != NULL && elem->prev != NULL)
    {
      elem->next->prev = elem->prev;
      elem->prev->next = elem->next;
    }
  else if (elem->next == NULL)
    {
      elem->prev->next = NULL;
      game->container->last = elem->prev;
    }
  else if (elem == game->container->first)
    {
      elem->next->prev = NULL;
      game->container->first = elem->next;
    }
  /*if (elem->hitbox)
    free(elem->hitbox);
  if (elem->tile_rect)
    free(elem->tile_rect)*/
  free(elem);
}

t_object	*create_object()
{
  t_object	*object;

  object = malloc(sizeof(t_object) + 1);
  object->next = NULL;
  object->prev = NULL;
  return object;
}
