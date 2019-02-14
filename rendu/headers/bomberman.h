/*
** bomberman.h for bomberman in /home/infitek/Documents/Etna/Bomberman
** 
** Made by REBOUL Simon
** Login   <reboul_s@etna-alternance.net>
** 
** Started on  Fri May  4 16:40:03 2018 REBOUL Simon
** Last update Thu Feb 14 10:16:44 2019 REBOUL Simon
*/

#ifndef __BOMBERMAN_H__
# define __BOMBERMAN_H__
# define SCREEN_W	672
# define SCREEN_H	480 
# define MAP_MARGIN_X	32
# define MAP_MARGIN_Y	32 
# define TILE_SIZE	32
# define ANIM_SPEED	10
# define WALK_SPEED     2
# define SCALE		1
# define EMPTY_TILE_INDEX 329
# define ICE_TILE_INDEX  356
# define BASETIME 999999
# define MAX_PLAYERS 4
# define IP "127.0.0.1"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

typedef struct	s_game		t_game;
typedef struct	s_player	t_player;
typedef struct	s_players	t_players;
typedef struct	s_position	t_position;
typedef struct	s_container	t_container;
typedef struct	s_object	t_object;
typedef struct	s_solid_object	t_solid_object;
typedef struct	s_non_solid_object t_non_solid_object;
typedef struct	s_bomb		t_bomb;
typedef	struct	s_flame		t_flame;
typedef struct	s_audio		t_audio;
typedef struct	s_ui		t_ui;
typedef struct	s_start_screen	t_start_screen;

struct	s_game
{
  SDL_Renderer	*screen;
  SDL_Surface	*flames_sprite;
  SDL_Texture	*flames_tex;
  SDL_Surface	*items_sprite;
  SDL_Texture	*items_tex;
  SDL_Surface	*background;
  SDL_Texture	*background_tex;
  SDL_Surface	*tileset;
  SDL_Texture	*tileset_tex;
  SDL_Texture	*panel;
  SDL_Rect	panelLeft;
  SDL_Rect	panelRight;
  SDL_Surface	*timeCounter;
  Uint32	time_count;
  int		time_out;
  SDL_Texture	*win;
  SDL_Texture	*lose;
  int		has_win;
  t_container	*container;
  t_player	**players;
  t_players	*playersPtr;	
  t_audio	*audio;
  int		time_left;
  t_ui		*ui;
  t_start_screen	*start_screen;
  int		this_player_index;
  char		**map;
  int		socket;
};

struct s_players
{
  int nb_players;
  t_player *first;
  t_player *last;
};


struct s_ui
{
  SDL_Surface	*text;
  SDL_Texture	*text_tex;
  SDL_Rect	text_pos;
  SDL_Surface	*starsSprite;
  SDL_Texture	*starsTexture;
  SDL_Rect	stars_pos;
  SDL_Surface	*playerPortrait;
  SDL_Texture	*playerPortraitTexture;
  SDL_Rect	**playerPortraitPos;
  SDL_Rect	**playerPortraitTilePos;
};

struct s_start_screen
{
  SDL_Renderer	*screen;
  SDL_Window	*window;
  TTF_Font	*police;
  TTF_Font	*font;
  SDL_Color	fontColor;
  SDL_Color	fontColorBg;
  int		victory_screen;
  int		connect_screen;
  int		level_select;
  int		has_started;
  int		quitting;
  char		**level_previews;

