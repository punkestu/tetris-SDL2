#include <SDL.h>

#define LSHAPE 0
#define JSHAPE 1
#define ZSHAPE 2
#define SSHAPE 3
#define ISHAPE 4
#define SQUARE 5

#define LEFT false
#define RIGHT true

struct tile{
      int prop[4];
      int shape;
};

int layout[16*24] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void field(SDL_Renderer* renderer);

void sShape(tile* _tile);
void zShape(tile* _tile);
void lShape(tile* _tile);
void jShape(tile* _tile);
void iShape(tile* _tile);
void squareS(tile* _tile);
void renTile(SDL_Renderer* renderer, tile* _tile);
void rePos(tile* _tile);
void moveTile(tile* _tile, bool dir);

bool falling(tile* _tile);

int main(int argc, char* argv[]){
      
      SDL_Init(SDL_INIT_EVERYTHING);

      SDL_Window* window = SDL_CreateWindow("tetris", 0,0,600,600,false);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

      tile fall;
      squareS(&fall);
      // iShape(&fall);
      // sShape(&fall);
      // zShape(&fall);
      // lShape(&fall);
      // jShape(&fall);

      uint frmTime = SDL_GetTicks();

      while (true){
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){break;}
            if(e.type == SDL_KEYDOWN){
                  if(e.key.keysym.sym == SDLK_a){moveTile(&fall, LEFT);}
                  if(e.key.keysym.sym == SDLK_d){moveTile(&fall, RIGHT);}
            }

            if(SDL_GetTicks()-frmTime >= 1000/10){
                  if(!falling(&fall)){
                        rePos(&fall);
                  }
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
      _tile->prop[0]=-5;_tile->prop[1]=-4;
      _tile->prop[2]=2;_tile->prop[3]=3;
      _tile->shape = SSHAPE;
}

void zShape(tile* _tile){
      _tile->prop[0]=-6;_tile->prop[1]=-5;
      _tile->prop[2]=3;_tile->prop[3]=4;
      _tile->shape = ZSHAPE;
}

void lShape(tile* _tile){
      _tile->prop[0]=-13;_tile->prop[1]=-5;
      _tile->prop[2]=3;_tile->prop[3]=4;
      _tile->shape = LSHAPE;
}

void jShape(tile* _tile){
      _tile->prop[0]=-12;_tile->prop[1]=-4;
      _tile->prop[2]=3;_tile->prop[3]=4;
      _tile->shape = JSHAPE;
}

void iShape(tile* _tile){
      _tile->prop[0]=-20;_tile->prop[1]=-12;
      _tile->prop[2]=-4;_tile->prop[3]=4;
      _tile->shape = ISHAPE;
}

void squareS(tile* _tile){
      _tile->prop[0]=-8;_tile->prop[1]=-7;
      _tile->prop[2]=8;_tile->prop[3]=9;
      _tile->shape = SQUARE;
}

void renTile(SDL_Renderer* renderer, tile* _tile){
      SDL_Rect tileD;
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      for(int i = 0; i < 4; i++){
            tileD = {_tile->prop[i]%16*25,_tile->prop[i]/16*25,24,24};
            SDL_RenderDrawRect(renderer,&tileD);
      }
      for(int i = 0; i < 16*24; i++){
            if(layout[i] == 1){
                  tileD = {i%16*25,i/16*25,24,24};
                  SDL_RenderDrawRect(renderer,&tileD);
            }
      }
}

void rePos(tile* _tile){
      squareS(_tile);
}

void moveTile(tile* _tile, bool dir){
      bool canMove = true;
      if(dir){
            for(int i = 0; i < 4; i++){
                  if(_tile->prop[i]%16==15){
                        canMove = false;
                        break;
                  }
                  if(layout[_tile->prop[i]+1] == 1){
                        canMove = false;
                        break;
                  }
            }
            if(canMove){
                  for(int i = 0; i < 4; i++){
                        _tile->prop[i]++;
                  }
            }
      }else{
            for(int i = 0; i < 4; i++){
                  if(_tile->prop[i]%16==0){
                        canMove = false;
                        break;
                  }
                  if(layout[_tile->prop[i]-1] == 1){
                        canMove = false;
                        break;
                  }
            }
            if(canMove){
                  for(int i = 0; i < 4; i++){
                        _tile->prop[i]--;
                  }
            }
      }
}

bool falling(tile* _tile){
      bool stack = false;
      if(_tile->prop[3]/16<23){
            for(int i = 0; i < 4; i++){
                  if(layout[_tile->prop[i]+16]==1){
                        stack = true;
                        SDL_Log("stack");
                        break;
                  }
            }
      }else{
            stack = true;
            // SDL_Log("stack");
      }

      if(stack){
            for(int i = 0; i < 4; i++){
                  layout[_tile->prop[i]]=1;
                  SDL_Log("%d",_tile->prop[i]);
            }
      }else{
            for(int i = 0; i < 4; i++){
                  _tile->prop[i]+=16;
            }
      }

      return !stack;
}