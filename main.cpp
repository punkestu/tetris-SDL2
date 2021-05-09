#include <SDL.h>

#define LSHAPE 0
#define JSHAPE 1
#define ZSHAPE 2
#define SSHAPE 3
#define ISHAPE 4
#define TSHAPE 5
#define SQUARE 6

#define LEFT false
#define RIGHT true

#define CENTER {7,8}

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
void tShape(tile* _tile);
void squareS(tile* _tile);
void renTile(SDL_Renderer* renderer, tile* _tile);
void rePos(tile* _tile);
void moveTile(tile* _tile, bool dir);

void eraseLine();
bool falling(tile* _tile, int stackDel);
void layoutFall();

int randx(int min, int max);

int main(int argc, char* argv[]){
      
      SDL_Init(SDL_INIT_EVERYTHING);

      SDL_Window* window = SDL_CreateWindow("tetris", 0,0,600,600,false);
      SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

      tile fall;
      rePos(&fall);
      // squareS(&fall);
      // iShape(&fall);
      // sShape(&fall);
      // zShape(&fall);
      // lShape(&fall);
      // jShape(&fall);

      uint frmTime = SDL_GetTicks();

      bool drop = false;
      int stackDel = 0;

      while (true){
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){break;}
            if(e.type == SDL_KEYDOWN){
                  if(e.key.keysym.sym == SDLK_a){moveTile(&fall, LEFT);}
                  if(e.key.keysym.sym == SDLK_d){moveTile(&fall, RIGHT);}
                  if(e.key.keysym.sym == SDLK_s){drop = true;}
            }
            if(e.type == SDL_KEYUP){
                  if(e.key.keysym.sym == SDLK_s){drop = false;}
            }

            eraseLine();
            layoutFall();

            if(SDL_GetTicks()-frmTime >= 1000/(drop?20:10)){
                  if(drop){stackDel = 4;}
                  if(!falling(&fall, stackDel)){
                        if(stackDel < 3){
                              stackDel++;
                        }else{
                              rePos(&fall);
                              stackDel=0;
                        }
                  }else{
                        stackDel=0;
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
      _tile->prop[0]=-8;_tile->prop[1]=-7;
      _tile->prop[2]=7;_tile->prop[3]=8;
      _tile->shape = SSHAPE;
}

void zShape(tile* _tile){
      _tile->prop[0]=-10;_tile->prop[1]=-9;
      _tile->prop[2]=7;_tile->prop[3]=8;
      _tile->shape = ZSHAPE;
}

void lShape(tile* _tile){
      _tile->prop[0]=-25;_tile->prop[1]=-9;
      _tile->prop[2]=7;_tile->prop[3]=8;
      _tile->shape = LSHAPE;
}

void jShape(tile* _tile){
      _tile->prop[0]=-24;_tile->prop[1]=-8;
      _tile->prop[2]=7;_tile->prop[3]=8;
      _tile->shape = JSHAPE;
}

void iShape(tile* _tile){
      _tile->prop[0]=-40;_tile->prop[1]=-24;
      _tile->prop[2]=-8;_tile->prop[3]=8;
      _tile->shape = ISHAPE;
}

void tShape(tile* _tile){
      _tile->prop[0]=-10;_tile->prop[1]=-9;
      _tile->prop[2]=-8;_tile->prop[3]=7;
      _tile->shape = TSHAPE;
}

void squareS(tile* _tile){
      _tile->prop[0]=-9;_tile->prop[1]=-8;
      _tile->prop[2]=7;_tile->prop[3]=8;
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
      int param = randx(1,28)%7;
      switch (param){
            case SSHAPE: sShape(_tile); break;
            case ZSHAPE: zShape(_tile); break;
            case LSHAPE: lShape(_tile); break;
            case JSHAPE: jShape(_tile); break;
            case ISHAPE: iShape(_tile); break;
            case TSHAPE: tShape(_tile); break;
            case SQUARE: squareS(_tile); break;
            default: break;
      }
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

void eraseLine(){
      for(int i = 0; i < 24; i++){
            bool line = true;
            for(int j = 0; j < 16; j++){
                  if(layout[i*16+j] == 0){line = false;break;}
            }
            if(line){
                  for(int j = 0; j < 16; j++){
                        layout[i*16+j]=0;
                  }
            }
      }
}

bool falling(tile* _tile, int stackDel){
      bool stack = false;
      if(_tile->prop[3]/16<23){
            for(int i = 0; i < 4; i++){
                  if(layout[_tile->prop[i]+16]==1){
                        stack = true;
                        // SDL_Log("stack");
                        break;
                  }
            }
      }else{
            stack = true;
            // SDL_Log("stack");
      }

      if(stack){
            if(stackDel>2){
                  for(int i = 0; i < 4; i++){
                        layout[_tile->prop[i]]=1;
                        // SDL_Log("%d",_tile->prop[i]);
                  }
            }
      }else{
            for(int i = 0; i < 4; i++){
                  _tile->prop[i]+=16;
            }
      }

      return !stack;
}

void layoutFall(){
      for(int i = 0; i < 24; i++){
            bool line = true;
            for(int j = 0; j < 16; j++){
                  if(layout[i*16+j] == 1){line = false;break;}
            }
            if(line){
                  for(int j = i; j >= 0; j--){
                        for(int k = 0; k < 16; k++){
                              layout[j*16+k]=layout[(j-1)*16+k];
                        }
                  }
            }
      }
}

int randx(int min, int max){
      int ret;
      srand(SDL_GetTicks()%200*2-rand()-100+50);
      for(int i = 0; i < 1000; i++){
            ret = rand();
      }
      ret=(ret%(max-min+1))+min;
      return ret;
}