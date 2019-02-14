
/*
** network.c for  in /home/repos/bomberman
**
** Made by pereyr_a@etna-alternance.net
** Login   <pereyr_a@etna-alternance.net>
**
** Started on  Tue Jun 12 19:47:15 2018 pereyr_a@etna-alternance.net
** Last update Thu Feb 14 13:48:09 2019 REBOUL Simon
*/

#include "../headers/network.h"
#include "../headers/functions.h"

int create_bind_listen()
{
  int s = -1;
  int handler = -1;
  SOCKADDR_IN sin = { 0 };
  socklen_t recsize = sizeof(sin);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORT);
  sin.sin_family = AF_INET;

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("Can't create socket\n");
    return (-1);
  }

  if ((handler = bind(s, (SOCKADDR*)&sin, recsize)) == -1)
  {
    printf("Can't bind the socket\n");
    return (-1);
  }

  if ((handler = listen(s, 5)) == -1)
  {
    printf("Can't listen with our socket\n");
    return (-1);
  }
  return (s);
}

// Thread accepts new clients till' it reaches max players
void *accept_new_client(void *_game)
{
  t_game *game = _game;
  t_players *players = game->playersPtr;

  while (players->nb_players < MAX_PLAYERS)
  {
    t_player *new_player;
    SOCKADDR_IN	cli_sin;
    int	new_cli, count, ret1, ret2 ;
    pthread_t listen, send;
    socklen_t	crecsize = sizeof(cli_sin);
    printf("Checking if accepting client\n");
    new_cli = accept(game->socket, (SOCKADDR*)&cli_sin, &crecsize);
    new_player = init_player(game, new_cli);
    count = players->nb_players;
    count++;
    printf("We have now %d players\n", players->nb_players);

    // temp
    t_dude *dude = malloc(sizeof(t_dude));

    dude->player = new_player;
    dude->game = game;

    ret1 = pthread_create(&listen, NULL, listen_client, (void*)dude);
    ret2 = pthread_create(&send, NULL, send_data_server, (void*)game);

    if (ret1 || ret2)
      printf("Can't process client number %d\n", players->nb_players);
    else
      printf("Success processing client\n");
  }
  return (NULL);
}

void initiate_connection(t_game *game, char *server)
{
  SOCKADDR_IN sin;
  int cli_sock;

  cli_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (cli_sock != -1)
  {
    printf("We are going to connect to : %s\n", server);
    sin.sin_addr.s_addr = inet_addr(server);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;
    if (connect(cli_sock, (SOCKADDR *) &sin, sizeof(SOCKADDR)) != -1)
      printf("We are now connected\n");
    else
    {
      printf("Can't connect to server\n");
      return;
    }
  }
  else
  {
    printf("Can't create my socket\n");
    return;
  }
  cli_game_loop(game, cli_sock);
}

void *listen_client(void *_dude)
{
  t_dude *dude = _dude;
  t_player *player = dude->player;
  int han = 1;
  int moving = 0;
  int dir = 0;
  while (han)
  {
    int n = 0;
    char *buffer = malloc((512 + 1) * sizeof(char));  
    if ((n = recv(player->cli_socket, buffer, sizeof(buffer) -1, 0)) > 0)
    {
      printf("n = %d\n", n);
      buffer[n] = '\0';
      printf("Buffer recevied : %s\n", buffer);
      if (strcmp(buffer,"downP") == 0)
        {
	  moving = 1;
	  dir = 2;
	}
      else if (strcmp(buffer, "upP") == 0)
        {
	  moving = 1;
	  dir = 5;
	}
      else if (strcmp(buffer, "leftP") == 0)
        {
	  moving = 1; 
	  dir = 1;
	}
      else if (strcmp(buffer, "rightP") == 0)
        {
	  moving = 1;
	  dir = 3;
	}
      else if (strcmp(buffer,"downR") == 0)
	  moving = 0;
      else if (strcmp(buffer, "upR") == 0)
        moving = 0;
      else if (strcmp(buffer, "leftR") == 0)
        moving  = 0;
      else if (strcmp(buffer, "rightR") == 0)
        moving = 0;
      //else if (strcmp(buffer, "bomb") == 0)
        //place_bomb(dude->game, player);
      else if (strcmp(buffer, "quit") == 0)
        han = 0;
      free(buffer);
    }
    if (moving)
      gere_keyboard_network(dude->game, player, dir);
  }
  //drop_player(dude->game->players, player);
  return (NULL);
}

void send_data_client(int socket, const char *com)
{
  if (send(socket, com, sizeof(com) -1, 0) < 0)
  {
    printf("Can't send message\n");
  }
}

t_player *check_is_user_already_exist(t_game *game , int id)
{
  t_player *player = game->players[0];
  while (player != NULL)
  {
    if (player->id == id)
      return (player);
    player = player->next;
  }
  return (NULL);
}

void *recieve_data_client(void *_cli_struct)
{
  t_cli_struct *cli_struct = _cli_struct;
  t_game *game = cli_struct->game;

  int cli_sock = cli_struct->cli_socket;
  SOCKADDR_IN	*sin;
  socklen_t	recsize = sizeof(sin);
  t_data data = {0, 0, 0, 0};
  while(recvfrom(cli_sock, &data, sizeof(data), 0, (SOCKADDR*) &sin, &recsize))
  {
    t_player *player = check_is_user_already_exist(game, data.playerNumber);

    if (player == NULL)
    {
      player = init_player(0, data.playerNumber);
      //add_player_to_list(game->players, player);
    }

    if (data.hasBomb)
    {
      if (player->actual_bombs < player->nb_bombs)
      {
	my_putstr("SHOULD PLACING BOMB HERE");
        //place_bomb(game, player);
      }
    }
    player->position->pos->x = data.x;
    player->position->pos->y = data.y;
  }
  return (NULL);
}

void *send_data_server(void *_game)
{
  t_game *game = _game;
  t_players *players = game->playersPtr;
  t_player *player, *playerToSend = NULL;
  SOCKADDR_IN	sin = { 0 };
  socklen_t	recsize = sizeof(sin);

  while (game->has_win != 1)
  {
    playerToSend = players->first;
    while (playerToSend != NULL)
    {
      player = players->first;
      while (player != NULL && player->id >= 1 && player->id <= 4)
      {
        t_data data = {0, 0, 0, 0};
        data.playerNumber = player->id;
        data.x = player->position->pos->x;
        data.y = player->position->pos->y;
        data.hasBomb = player->actual_bombs;

        if (playerToSend->cli_socket != 0)
        {
          sendto(playerToSend->cli_socket, &data, sizeof(data), 0, (SOCKADDR*) &sin, sizeof(recsize));
          usleep(10000);
        }
	if (player->next != NULL)
	  player = player->next;
	//printf("player %d found! \n", player->id + 1);
      }
      playerToSend = playerToSend->next;
    }
  }
  return (NULL);
}
