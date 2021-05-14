#include <SDL.h>
#include <iostream>

#define LSHAPE 0
#define JSHAPE 1
#define ZSHAPE 2
#define SSHAPE 3
#define ISHAPE 4
#define TSHAPE 5
#define SQUARE 6

#define LEFT false
#define RIGHT true

#define WIDTH 16
#define HEIGHT 24

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
bool rePos(tile* _tile);
void moveTile(tile* _tile, bool dir);

void rotate(tile* _tile);
void rRotate(tile* _tile);

int eraseLine();
bool falling(tile* _tile, int stackDel);
void layoutFall();

int randx(int min, int max);
bool focus(SDL_Window* window, SDL_Event *e, bool recent);

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
      uint tileTime = SDL_GetTicks();

      bool drop = false;
      int stackDel = 0;
      // for(int i = 0; i < 24; i++){
      //       for(int j = 0; j < 16; j++){
      //             std::cout<<layout[i*16+j]<<" ";
      //       }
      //       std::cout<<std::endl;
      // }

      while (true){

            int erased = 0;
            SDL_Event e;
            SDL_PollEvent(&e);
            if(e.type == SDL_QUIT){break;}

            if(e.type == SDL_KEYDOWN){
                  // if(SDL_GetTicks()-btnP>=1000/200){
                  if(e.key.keysym.sym == SDLK_a){moveTile(&fall, LEFT);}
                  if(e.key.keysym.sym == SDLK_d){moveTile(&fall, RIGHT);}
                  if(e.key.keysym.sym == SDLK_s){drop = true;}
                  if(e.key.keysym.sym == SDLK_l){rotate(&fall);}
                  if(e.key.keysym.sym == SDLK_j){rRotate(&fall);}
                  // }
            }
            if(e.type == SDL_KEYUP){
                  if(e.key.keysym.sym == SDLK_s){drop = false;}
            }
            // SDL_Log("%d %d %d %d",fall.prop[0],fall.prop[1],fall.prop[2],fall.prop[3]);
            if(SDL_GetTicks()-tileTime >= 1000/(drop?20:4)){
                  erased = eraseLine();
                  layoutFall();
                  if(drop){stackDel = 4;}
                  if(!falling(&fall, stackDel)){
                        if(stackDel < 3){
                              stackDel++;
                        }else{
                              if(!rePos(&fall)){
                                    break;
                              }
                              stackDel=0;
                        }
                  }else{
                        stackDel=0;
                  }
                  tileTime = SDL_GetTicks();
            }     
            if(SDL_GetTicks()-frmTime >= 1000/30){       
                  SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                  SDL_RenderClear(renderer);
                  field(renderer);
                  renTile(renderer, &fall);
                  frmTime = SDL_GetTicks();
                  SDL_RenderPresent(renderer);
            }
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
      _tile->prop[0]=-8+(1*WIDTH);_tile->prop[1]=-7+(1*WIDTH);
      _tile->prop[2]=7+(1*WIDTH);_tile->prop[3]=8+(1*WIDTH);
      _tile->shape = SSHAPE;
}

void zShape(tile* _tile){
      _tile->prop[0]=(1*WIDTH)-10;_tile->prop[1]=(1*WIDTH)-9;
      _tile->prop[2]=(1*WIDTH)+7;_tile->prop[3]=(1*WIDTH)+8;
      _tile->shape = ZSHAPE;
}

void lShape(tile* _tile){
      _tile->prop[0]=(1*WIDTH)-25;_tile->prop[1]=(1*WIDTH)-9;
      _tile->prop[2]=(1*WIDTH)+7;_tile->prop[3]=(1*WIDTH)+8;
      _tile->shape = LSHAPE;
}

void jShape(tile* _tile){
      _tile->prop[0]=(1*WIDTH)-24;_tile->prop[1]=(1*WIDTH)-8;
      _tile->prop[2]=(1*WIDTH)+7;_tile->prop[3]=(1*WIDTH)+8;
      _tile->shape = JSHAPE;
}

void iShape(tile* _tile){
      _tile->prop[2]=(1*WIDTH)-8;
      _tile->prop[0]=_tile->prop[2]-2;
      _tile->prop[1]=_tile->prop[2]-1;
      _tile->prop[3]=_tile->prop[2]+1;
      _tile->shape = ISHAPE;
}

void tShape(tile* _tile){
      _tile->prop[1]=(3*WIDTH)-9;
      _tile->prop[0]=_tile->prop[1]-1;
      _tile->prop[2]=_tile->prop[1]+1;
      _tile->prop[3]=_tile->prop[1]+WIDTH;
      _tile->shape = TSHAPE;
}

