#include <SDL.h>

#define WWIDTH 800
#define WHEIGHT 600
#define BGRED 255
#define BGGREEN 255
#define BGBLUE 255
#define LNRED 0
#define LNGREEN 0
#define LNBLUE 0
#define OPAQUE 255
#define MIDPOINT /2
#define DEFAULTRNDR -1
#define NORENDFLGS 0

typedef struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
   int x1;
   int y1;
   int x2;
   int y2;
   int angle;
} SDL_Simplewin;

void Adapted_SDL_Init(SDL_Simplewin *sw);
void Adapted_SDL_UpdateScreen(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
void Exit_SDL(SDL_Simplewin *sw);
