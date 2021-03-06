/*
** gere_intro.c for bomberman in /home/infitek/Documents/Etna/Bomberman/rendu
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Thu Jun 28 16:03:02 2018 REBOUL Simon
** Last update Thu Feb 14 14:15:14 2019 REBOUL Simon
*/

#include "../headers/functions.h"


int	gere_intro(t_start_screen *start_screen, t_game *game, SDL_Event event)
{
  int x = 0;
  int y = 0;
  float w;
  float h;
  int pressed = 0;
  char	*map_name;
  SDL_Color black = {0, 0, 0, 0};
  SDL_Surface *surf = TTF_RenderText_Blended(start_screen->font, IP, black);
  if (SDL_GetWindowSurface(start_screen->window) == NULL)
    printf("SDL_Init failed: %s\n", SDL_GetError());
  //printf("Surface w = %d\n", SDL_GetWindowSurface(start_screen->window)->w);
  w = SDL_GetWindowSurface(start_screen->window)->w;
  h = SDL_GetWindowSurface(start_screen->window)->h;

  //SDL_GetMouseState(&x, &y);
  if (SDL_GetMouseState(&x, &y) && SDL_BUTTON(1))
    pressed = 1;
  x /= w / 670;
  y /= h / 480;
  if (start_screen->quitting == 1)
    {
      SDL_RenderCopy(start_screen->screen, start_screen->bgTexture, NULL, NULL);
      SDL_RenderCopy(start_screen->screen, start_screen->noButtonTexture, NULL, &start_screen->clientButton);
      SDL_RenderCopy(start_screen->screen, start_screen->yesButtonTexture, NULL, &start_screen->masterButton);
      if (x > start_screen->masterButton.x && x < (start_screen->masterButton.x + start_screen->masterButton.w)
	  && y > start_screen->masterButton.y && y < (start_screen->masterButton.y + start_screen->masterButton.h))
	{
	  if (pressed == 1)
	    return 0;
	}
      else if (x > start_screen->clientButton.x && x < (start_screen->clientButton.x + start_screen->clientButton.w)
	       && y > start_screen->clientButton.y && y < (start_screen->clientButton.y + start_screen->clientButton.h))
	if (pressed == 1)
	  start_screen->quitting = 0;
    }
  else if (start_screen->victory_screen == 1)
    {
      SDL_RenderCopy(start_screen->screen, start_screen->winBgTexture, NULL, NULL);
      SDL_RenderCopy(start_screen->screen, start_screen->winPlayerTexture, NULL, &start_screen->winPlayerPos);
      SDL_RenderCopy(start_screen->screen, start_screen->winButtonTexture, NULL, &start_screen->winButtonPos);
      if (x > start_screen->winButtonPos.x && x < (start_screen->winButtonPos.x + start_screen->winButtonPos.w)
	  && y > start_screen->winButtonPos.y && y < (start_screen->winButtonPos.y + start_screen->winButtonPos.h))
	if (pressed == 1)
	  start_screen->victory_screen = 0;
    }
  else if (start_screen->level_select == 0 && start_screen->connect_screen == 0)
    {
      SDL_RenderCopy(start_screen->screen, start_screen->bgTexture, NULL, NULL);
      SDL_RenderCopy(start_screen->screen, start_screen->clientButtonTexture, NULL, &start_screen->clientButton);
      SDL_RenderCopy(start_screen->screen, start_screen->masterButtonTexture, NULL, &start_screen->masterButton);
      SDL_RenderCopy(start_screen->screen, start_screen->quitButtonTexture, NULL, &start_screen->quitButton);
      if (x > start_screen->masterButton.x && x < (start_screen->masterButton.x + start_screen->masterButton.w)
	  && y > start_screen->masterButton.y && y < (start_screen->masterButton.y + start_screen->masterButton.h))
	{
	  if (pressed == 1)
	    {
	      map_name = my_strcat("./maps/", start_screen->level_previews[start_screen->level_index]);
	      map_name = my_strcat(map_name, ".png");
	      
	      start_screen->level_select = 1;
	      printf("Map name = %s\n", map_name);
	      start_screen->levelPreviewSprite = IMG_Load(map_name);
	      start_screen->levelPreviewTexture = SDL_CreateTextureFromSurface(start_screen->screen, start_screen->levelPreviewSprite);
	      start_screen->levelPreview.x = 80;
	      start_screen->levelPreview.y = 60;
	      start_screen->levelPreview.w = 300;
	      start_screen->levelPreview.h = 250;
	    }
	}
      else if (x > start_screen->clientButton.x && x < (start_screen->clientButton.x + start_screen->clientButton.w)
	       && y > start_screen->clientButton.y && y < (start_screen->clientButton.y + start_screen->clientButton.h))
	{
	  if (pressed == 1)
	    {
	      start_screen->connect_screen = 1;
	    }
	}
        else if (x > start_screen->quitButton.x && x < (start_screen->quitButton.x + start_screen->quitButton.w)
	       && y > start_screen->quitButton.y && y < (start_screen->quitButton.y + start_screen->quitButton.h))
	{
	  if (pressed == 1)
	    {
	      start_screen->quitting = 1;
	    }
	}
    }
  else if (start_screen->connect_screen == 1)
    {
      SDL_Rect pos;
      pos.y = SCREEN_H / 1.5f;
      pos.x = SCREEN_W / 2.75;
      SDL_BlitSurface(surf, NULL, start_screen->multiBgSprite, &pos);
      start_screen->multiBgTexture = SDL_CreateTextureFromSurface(start_screen->screen, start_screen->multiBgSprite);
      SDL_RenderCopy(start_screen->screen, start_screen->multiBgTexture, NULL, NULL);
      SDL_RenderCopy(start_screen->screen, start_screen->backButtonTexture, NULL, &start_screen->backButton);
      SDL_RenderCopy(start_screen->screen, start_screen->goButtonTexture, NULL, &start_screen->goButton);
      if (x > start_screen->backButton.x && x < (start_screen->backButton.x + start_screen->backButton.w)
	  && y > start_screen->backButton.y && y < (start_screen->backButton.y + start_screen->backButton.h))
	{
	  if (pressed == 1)
	    {
	      start_screen->connect_screen  = 0;
	    }
	}
          else if (x > start_screen->goButton.x && x < (start_screen->goButton.x + start_screen->goButton.w)
	  && y > start_screen->goButton.y && y < (start_screen->goButton.y + start_screen->goButton.h))
	{
	  if (pressed == 1)
	    {
	      initiate_connection(game, IP);
	    }
	}
    }
  else
    {
      SDL_RenderCopy(start_screen->screen, start_screen->levelSelectTexture, NULL, NULL);
      SDL_RenderCopy(start_screen->screen, start_screen->levelPreviewTexture, NULL, &start_screen->levelPreview);
      SDL_RenderCopy(start_screen->screen, start_screen->backButtonTexture, NULL, &start_screen->backButton);
      SDL_RenderCopy(start_screen->screen, start_screen->goButtonTexture, NULL, &start_screen->goButton);
      SDL_RenderCopy(start_screen->screen, start_screen->nextLevelButtonTexture, NULL, &start_screen->nextLevelButton);
      SDL_RenderCopy(start_screen->screen, start_screen->previousLevelButtonTexture, NULL, &start_screen->previousLevelButton);
      if (x > start_screen->backButton.x && x < (start_screen->backButton.x + start_screen->backButton.w)
	  && y > start_screen->backButton.y && y < (start_screen->backButton.y + start_screen->backButton.h))
	{
	  if (pressed == 1)
	    {
	      start_screen->level_select = 0;
	    }
	}
      else if (x > start_screen->goButton.x && x < (start_screen->goButton.x + start_screen->goButton.w)
	  && y > start_screen->goButton.y && y < (start_screen->goButton.y + start_screen->goButton.h))
	{
	  if (pressed == 1)
	    {
	      start_screen->level_select = 0;
	      char *music = my_strcat("./music/", start_screen->level_previews[start_screen->level_index]);
	      music = my_strcat(music, ".mid");
	      start_screen->audio->music = Mix_LoadMUS(music);
	      Mix_VolumeMusic(MIX_MAX_VOLUME/10);
	      Mix_PlayMusic(start_screen->audio->music, -1);
	      start_screen->map_chosen = my_strcat("./maps/", start_screen->level_previews[start_screen->level_index]);
	      init_game(game, start_screen);
	      start_screen->has_started = 1;
	    }
	}
      else if (x > start_screen->nextLevelButton.x && x < (start_screen->nextLevelButton.x + start_screen->nextLevelButton.w)
	  && y > start_screen->nextLevelButton.y && y < (start_screen->nextLevelButton.y + start_screen->nextLevelButton.h))
	{
	   if (pressed == 1)
	     {
	       if (start_screen->level_index < 4)
		 start_screen->level_index++;
	       else
		 start_screen->level_index = 0;
	       printf("start_screen->level_index = %d\n", start_screen->level_index);
	       map_name = my_strcat("./maps/", start_screen->level_previews[start_screen->level_index]);
	       map_name = my_strcat(map_name, ".png");
	       start_screen->levelPreviewSprite = IMG_Load(map_name);
	       start_screen->levelPreviewTexture = SDL_CreateTextureFromSurface(start_screen->screen, start_screen->levelPreviewSprite);
	       SDL_Delay(200);

	     }
	}
      else if (x > start_screen->previousLevelButton.x && x < (start_screen->previousLevelButton.x + start_screen->previousLevelButton.w)
	       && y > start_screen->previousLevelButton.y && y < (start_screen->previousLevelButton.y + start_screen->previousLevelButton.h))
	{
	  if (pressed == 1)
	    {
	      if (start_screen->level_index > 0)
		start_screen->level_index--;
	      else
		start_screen->level_index = 4;
	      printf("start_screen->level_index = %d\n", start_screen->level_index);
	      map_name = my_strcat("./maps/", start_screen->level_previews[start_screen->level_index]);
	      map_name = my_strcat(map_name, ".png");
	      start_screen->levelPreviewSprite = IMG_Load(map_name);
	      start_screen->levelPreviewTexture = SDL_CreateTextureFromSurface(start_screen->screen, start_screen->levelPreviewSprite);
	      SDL_Delay(200);
	    }
	}
    }
  return 1;
}
