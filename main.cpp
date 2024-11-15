#include<iostream>
#include<SDL2/SDL.h>
#include<math.h>
#include<vector>
// #include<sd

SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
int height = 720;
int width = 1280;
int pach=height/15,pacw=height/15;


bool init()    {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if(SDL_CreateWindowAndRenderer(width,height,0,&win,&rend) < 0)    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

std::vector<int> pipe_gen(){
    int gap = (rand()%50)+150;
    int pos = (rand()%(width-200-gap))+100;
    std::vector<int> res = {600,pos,gap};
    // std::vector<int> res = {width,pos,gap};
    return res;
}

int main(int argc, char* argv[])    {
    if(!init())    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    SDL_SetRenderDrawColor(rend,0,0,0,255);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    SDL_Rect pacdim;
    pacdim.y = round((height+pach)/6);
    pacdim.x = round((width+pacw)/6);
    pacdim.h = pach;
    pacdim.w = pacw;

    bool running = true;
    uint spat = 0,piptim=0;

    std::vector<std::vector<int>> pipes;

    while(running)    {
        Uint32 startTick = SDL_GetTicks();
        SDL_Event ev;
        while(SDL_PollEvent(&ev))    {
            switch(ev.type){
            case SDL_QUIT: running = false;
                break;
            case SDL_MOUSEBUTTONDOWN : 
            if(spat == 0)
                spat=60;    
            }
        }

        SDL_SetRenderDrawColor(rend,60,170,255,255);
        SDL_RenderClear(rend);

        const uint8_t* press = SDL_GetKeyboardState(NULL);
        if(spat == 0 && press[SDL_SCANCODE_KP_0]){
            spat = 60;
        }

        
        

        if(spat){
            spat--;
            pacdim.y-=ceil(spat/4);

        }

        if(pacdim.y >= (height-(pach/4)))
            running = false;
        
        if(piptim==250){
            pipes.push_back(pipe_gen());
            piptim = 0;
        }

        SDL_Rect rrr;
        SDL_SetRenderDrawColor(rend,0,200,20,255);
        for(std::vector<int> pip:pipes){
            pip[0]--;
            rrr.y = 0;
            rrr.x = pip[0];
            rrr.h = pip[1];
            rrr.w = 100;

            SDL_RenderFillRect(rend,&rrr);

            rrr.y+=(pip[1]+pip[2]);
            rrr.h=height-rrr.y;

            SDL_RenderFillRect(rend,&rrr);
        }


        SDL_SetRenderDrawColor(rend,0,0,20,255);
        SDL_RenderFillRect( rend, &pacdim );
        // SDL_RenderDrawRect(rend,&pacdim);



        piptim++;
        pacdim.y+= 4;
        std::cout<<piptim<<'\n';

        SDL_RenderPresent( rend );
        Uint32 frameDuration = SDL_GetTicks() - startTick;
        // std::cout << "Frame Duration: " << frameDuration << "ms" << std::endl;
        if (frameDuration < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameDuration);
        }
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;



}