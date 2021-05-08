#include <SDL.h>

struct tile{
      
};

int layout[8*12] = {
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,
};

void field(SDL_Renderer* renderer);

int main(int argc, char* argv[]){
      
      SDL_Init(SDL_INIT_EVERYTHING);

      SDL_Window* window = SDL_CreateWindow("tetris", 0,0,600,600,false);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

      while (true){
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){break;}
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderClear(renderer);
            
            field(renderer);

            SDL_RenderPresent(renderer);
      }

      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);


      SDL_Quit();
      
      return 0;
}

void field(SDL_Renderer* renderer){
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      SDL_Rect panel = {400,0,200,600};
      SDL_RenderFillRect(renderer, &panel);
      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
      panel = {420,200,160,200};
      SDL_RenderFillRect(renderer, &panel);
}