  t_audio	*audio;
  SDL_Surface	*bgSprite;
  SDL_Texture	*bgTexture;
  SDL_Surface	*multiBgSprite;
  SDL_Texture	*multiBgTexture;
  SDL_Surface	*clientButtonSprite;
  SDL_Texture	*clientButtonTexture;
  SDL_Surface	*levelSelectSprite;
  SDL_Texture	*levelSelectTexture;
  SDL_Surface	*levelPreviewSprite;
  SDL_Texture	*levelPreviewTexture;
  SDL_Rect	levelPreview;
  SDL_Surface	*masterButtonSprite;
  SDL_Texture	*masterButtonTexture;
  SDL_Surface	*yesButtonSprite;
  SDL_Texture	*yesButtonTexture;
  SDL_Surface	*noButtonSprite;
  SDL_Texture	*noButtonTexture;
  SDL_Surface	*quitButtonSprite;
  SDL_Texture	*quitButtonTexture;
  SDL_Rect	clientButton;
  SDL_Rect	masterButton;
  SDL_Rect	quitButton;
  SDL_Surface	*backButtonSprite;
  SDL_Texture	*backButtonTexture;
  SDL_Rect	backButton;
  SDL_Surface	*goButtonSprite;
  SDL_Texture	*goButtonTexture;
  SDL_Rect	goButton;
  SDL_Surface	*nextLevelButtonSprite;
  SDL_Texture	*nextLevelButtonTexture;
  SDL_Rect      nextLevelButton;
  SDL_Surface	*previousLevelButtonSprite;
  SDL_Texture	*previousLevelButtonTexture;
  SDL_Rect      previousLevelButton;
  int		level_index;
  char		*map_chosen;
  SDL_Rect      winPlayerPos;
  SDL_Surface	*winPlayerSprite;
  SDL_Texture	*winPlayerTexture;
  SDL_Surface	*winBgSprite;
  SDL_Texture	*winBgTexture;
  SDL_Surface	*winButtonSprite;
  SDL_Texture	*winButtonTexture;
  SDL_Rect      winButtonPos;
};

struct s_audio
{
  Mix_Music	*music;
  Mix_Chunk	*dropBomb;
  Mix_Chunk	*bombExplode;
  Mix_Chunk	*playerDead;
  Mix_Chunk	*malusGet;
  Mix_Chunk	*bonusGet;
};

struct	s_player
{
  int		id;
  SDL_Surface	*sprite;
  SDL_Texture	*sprite_tex;
  int		walking;
  SDL_Rect	*anim_rect;
  int		anim_tick;
  int		anim_duration;
  int		anim_index;
  int		anim_forward;
  int		last_anim_dir;
  float		speed;
  int		nb_bombs;
  int		actual_bombs;
  int	        flame_range;
  t_position	*position;
  int		is_dead;
  int		is_ia;
  int		is_chasing;
  int		is_hiding;
  int		has_destination;
  int		destination_x;
  int		destination_y;
  int		is_sliding;
  SDL_Rect	statNbBombsPos;
  SDL_Rect	statSpeedPos;
  SDL_Rect	statRangePos;
  int		waitTimer;
  int		lastBombHasExploded;
  int		start_pos_x;
  int		start_pos_y;
  int		cli_socket;
  struct s_player *next;
  struct s_player *prev;
};

struct s_position
{
  SDL_Rect	*pos;
  int		last_x;
  int		last_y;
  SDL_Rect	*hitbox;
  float		xvel;
  float		yvel;
  int		dir; // As the numpad : 1 = left, 2 = down, 3 = right, 5 = up
  int		second_dir;
  int		last_dir;
  int		x;
  int		y;
};

struct s_container
{
  int		nb_elems;
  t_object	*first;
  t_object	*last;
};

struct s_object
{
  int		x;
  int		y;
  int		deadly;
  int		is_falling;
  int		fall_offset;
  SDL_Surface	*sprite;
  SDL_Texture	*sprite_tex;
  SDL_Rect	*tile_rect;
  SDL_Rect	*hitbox;
  char		*name;
  int		id;
  int		dir; // As the numpad : 1 = left, 2 = down, 3 = right, 5 = up
  t_object	*next;
  t_object	*prev;
  int		is_ice;
  int		is_solid;
  int		is_destructible;
  int		is_destruction_effect;
  int		is_over_player;
  int		is_damaging;
  int		is_animated;
  int		is_bomb;
  int		is_flame;
  int		is_gonna_disappear;
  SDL_Rect	*anim_rect;
  int		anim_tick;
  int		anim_duration;
  int		anim_index;
  int		anim_forward;
  int		timer;
  int		range;
  int		flame_dir;
  int		distance_from_origin;
  int		bomb_range;
  int		owned_by;
  int		is_bonus;
  int		rangeEffect;
  int		nbBombsEffect;
  float		speedEffect;
  int		canSpawnBonus;
  t_object	*parent;
  int		player_id;
};
  
#endif /*__BOMBERMAN_H__*/
