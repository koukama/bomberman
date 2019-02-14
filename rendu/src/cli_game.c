/*
** cli_game.c for  in /home/repos/bomberman/src
**
** Made by pereyr_a@etna-alternance.net
** Login   <pereyr_a@etna-alternance.net>
**
** Started on  Sun Jun 17 00:10:35 2018 pereyr_a@etna-alternance.net
** Last update Thu Feb 14 14:14:38 2019 REBOUL Simon
*/

#include "../headers/functions.h"
#include "../headers/bomberman.h"
#include "../headers/network.h"

void cli_game_loop(t_game *game, int cli_sock)
{
  SDL_Event event;
  int han, ret = 1;
  pthread_t listen_srv;
  //  game->players = init_list();

  t_cli_struct *cli_struct;
  cli_struct = malloc(sizeof(t_cli_struct));
  cli_struct->cli_socket = cli_sock;
  cli_struct->game = game;
  ret = pthread_create(&listen_srv, NULL, recieve_data_client, (void*)cli_struct);
  if (ret)
    printf("Can't launch thread for recieving data client\n");

  while (han)
  {
    gere_display(game);
    //place_users(game);
    SDL_PollEvent(&event);
    switch (event.type)
      {
        case SDL_QUIT:
          han = 0;
          break;
      case SDL_KEYUP:
	  {
	  case SDLK_RIGHT:
	    send_data_client(cli_sock, "rightR");
	    break;
	  case SDLK_LEFT:
	    send_data_client(cli_sock, "leftR");
	    break;
	  case SDLK_UP:
	    send_data_client(cli_sock, "upR");
	    break;
	  case SDLK_DOWN:
	    send_data_client(cli_sock, "downR");
	    break;
	  case SDLK_ESCAPE:
	    han = 0;
	    break;
	  case SDLK_SPACE:
	      send_data_client(cli_sock, "bomb");
	    break;
	  }
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
            {
              case SDLK_RIGHT:
                  send_data_client(cli_sock, "rightP");
                break;
              case SDLK_LEFT:
                  send_data_client(cli_sock, "leftP");
                break;
              case SDLK_UP:
                  send_data_client(cli_sock, "upP");
                break;
              case SDLK_DOWN:
                  send_data_client(cli_sock, "downP");
                break;
              case SDLK_ESCAPE:
                han = 0;
                break;
            }
        break;
      }
  }
  send_data_client(cli_sock, "quit");
  // https://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
  shutdown(cli_sock, 2);
}
