   /*deleted most functions and editted (removed bits) two of the functions from,
neillsdl2.c*/
   /*for some reason im getting leaks whenever i exit the the the output file, I'm
using xLaunch to visualise the output file*/

#include "adaptedsdl2.h"

/* Do something standard everytime an error is trapped */
#define ON_ERROR(STR) fprintf(stderr, "\n%s: %s\n", STR, SDL_GetError()); SDL_Quit(); exit(EXIT_FAILURE);

/* Set up a simple (WIDTH, HEIGHT) window.
   Attempt to hide the renderer etc. from user. */
void Adapted_SDL_Init(SDL_Simplewin *sw)
{

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      ON_ERROR("Unable to initialize SDL");
   }

   sw->finished = SDL_FALSE;
   /*create SDL window*/
   sw->win= SDL_CreateWindow("Drawn Turtle File",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      ON_ERROR("Unable to initialize SDL Window");
   }
   /*create renderer*/
   sw->renderer = SDL_CreateRenderer(sw->win, DEFAULTRNDR, NORENDFLGS);
   if(sw->renderer == NULL){
      ON_ERROR("Unable to initialize SDL Renderer");
   }

   /*Set a blank white background*/
   SDL_SetRenderDrawColor(sw->renderer, BGRED, BGGREEN, BGBLUE, OPAQUE);
   SDL_RenderClear(sw->renderer);

   /*set line to black*/
   SDL_SetRenderDrawColor(sw->renderer, LNRED, LNGREEN, LNBLUE, OPAQUE);

   /*Set middle of screen as starting point*/
   sw->x1 = WWIDTH MIDPOINT;
   sw->y1 = WHEIGHT MIDPOINT;
   /*clear other vars*/
   sw->x2 = sw->y2 = sw->angle = 0;
}
/* Housekeeping to do with the render buffer & updating the screen */
void Adapted_SDL_UpdateScreen(SDL_Simplewin *sw)
{
   SDL_RenderPresent(sw->renderer);
}
/* Gobble all events & ignore most */
void Neill_SDL_Events(SDL_Simplewin *sw)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sw->finished = SDL_TRUE;
       }
    }
}
/*clean exit from SDL*/
void Exit_SDL(SDL_Simplewin *sw)
{
   SDL_DestroyRenderer(sw->renderer);
   SDL_DestroyWindow(sw->win);
   /*SDL_Quit();*/
}