void squareS(tile* _tile){
      _tile->prop[0]=(1*WIDTH)-9;_tile->prop[1]=(1*WIDTH)-8;
      _tile->prop[2]=(1*WIDTH)+7;_tile->prop[3]=(1*WIDTH)+8;
      _tile->shape = SQUARE;
}

void renTile(SDL_Renderer* renderer, tile* _tile){
      SDL_Rect tileD;
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      for(int i = 0; i < 4; i++){
            tileD = {_tile->prop[i]%WIDTH*25,_tile->prop[i]/WIDTH*25,24,24};
            SDL_RenderFillRect(renderer,&tileD);
      }
      for(int i = 0; i < WIDTH*24; i++){
            if(layout[i] == 1){
                  tileD = {i%WIDTH*25,i/WIDTH*25,24,24};
                  SDL_RenderFillRect(renderer,&tileD);
            }
      }
}

bool rePos(tile* _tile){
      if(layout[WIDTH/2-2] == 1 || layout[WIDTH/2-1] == 1 || 
         layout[WIDTH/2] == 1 || layout[WIDTH/2+1] == 1){
            std::cout<<"game over"<<std::endl;
            return false;
      }else{
            int param = randx(1,28)%7;
            // param = ISHAPE;
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
            return true;
      }
}

void moveTile(tile* _tile, bool dir){
      bool canMove = true;
      if(dir){
            for(int i = 0; i < 4; i++){
                  if(_tile->prop[i]%WIDTH==WIDTH-1){
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
                  if(_tile->prop[i]%WIDTH==0){
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

void rotate(tile* _tile){
      // if(_tile->prop[0]<_tile->prop[1]){
      //       if(layout[_tile->prop[3]+1] == 0 && layout[_tile->prop[3]+WIDTH+1] == 0 && _tile->prop[3]/WIDTH < HEIGHT-1){
      //             _tile->prop[2]=_tile->prop[3]+1;
      //             _tile->prop[1]=_tile->prop[0];
      //             _tile->prop[0]=_tile->prop[3];
      //             _tile->prop[3]+=WIDTH+1;
      //       }
      // }else if(_tile->prop[1]<_tile->prop[0]){
      //       if(layout[_tile->prop[0]-1] == 0 && layout[_tile->prop[1]+1] == 0 && _tile->prop[0]%WIDTH > 0){
      //             _tile->prop[3]=_tile->prop[0];
      //             _tile->prop[2]=_tile->prop[3]-1;
      //             _tile->prop[0]=_tile->prop[1];
      //             _tile->prop[1]=_tile->prop[0]+1;
      //       }
      // }
      int position;
      switch (_tile->shape)
      {
      case SSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  position = 1;
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  position = 2;
            }
            if(position == 1){
                  if(layout[_tile->prop[3]+1] == 0 && layout[_tile->prop[3]+WIDTH+1] == 0 && _tile->prop[3]/WIDTH < HEIGHT-1){
                        _tile->prop[2]=_tile->prop[3]+1;
                        _tile->prop[1]=_tile->prop[0];
                        _tile->prop[0]=_tile->prop[3];
                        _tile->prop[3]+=WIDTH+1;
                  }
            }
            if(position == 2){
                  if(layout[_tile->prop[0]-1] == 0 && layout[_tile->prop[1]+1] == 0 && _tile->prop[0]%WIDTH > 0){
                        _tile->prop[3]=_tile->prop[0];
                        _tile->prop[2]=_tile->prop[3]-1;
                        _tile->prop[0]=_tile->prop[1];
                        _tile->prop[1]=_tile->prop[0]+1;
                  }
            }
            break;
      case ZSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  position = 1;
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  position = 2;
            }
            if(position==1){
                  if(layout[_tile->prop[2]+WIDTH-1] == 0 && _tile->prop[3]/WIDTH < HEIGHT-1){
                        _tile->prop[0] += WIDTH;
                        _tile->prop[3] = _tile->prop[0]+WIDTH;
                  }
            }
            if(position==2){
                  if(layout[_tile->prop[2]+1] == 0 && _tile->prop[2]%WIDTH < WIDTH-1){
                        _tile->prop[0] -= WIDTH;
                        _tile->prop[3] = _tile->prop[2]+1;
                  }
            }
            break;
      case LSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 4;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 2;
                  }else{
                        position = 3;
                  }
            }
            
            switch (position)
            {
            case 1:
                  if(layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]+WIDTH-1] == 0 && 
                  layout[_tile->prop[1]+1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[1]-1;
                        _tile->prop[3] = _tile->prop[2]+WIDTH;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]-WIDTH-1] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]-WIDTH] == 0){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[1]-WIDTH;
                        _tile->prop[3] = _tile->prop[2]-1;
                  }
                  break;
            case 3:
                  if(layout[_tile->prop[1]+1] == 0 && layout[_tile->prop[1]-1] == 0 &&
                  layout[_tile->prop[1]-WIDTH+1] == 0 && _tile->prop[1]%WIDTH < WIDTH-1){
                        _tile->prop[0] = _tile->prop[1]-1;
                        _tile->prop[2] = _tile->prop[1]+1;
                        _tile->prop[3] = _tile->prop[2]-WIDTH;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]-WIDTH] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0 && _tile->prop[1]/WIDTH < HEIGHT-1){
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                        _tile->prop[2] = _tile->prop[1]+WIDTH;
                        _tile->prop[3] = _tile->prop[2]+1;
                  }            
                  break;
            default:
                  break;
            }
            break;
      case JSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 4;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 2;
                  }else{
                        position = 3;
                  }
            }
            switch (position)
            {
            case 1:
                  if(layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]-WIDTH-1] == 0 && 
                  layout[_tile->prop[1]+1] == 0 && _tile->prop[1]%WIDTH < WIDTH-1){
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[3] = _tile->prop[1]-1;
                        _tile->prop[2] = _tile->prop[3]-WIDTH;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]-WIDTH+1] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]-WIDTH] == 0 && _tile->prop[1]/WIDTH < HEIGHT-1){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[3] = _tile->prop[1]-WIDTH;
                        _tile->prop[2] = _tile->prop[3]+1;
                  }
                  break;
            case 3:
                  if(layout[_tile->prop[1]+1] == 0 && layout[_tile->prop[1]-1] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[0] = _tile->prop[1]-1;
                        _tile->prop[3] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[3]+WIDTH;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]-WIDTH] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0){
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                        _tile->prop[3] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[3]-1;
                  }            
                  break;
            default:
                  break;
            }            
            break;
      case TSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[1] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 2;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[3] < _tile->prop[1]){
                        position = 3;
                  }else{
                        position = 4;
                  }
            }
            switch (position)
            {
            case 1:
                  if(layout[_tile->prop[1]-WIDTH] == 0){
                        _tile->prop[2] = _tile->prop[3];
                        _tile->prop[3] = _tile->prop[0];
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]+1]==0 && _tile->prop[1]%WIDTH < 15){
                        _tile->prop[3] = _tile->prop[1]-WIDTH;
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[1]-1;
                  }
                  break;
            case 3:
                  if(layout[_tile->prop[1]+WIDTH] == 0 && _tile->prop[1]/WIDTH < 23){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[3];
                        _tile->prop[3] = _tile->prop[1]+1;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]-1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[3] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[1]+1;
                        _tile->prop[0] = _tile->prop[1]-1;
                  }
                  break;
            
            default:
                  break;
            }
            break;
      case ISHAPE:
            if(_tile->prop[3]-_tile->prop[2] == 1){
                  position = 1;
            }else{
                  position = 2;
            }
            if(position == 1){
                  if(_tile->prop[2]/WIDTH<22 && layout[_tile->prop[2]+WIDTH] == 0&&
                  layout[_tile->prop[2]+(2*WIDTH)] == 0 && layout[_tile->prop[2]-WIDTH]==0){
                        _tile->prop[0]=_tile->prop[2]-WIDTH;
                        _tile->prop[1]=_tile->prop[2];
                        _tile->prop[2]+=WIDTH;
                        _tile->prop[3]=_tile->prop[2]+WIDTH;
                  }
            }else{
                  if(_tile->prop[1]%WIDTH>1 && _tile->prop[1]%WIDTH<15 &&
                  layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]-2] == 0 &&
                  layout[_tile->prop[1]+1] == 0){
                        _tile->prop[2]=_tile->prop[1];
                        _tile->prop[1]=_tile->prop[2]-1;
                        _tile->prop[0]=_tile->prop[1]-1;
                        _tile->prop[3]=_tile->prop[2]+1;
                  }
            }
            break;
      
      default:
            break;
      }
}

