#include <SDL.h>

#define LSHAPE 0
#define JSHAPE 1
#define ZSHAPE 2
#define SSHAPE 3
#define ISHAPE 4
#define SQUARE 5

struct tile{
      int prop[4];
      int shape;
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

void sShape(tile* _tile);
void zShape(tile* _tile);
void lShape(tile* _tile);
void jShape(tile* _tile);
void iShape(tile* _tile);
void squareS(tile* _tile);
void renTile(SDL_Renderer* renderer, tile* _tile);

bool falling(tile* _tile);

int main(int argc, char* argv[]){
      
      SDL_Init(SDL_INIT_EVERYTHING);

      SDL_Window* window = SDL_CreateWindow("tetris", 0,0,600,600,false);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

      tile fall;
      // squareS(&fall);
      iShape(&fall);

      uint frmTime = SDL_GetTicks();

      while (true){
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){break;}

            if(SDL_GetTicks()-frmTime >= 1000/10){
                  falling(&fall);
                  frmTime = SDL_GetTicks();
            }

            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderClear(renderer);
            
            field(renderer);
            renTile(renderer, &fall);

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

void sShape(tile* _tile){

}

void zShape(tile* _tile){
      
}

void lShape(tile* _tile){
      
}

void jShape(tile* _tile){
      
}

void iShape(tile* _tile){
      _tile->prop[0]=-20;_tile->prop[1]=-12;
      _tile->prop[2]=-4;_tile->prop[3]=4;
      _tile->shape = ISHAPE;
}

void squareS(tile* _tile){
      _tile->prop[0]=-5;_tile->prop[1]=-4;
      _tile->prop[2]=3;_tile->prop[3]=4;
      _tile->shape = SQUARE;
}

void renTile(SDL_Renderer* renderer, tile* _tile){
      SDL_Rect tileD;
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      for(int i = 0; i < 4; i++){
            tileD = {_tile->prop[i]%8*50,_tile->prop[i]/8*50,50,50};
            SDL_RenderDrawRect(renderer,&tileD);
      }
}

bool falling(tile* _tile){
      bool stack = false;
      if(_tile->prop[3]/8!=11){
            for(int i = 0; i < 4; i++){
                  if(layout[_tile->prop[i]]+8==1){
                        stack = true;
                        break;
                  }
            }
      }else{
            stack = true;
      }

      if(stack){
            for(int i = 0; i < 4; i++){
                  layout[_tile->prop[i]]=1;
            }
      }else{
            for(int i = 0; i < 4; i++){
                  _tile->prop[i]+=8;
            }
      }

      return !stack;
}