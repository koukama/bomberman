/*
** network.h for  in /home/repos/bomberman
**
** Made by pereyr_a@etna-alternance.net
** Login   <pereyr_a@etna-alternance.net>
**
** Started on  Tue Jun 12 19:49:43 2018 pereyr_a@etna-alternance.net
** Last update Sat Jun 16 17:13:43 2018 pereyr_a@etna-alternance.net
*/

#ifndef __NETWORK_H__
#define __NETWORK_H__

#define PORT 5000

#include "bomberman.h"

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

typedef struct s_data
{
  int playerNumber;
  int x;
  int y;
  int hasBomb;
} t_data;

typedef struct s_cli_struct
{
  int cli_socket;
  t_game *game;
} t_cli_struct;

typedef struct s_dude
{
  t_player *player;
  t_game *game;
} t_dude;



#endif
