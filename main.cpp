#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER);
    TTF_Init();

    const char* message = NULL;

    if (argc == 2)
    {
        message = argv[1];
    }
    else
    {
        message = "You Died";
    }

    //Frame counter subsystem
    uint64_t currentframe = 0;
    //END Frame counter subsystem

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int Width = DM.w;
    int Height = DM.h/5;

    SDL_Window* window = SDL_CreateWindow("YOU DIED", SDL_WINDOWPOS_UNDEFINED, DM.h/2 - Height/2, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    SDL_SetWindowOpacity(window, 0.0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool loop = true;
    SDL_Event event;

    //Text thing
    SDL_Color red = {180, 0, 0};

    TTF_Font* font = TTF_OpenFont("OptimusPrinceps.ttf", 128);

    SDL_Surface* srfbuf = TTF_RenderText_Solid(font, message, red);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, srfbuf);
    SDL_FreeSurface(srfbuf);
    SDL_Rect recttext = {0, 0, 0, 0};
    SDL_QueryTexture(text, NULL, NULL, &recttext.w, &recttext.h);
    recttext.x = Width/2 - recttext.w/2;
    recttext.y = Height/2 - recttext.h/2;

    unsigned int text_alpha = 0;
    SDL_SetTextureAlphaMod(text, text_alpha);

    //Black bar thing
    float bar_alpha = 0.0;

    while (loop)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, NULL);

        //Input handling
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                loop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    loop = false;
                }
            }
        }

        //Main section
        if (currentframe < 60)
        {
            bar_alpha += 0.017; //0.017 ~= (10/60)/10
            SDL_SetWindowOpacity(window, bar_alpha);
        }

        if ((currentframe > 60) && (currentframe < 120))
        {
            text_alpha += 255 / 60;
            SDL_SetTextureAlphaMod(text, text_alpha);
        }

        if ((currentframe > 60) && (currentframe % 2 == 0))
        {
            recttext.w = (recttext.w + 1) * 1.004;
            recttext.h = (recttext.h + 1) * 1.001;
            recttext.x = Width/2 - recttext.w/2;
            recttext.y = Height/2 - recttext.h/2;
        }

        if ((currentframe > 240) && (currentframe < 300))
        {
            text_alpha -= 255 / 60;
            SDL_SetTextureAlphaMod(text, text_alpha);
            bar_alpha -= 0.017;
            SDL_SetWindowOpacity(window, bar_alpha);
        }

        SDL_RenderCopy(renderer, text, NULL, &recttext);

        if (currentframe == 360)
            loop = false;
        else
            currentframe++;

        SDL_RenderPresent(renderer);
    }
    return 0;
}
