#include<iostream>
#include<SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include<math.h>
#include<vector>
// #include<sd

SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
int height = 720;
int width = 1280;
int pach=height/15,pacw=height/15;
int spc = 45;
int speed = 2;
SDL_Texture* BIRD = NULL;
SDL_Texture* BIRD_SPRITE = NULL;
SDL_Texture* OBSTICLE = NULL;
SDL_Texture* REVOBSTICLE = NULL;
SDL_Texture* BACKGROUND = NULL;
TTF_Font* font = NULL;
SDL_Texture* PAUS = NULL;
SDL_Texture* STRT = NULL;
SDL_Texture* GOV = NULL;
SDL_Rect pausrect ;
SDL_Texture* SCH = NULL;
SDL_Texture* val = NULL;

// SDL_Rect pausrect = {300,200,500,700};


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

    if ( TTF_Init() < 0 ) {
	    std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
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


SDL_Texture* textTexture(const std::string& text,SDL_Color color = {0,0,0}) {
    SDL_Surface* textsurf = TTF_RenderText_Solid( font, text.c_str(), color );
    if ( !textsurf ) {
        std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
    }

    SDL_Texture* texTure = SDL_CreateTextureFromSurface( rend, textsurf );

    return texTure;
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
    BACKGROUND = loadTexture("../assets/flapbg.png");
    if (!BACKGROUND) {
        std::cout << "Failed to load BACKGROUND texture!" << std::endl;
        return false;
    }
    font = TTF_OpenFont("../fonts/sifi_font.ttf", 72);
	if ( !font ) {
		std::cout << "Error loading font: " << TTF_GetError() << std::endl;
		return false;
	}
    PAUS = textTexture("PAUSED",{155,120,50});
    if( !PAUS){
        std::cout << "Error loading text " << TTF_GetError() << std::endl;
		return false;
    }
    pausrect = {static_cast<int>(round(width/2-250)), static_cast<int>(round(height/2-100)), 500, 200};
    STRT = textTexture("START",{255,20,50});
    if( !STRT){
        std::cout << "Error loading text " << TTF_GetError() << std::endl;
        return false;
    }
    GOV = textTexture("GAME OVER",{255,20,50}); 
    if( !GOV){
        std::cout << "Error loading text " << TTF_GetError() << std::endl;
        return false;
    }
    SCH = textTexture("SCORE: ",{20,200,250});
    if( !SCH){
        std::cout << "Error loading text " << TTF_GetError() << std::endl;
        return false;
    }
    val = textTexture("0",{20,200,250});
    if( !val){
        std::cout << "Error loading text " << TTF_GetError() << std::endl;
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

void kill(){

    if (BIRD != NULL) 
        SDL_DestroyTexture(BIRD);
    if (OBSTICLE != NULL) 
        SDL_DestroyTexture(OBSTICLE);
    if (REVOBSTICLE != NULL) 
        SDL_DestroyTexture(REVOBSTICLE);
    if (BIRD_SPRITE != NULL)
        SDL_DestroyTexture(BIRD_SPRITE);
    if (BACKGROUND != NULL)
        SDL_DestroyTexture(BACKGROUND);
    if (font != NULL)
        TTF_CloseFont(font);
    if (PAUS != NULL)
        SDL_DestroyTexture(PAUS);
        



    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
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


    SDL_Rect pacdim = {static_cast<int>(round(width/6)), static_cast<int>(round(height/4)), pacw, pach};
    SDL_Rect assinbrd = {20,30,155,138};//EXACT DIMENSIONS OF THE BIRD

    SDL_Rect strtrect = {static_cast<int>(round(width/2-250)), static_cast<int>(round(height/2-100)), 500, 200};
    SDL_Rect govrect = {static_cast<int>(round(width/2-250)), static_cast<int>(round(height/2-100)), 500, 200};
    SDL_Rect logorect = {10,10,40,40};
    SDL_Rect scorrect = {80,10,200,40};
    SDL_Rect valrect = {310,5,50,50};


    int frame = 0,duration = 0,score = 0;
    bool running = true,paused = true,started = false,collided = false,resartable = false;
    uint spat = 0,piptim=0;
    std::vector<std::vector<int>> pipes;
    std::vector<int> front;

    std::vector<SDL_Rect> bg = {{0,0,width,height},{width,0,width,height}};
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND); // Use blend mode for alpha transparency


    while(running)    {
        Uint32 startTick = SDL_GetTicks();
        SDL_Event ev;
        while(SDL_PollEvent(&ev))    {
            switch(ev.type){
            case SDL_QUIT: running = false;
                break;
            case SDL_MOUSEBUTTONDOWN : 
            if(!started)
                started = true;
            if(paused && !collided)
                paused = false;
            else if(spat <= (spc/4*3))
                spat=spc;

            break;
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym == SDLK_SPACE){
                    if(!started)
                        started = true;
                    if(collided){
                        if(resartable){
                            pacdim.y = height/4;
                            pipes.clear();
                            spat = 0;
                            piptim = 0;
                            collided = false;
                            score = 0;
                            val = textTexture("0",{255,200,50});
                            started = false;
                        }
                    }
                    else if(paused)
                        paused = false;
                    else if(spat <= (spc/4*3))
                        spat=spc;
                }
                if(ev.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
                if(ev.key.keysym.sym == SDLK_RETURN){
                    if(collided){
                        pacdim.y = height/4;
                        pipes.clear();
                        spat = 0;
                        piptim = 0;
                        collided = false;
                        score = 0;
                        val = textTexture("0",{255,200,50});
                        started = false;
                    }
                    else    {
                        paused = !paused;
                        if(!started)
                            started = true;
                    }
                }
                break;
                case SDL_KEYUP:
                if(ev.key.keysym.sym == SDLK_SPACE && collided)
                    resartable = true;
                break;
            }
        }
        for(int i=0;i<bg.size();i++){
            if(!paused)
                bg[i].x-=1;
            if(bg[i].x<=-width)
                bg[i].x = width;
            SDL_RenderCopy(rend, BACKGROUND, NULL, &bg[i]);
        }
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 100);
        SDL_Rect overlay = {0, 0, width, height};
        SDL_RenderFillRect(rend, &overlay);
        // SDL_SetRenderDrawColor(rend,60,170,255,255);
        // SDL_RenderClear(rend);
        if(!paused){

            const uint8_t* press = SDL_GetKeyboardState(NULL);
            if(spat <= (spc/4*3) && press[SDL_SCANCODE_KP_0]){
                spat = 40;
            }
        

            if(spat){
                spat--;
                pacdim.y-=spat/7*2;
            }

            if(pacdim.y >= (height-(pach/4)))
                running = false;
            
            if(piptim==150){
                // pipes.push_back(pipe_gen());
                pipes.insert(pipes.begin(),pipe_gen());
                piptim = 0;
            }
            SDL_Rect rrr;
            SDL_SetRenderDrawColor(rend,0,200,20,255);
            for(int i=0;i<pipes.size();i++){
                pipes[i][0]-=speed;
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

                // std::cout<<pipes.size()<<" - "<<pipes.size()-3<<'\n';
                if(i>pipes.size()-3){
                    if(((pacdim.x+pacdim.w)>=rrr.x) && ((pacdim.x)<=(rrr.x+rrr.w))){
                        // std::cout<<"<0>";
                        if(((rrr.x+rrr.w)<=pacdim.x+speed)){
                            val = textTexture(std::to_string(++score),{20,200,250});
                            SDL_QueryTexture(val,NULL,NULL,&valrect.w,&valrect.h);
                            valrect.w = (valrect.w*50)/valrect.h;
                            valrect.h = 50;
                            // valrect.x = val->w;
                        }

                        SDL_SetRenderDrawColor(rend,200,200,220,255);
                        SDL_RenderDrawLine(rend,220,0,220,height);
                        SDL_RenderDrawLine(rend,270,0,270,height);

                        if(!((pacdim.y>pipes[i][1]) && ((pacdim.y+pacdim.h)<=rrr.y))){
                            SDL_SetRenderDrawColor(rend,250,0,0,255);
                            SDL_RenderDrawLine(rend,0,pacdim.y,width,pacdim.y);
                            SDL_RenderDrawLine(rend,0,pacdim.y+50,width,pacdim.y+50);
                            collided = true;
                            paused = true;
                            resartable = false;
                        }
                        SDL_SetRenderDrawColor(rend,0,200,20,255);
                    }
                }
            }

            if(!pipes.empty() && pipes[pipes.size()-1][0]< -100)
                pipes.pop_back();

            piptim++;
            pacdim.y+= 6;

        }
        else{
            if(!started){
                SDL_SetRenderDrawColor(rend, 100, 100, 100, 100);
                SDL_Rect overlay = {0, 0, width, height};
                SDL_RenderFillRect(rend, &overlay);
                SDL_RenderCopy(rend, STRT, NULL, &strtrect);
            }
            else if(collided){
                SDL_SetRenderDrawColor(rend, 100, 0, 0, 150);
                SDL_Rect overlay = {0, 0, width, height};
                SDL_RenderFillRect(rend, &overlay);
                SDL_RenderCopy(rend, GOV, NULL, &govrect);
            }
            else{
                SDL_SetRenderDrawColor(rend, 0, 0, 0, 150);
                SDL_Rect overlay = {0, 0, width, height};
                SDL_RenderFillRect(rend, &overlay);
                SDL_RenderCopy(rend, PAUS, NULL, &pausrect);
            }
            // SDL_RenderCopy(rend, PAUS, NULL, NULL);

        }
        if(started){
            SDL_RenderCopy(rend, SCH, NULL, &scorrect);
            SDL_RenderCopy(rend, val, NULL, &valrect);
        }
        SDL_RenderCopy(rend, BIRD, NULL, &logorect);
        


        if(duration == 5){
            duration = 0;
            assinbrd.y = (frame/5)*168;
            assinbrd.x = (frame%5)*183;
            frame++;
            if(frame == 14)
                frame = 0;
            duration = 0;
        }
        duration++;

        SDL_RenderCopy(rend, BIRD_SPRITE, &assinbrd, &pacdim);
        // SDL_RenderCopy(rend, BIRD, NULL, &pacdim);
        SDL_RenderPresent( rend );
        Uint32 frameDuration = SDL_GetTicks() - startTick;
        // std::cout << "Frame Duration: " << frameDuration << "ms" << std::endl;
        if (frameDuration < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameDuration);
        }

    }
    kill();

    return 0;



}