void rRotate(tile* _tile){
      int position;
      switch (_tile->shape)
      {
      case SSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  position = 1;
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  position = 2;
            }
            if(position == 1){
                  if(layout[_tile->prop[3]+1] == 0 && layout[_tile->prop[3]+WIDTH+1] == 0 && _tile->prop[3]/WIDTH < HEIGHT-1){
                        _tile->prop[2]=_tile->prop[3]+1;
                        _tile->prop[1]=_tile->prop[0];
                        _tile->prop[0]=_tile->prop[3];
                        _tile->prop[3]+=WIDTH+1;
                  }
            }
            if(position == 2){
                  if(layout[_tile->prop[0]-1] == 0 && layout[_tile->prop[1]+1] == 0 && _tile->prop[0]%WIDTH > 0){
                        _tile->prop[3]=_tile->prop[0];
                        _tile->prop[2]=_tile->prop[3]-1;
                        _tile->prop[0]=_tile->prop[1];
                        _tile->prop[1]=_tile->prop[0]+1;
                  }
            }
            break;
      case ZSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  position = 1;
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  position = 2;
            }
            if(position==1){
                  if(layout[_tile->prop[2]+WIDTH-1] == 0 && _tile->prop[3]/WIDTH < HEIGHT-1){
                        _tile->prop[0] += WIDTH;
                        _tile->prop[3] = _tile->prop[0]+WIDTH;
                  }
            }
            if(position==2){
                  if(layout[_tile->prop[2]+1] == 0 && _tile->prop[2]%WIDTH < WIDTH-1){
                        _tile->prop[0] -= WIDTH;
                        _tile->prop[3] = _tile->prop[2]+1;
                  }
            }
            break;
      case LSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 4;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 2;
                  }else{
                        position = 3;
                  }
            }
            
            switch (position)
            {
            case 3:
                  if(layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]+WIDTH-1] == 0 && 
                  layout[_tile->prop[1]+1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[1]-1;
                        _tile->prop[3] = _tile->prop[2]+WIDTH;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]-WIDTH-1] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]-WIDTH] == 0){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[1]-WIDTH;
                        _tile->prop[3] = _tile->prop[2]-1;
                  }
                  break;
            case 1:
                  if(layout[_tile->prop[1]+1] == 0 && layout[_tile->prop[1]-1] == 0 &&
                  layout[_tile->prop[1]-WIDTH+1] == 0 && _tile->prop[1]%WIDTH < WIDTH-1){
                        _tile->prop[0] = _tile->prop[1]-1;
                        _tile->prop[2] = _tile->prop[1]+1;
                        _tile->prop[3] = _tile->prop[2]-WIDTH;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]-WIDTH] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0 && _tile->prop[1]/WIDTH < HEIGHT-1){
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                        _tile->prop[2] = _tile->prop[1]+WIDTH;
                        _tile->prop[3] = _tile->prop[2]+1;
                  }            
                  break;
            default:
                  break;
            }
            break;
      case JSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 4;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[2] < _tile->prop[3]){
                        position = 2;
                  }else{
                        position = 3;
                  }
            }
            switch (position)
            {
            case 3:
                  if(layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]-WIDTH-1] == 0 && 
                  layout[_tile->prop[1]+1] == 0 && _tile->prop[1]%WIDTH < WIDTH-1){
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[3] = _tile->prop[1]-1;
                        _tile->prop[2] = _tile->prop[3]-WIDTH;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]-WIDTH+1] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]-WIDTH] == 0 && _tile->prop[1]/WIDTH < HEIGHT-1){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[3] = _tile->prop[1]-WIDTH;
                        _tile->prop[2] = _tile->prop[3]+1;
                  }
                  break;
            case 1:
                  if(layout[_tile->prop[1]+1] == 0 && layout[_tile->prop[1]-1] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[0] = _tile->prop[1]-1;
                        _tile->prop[3] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[3]+WIDTH;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]-WIDTH] == 0 && layout[_tile->prop[1]+WIDTH] == 0 &&
                  layout[_tile->prop[1]+WIDTH+1] == 0){
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                        _tile->prop[3] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[3]-1;
                  }            
                  break;
            default:
                  break;
            }            
            break;
      case TSHAPE:
            if(_tile->prop[0]<_tile->prop[1]){
                  if(_tile->prop[1] < _tile->prop[3]){
                        position = 1;
                  }else{
                        position = 2;
                  }
            }
            if(_tile->prop[1]<_tile->prop[0]){
                  if(_tile->prop[3] < _tile->prop[1]){
                        position = 3;
                  }else{
                        position = 4;
                  }
            }
            switch (position)
            {
            case 3:
                  if(layout[_tile->prop[1]-WIDTH] == 0){
                        _tile->prop[2] = _tile->prop[3];
                        _tile->prop[3] = _tile->prop[0];
                        _tile->prop[0] = _tile->prop[1]-WIDTH;
                  }
                  break;
            case 4:
                  if(layout[_tile->prop[1]+1]==0 && _tile->prop[1]%WIDTH < 15){
                        _tile->prop[3] = _tile->prop[1]-WIDTH;
                        _tile->prop[0] = _tile->prop[1]+1;
                        _tile->prop[2] = _tile->prop[1]-1;
                  }
                  break;
            case 1:
                  if(layout[_tile->prop[1]+WIDTH] == 0 && _tile->prop[1]/WIDTH < 23){
                        _tile->prop[0] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[3];
                        _tile->prop[3] = _tile->prop[1]+1;
                  }
                  break;
            case 2:
                  if(layout[_tile->prop[1]-1] == 0 && _tile->prop[1]%WIDTH > 0){
                        _tile->prop[3] = _tile->prop[1]+WIDTH;
                        _tile->prop[2] = _tile->prop[1]+1;
                        _tile->prop[0] = _tile->prop[1]-1;
                  }
                  break;
            
            default:
                  break;
            }
            break;
      case ISHAPE:
            if(_tile->prop[3]-_tile->prop[2] == 1){
                  position = 1;
            }else{
                  position = 2;
            }
            if(position == 1){
                  if(_tile->prop[2]/WIDTH<22 && layout[_tile->prop[2]+WIDTH] == 0&&
                  layout[_tile->prop[2]+(2*WIDTH)] == 0 && layout[_tile->prop[2]-WIDTH]==0){
                        _tile->prop[0]=_tile->prop[2]-WIDTH;
                        _tile->prop[1]=_tile->prop[2];
                        _tile->prop[2]+=WIDTH;
                        _tile->prop[3]=_tile->prop[2]+WIDTH;
                  }
            }else{
                  if(_tile->prop[1]%WIDTH>1 && _tile->prop[1]%WIDTH<15 &&
                  layout[_tile->prop[1]-1] == 0 && layout[_tile->prop[1]-2] == 0 &&
                  layout[_tile->prop[1]+1] == 0){
                        _tile->prop[2]=_tile->prop[1];
                        _tile->prop[1]=_tile->prop[2]-1;
                        _tile->prop[0]=_tile->prop[1]-1;
                        _tile->prop[3]=_tile->prop[2]+1;
                  }
            }
            break;
      
      default:
            break;
      }
}

