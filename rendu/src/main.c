/*
** main.c for ftl in /home/infitek/Documents/Etna/My_FTL/reboul_s
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Fri Nov 10 13:12:28 2017 REBOUL Simon
** Last update Thu Feb 14 14:14:53 2019 REBOUL Simon
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../headers/bomberman.h"
#include "../headers/functions.h"

int		main()
{
  t_game	game = {0};
  t_start_screen start_screen = {0};
  int	        run;
  SDL_Event	event;
  const int	FPS = 60;
  const int	frameDelay = 1000 / FPS;
  Uint32	frameStart;
  int		frameTime;

  event.type = 0;
  run = 1;
  srand(time(NULL));
  start_screen.screen = init_SDL(&start_screen);
  init_startup(&start_screen);
  Mix_PlayMusic(start_screen.audio->music, -1);
  Mix_VolumeMusic(0);
  Mix_VolumeMusic(MIX_MAX_VOLUME/2);
  while (run == 1)
  {
    while (SDL_PollEvent(&event))
      {
	if (start_screen.has_started == 1)
	  gere_keyboard(&game, game.players[0], event);
      }
    if (start_screen.has_started == 1)
      {
	gere_display(&game);
	gere_gameplay(&game);
      }
    else
      run = gere_intro(&start_screen, &game,event);
    frameStart = SDL_GetTicks();
    SDL_RenderPresent(start_screen.screen);
    SDL_RenderClear(start_screen.screen);
    //SDL_PollEvent(&event);
    switch(event.type)
	{
	case SDL_QUIT:
	  run = 0;
	  break;
    	}
    frameTime = SDL_GetTicks() - frameStart;
    if (game.time_left > 0 && start_screen.has_started == 1)
      game.time_left -= 1;
    if (frameDelay > frameTime)
      SDL_Delay(frameDelay - frameTime);
  }
    
  
  //if (game != NULL)
  //free(game);
  Mix_FreeMusic(start_screen.audio->music);
  Mix_CloseAudio();
  TTF_CloseFont(start_screen.police); 
  TTF_Quit();
  SDL_Quit();
  return EXIT_SUCCESS;
}
