#include<iostream>
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<math.h>
#include<vector>
// #include<sd

SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
int height = 720;
int width = 1280;
int pach=height/15,pacw=height/15;
int spc = 45;
SDL_Texture* BIRD = NULL;
SDL_Texture* BIRD_SPRITE = NULL;
SDL_Texture* OBSTICLE = NULL;
SDL_Texture* REVOBSTICLE = NULL;
SDL_Texture* BACKGROUND = NULL;
// int spc = 50;


bool init()    {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if(SDL_CreateWindowAndRenderer(width,height,0,&win,&rend) < 0)    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Failed to initialize SDL_image! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }


    return true;
}

SDL_Texture* loadTexture(const std::string& path) {
    SDL_Texture* newTexture = IMG_LoadTexture(rend, path.c_str());
    if (newTexture == NULL) {
        std::cout << "Failed to load texture! IMG_Error: " << IMG_GetError() << std::endl;
    }
    return newTexture;
}

bool loader(){
    BIRD = loadTexture("../assets/flappybird.png");
    if (!BIRD) {
        std::cout << "Failed to load BIRD texture!" << std::endl;
        return false;
    }

    BIRD_SPRITE = loadTexture("../assets/animated_bird.png");
    if (!BIRD_SPRITE) {
        std::cout << "Failed to load BIRD_SPRITE texture!" << std::endl;
        return false;
    }

    OBSTICLE = loadTexture("../assets/obsticle.png");
    if (!OBSTICLE) {
        std::cout << "Failed to load OBSTICLE texture!" << std::endl;
        return false;
    }

    REVOBSTICLE = loadTexture("../assets/revobsticle.png");
    if (!REVOBSTICLE) {
        std::cout << "Failed to load REVOBSTICLE texture!" << std::endl;
        return false;
    }
    return true;
}

std::vector<int> pipe_gen(){
    int gap = (rand()%50)+150;
    int pos = (rand()%(height-100-gap))+50;
    // std::vector<int> res = {600,pos,gap};
    // std::cout<<pos<<' '<<gap<<'\n';
    std::vector<int> res = {width,pos,gap};
    return res;
}






int main(int argc, char* argv[])    {
    if(!init())    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    if(!loader())    {
        std::cout << "Failed to load!" << std::endl;
        return -1;
    }



    SDL_Rect pacdim = {static_cast<int>(round(width/6)), static_cast<int>(round(height/2)), pacw, pach};

    bool running = true,paused = true;
    uint spat = 0,piptim=0;
    std::vector<std::vector<int>> pipes;
    std::vector<int> front;

    while(running)    {
        Uint32 startTick = SDL_GetTicks();
        SDL_Event ev;
        while(SDL_PollEvent(&ev))    {
            switch(ev.type){
            case SDL_QUIT: running = false;
                break;
            case SDL_MOUSEBUTTONDOWN : 
            if(paused)
                paused = false;
            if(spat <= (spc/4*3))
                spat=spc;    
            break;
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym == SDLK_SPACE){
                    if(paused)
                        paused = false;
                    if(spat <= (spc/4*3))
                        spat=spc;
                }
                if(ev.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
                if(ev.key.keysym.sym == SDLK_RETURN){
                    paused = !paused;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(rend,60,170,255,255);
        SDL_RenderClear(rend);
        if(!paused){

            const uint8_t* press = SDL_GetKeyboardState(NULL);
            if(spat <= (spc/4*3) && press[SDL_SCANCODE_KP_0]){
                spat = 40;
            }
        

            if(spat){
                spat--;
                pacdim.y-=spat/7*2;
                // pacdim.y-=spat;

            }

            if(pacdim.y >= (height-(pach/4)))
                running = false;
            
            if(piptim==150){
                // pipes.push_back(pipe_gen());
                pipes.insert(pipes.begin(),pipe_gen());
                piptim = 0;
            }
            // std::cout<<pipes.size()<<" @ ";
            SDL_Rect rrr;
            SDL_SetRenderDrawColor(rend,0,200,20,255);
            for(int i=0;i<pipes.size();i++){
                pipes[i][0]-=2;
                rrr.y = 0;
                rrr.x = pipes[i][0];
                rrr.h = pipes[i][1];
                rrr.w = 100;

                SDL_RenderCopy(rend, REVOBSTICLE, NULL, &rrr);
                // SDL_RenderFillRect(rend,&rrr);

                rrr.y+=(pipes[i][1]+pipes[i][2]);
                rrr.h=height-rrr.y;

                // SDL_RenderFillRect(rend,&rrr);
                SDL_RenderCopy(rend, OBSTICLE, NULL, &rrr);


                if(i>pipes.size()-3){
                    if(((pacdim.x+pacdim.w)>=rrr.x) && ((pacdim.x)<=(rrr.x+rrr.w))){
                        if(press[SDL_SCANCODE_SPACE]){
                            std::cout<<"pressed"<<"\n";
                            std::cout<<pacdim.y<<' '<<pipes[i][1]<<' '<<pacdim.h<<' '<<rrr.y<<'\n';
                        }
                        SDL_SetRenderDrawColor(rend,200,200,220,255);
                        SDL_RenderDrawLine(rend,220,0,220,height);
                        SDL_RenderDrawLine(rend,270,0,270,height);

                        if(!((pacdim.y>pipes[i][1]) && ((pacdim.y+pacdim.h)<=rrr.y))){
                            SDL_SetRenderDrawColor(rend,250,0,0,255);
                            SDL_RenderDrawLine(rend,0,pacdim.y,width,pacdim.y);
                            SDL_RenderDrawLine(rend,0,pacdim.y+50,width,pacdim.y+50);
                            // std::cout<<"collided"<<"\n";
                            // running = false;
                        }
                        SDL_SetRenderDrawColor(rend,0,200,20,255);

                    }
                }
            }
            if(!pipes.empty() && pipes[pipes.size()-1][0]< -100)
                pipes.pop_back();



            piptim++;
            pacdim.y+= 6;
            // std::cout<<piptim<<'\n';

        }
        SDL_RenderCopy(rend, BIRD, NULL, &pacdim);
        SDL_RenderPresent( rend );
        Uint32 frameDuration = SDL_GetTicks() - startTick;
        // std::cout << "Frame Duration: " << frameDuration << "ms" << std::endl;
        if (frameDuration < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameDuration);
        }

    }

    if (BIRD != NULL) 
        SDL_DestroyTexture(BIRD);
    if (OBSTICLE != NULL) 
        SDL_DestroyTexture(OBSTICLE);
    if (REVOBSTICLE != NULL) 
        SDL_DestroyTexture(REVOBSTICLE);
    if (BIRD_SPRITE != NULL)
        SDL_DestroyTexture(BIRD_SPRITE);



    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    IMG_Quit();


    return 0;



}