int eraseLine(){
      int erased = 0;
      for(int i = 0; i < HEIGHT; i++){
            bool line = true;
            for(int j = 0; j < WIDTH; j++){
                  if(layout[i*WIDTH+j] == 0){line = false;break;}
            }
            if(line){
                  erased++;
                  for(int j = 0; j < WIDTH; j++){
                        layout[i*WIDTH+j]=0;
                  }
            }
      }
      return erased;
}

bool falling(tile* _tile, int stackDel){
      bool stack = false;
      for(int i = 0; i < 4; i++){
            if(layout[_tile->prop[i]+WIDTH]==1){
                  stack = true;
                  // SDL_Log("stack");
                  break;
            }
            if(_tile->prop[i]/WIDTH == HEIGHT-1){
                  stack = true;
            }
      }

      if(stack){
            if(stackDel>2){
                  for(int i = 0; i < 4; i++){
                        layout[_tile->prop[i]]=1;
                  }
            }
      }else{
            for(int i = 0; i < 4; i++){
                  _tile->prop[i]+=WIDTH;
            }
      }

      return !stack;
}

void layoutFall(){
      for(int i = 0; i < 24; i++){
            bool line = true;
            for(int j = 0; j < WIDTH; j++){
                  if(layout[i*WIDTH+j] == 1){line = false;break;}
            }
            if(line){
                  for(int j = i; j >= 0; j--){
                        for(int k = 0; k < WIDTH; k++){
                              if(j>0){
                                    layout[j*WIDTH+k]=layout[(j-1)*WIDTH+k];
                              }else{
                                    layout[j*WIDTH+k] = 0;
                              }
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

bool focus(SDL_Window* window, SDL_Event *e, bool recent){
      if(SDL_GetMouseFocus() == window){
            if(!recent){
                  if(e->type == SDL_MOUSEBUTTONDOWN){
                        return true;
                  }else{
                        return false;
                  }
            }else{
                  return false;
            }
      }else{
            if(!recent){
                  return false;
            }else{
                  if(e->type == SDL_MOUSEBUTTONDOWN){
                        return false;
                  }else{
                        return true;
                  }
            }
      }
}