#include <iostream>
#include <string>
#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER);
	TTF_Init();

	string message, fontname;
	bool loop = true;

	//Your daily dose of command line args
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--msg") == 0)
		{
			if (argv[i+1])
				message = argv[i+1];
		}
		else if (strcmp(argv[i], "--font") == 0)
		{
			if (argv[i+1])
				fontname = argv[i+1];
		}
	}

	if (message.empty())
		message = "YOU DIED";

	if (fontname.empty())
	{
		fontname = "OptimusPrinceps.ttf";
	}

	//Frame counter subsystem
	uint64_t currentframe = 0;
	//END Frame counter subsystem

	//Get display info
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	int windowHeight = DM.h/5;

	//Creating a window to draw on
	SDL_Window* window = SDL_CreateWindow(message.c_str(), SDL_WINDOWPOS_UNDEFINED, DM.h/2 - windowHeight/2, DM.w, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	SDL_SetWindowOpacity(window, 0.0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Event event;

	//Text stuff
	SDL_Color red = {180, 0, 0};
	SDL_Color black = {0, 0, 0};
		//Opening font
		TTF_Font* font = TTF_OpenFont(fontname.c_str(), 256);
		if (!font)
		{
			cout << "Error: font file " << fontname << " not found ! Exiting." << endl;
			exit(1);
		}

	//Create text texture
	SDL_Surface* textbuf = TTF_RenderText_Shaded(font, message.c_str(), red, black);
	SDL_Surface* srfbuf = SDL_CreateRGBSurface(0, textbuf->w, textbuf->w, 32, 0, 0, 0, 255);	//Create square texture to solve aspect ratio scaling issues
	SDL_FillRect(srfbuf, NULL, SDL_MapRGB(srfbuf->format, 0, 0, 0));
	SDL_Rect tmprect = {0, srfbuf->h/2 - textbuf->h/2, srfbuf->w, srfbuf->h};
	SDL_BlitSurface(textbuf, nullptr, srfbuf, &tmprect);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, srfbuf);

	//Set text to the right size
	SDL_Rect recttext = {0, 0, 0, 0};
	SDL_QueryTexture(text, nullptr, nullptr, &recttext.w, &recttext.h);
	recttext.w = recttext.h = windowHeight * 3;	//Completely arbitrary number that happens to work well enough
	recttext.x = DM.w/2 - recttext.w/2;
	recttext.y = windowHeight/2 - recttext.h/2;

	//We no longer need those
	SDL_FreeSurface(textbuf);
	SDL_FreeSurface(srfbuf);

	//Setting text transparency
	unsigned int text_alpha = 0;
	SDL_SetTextureAlphaMod(text, text_alpha);

	//Setting background transparency
	float bar_alpha = 0.0;

	while (loop)
	{
	    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    SDL_RenderFillRect(renderer, nullptr);

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
	        bar_alpha += 0.016; //0.017 ~= (10/60)/10, so we do one step lower to get the transparency effect closer to what it looks like in DS
	        SDL_SetWindowOpacity(window, bar_alpha);
	    }

	    if (currentframe > 20)
	    {
			recttext.w = (recttext.w + 1) * (1 + recttext.w / 1000);
			recttext.h = (recttext.h + 1) * (1 + recttext.h / 1000);
	        recttext.x = DM.w/2 - recttext.w/2;
	        recttext.y = windowHeight/2 - recttext.h/2;
	    }

		if ((currentframe > 30) && (currentframe < 90))
	    {
	        text_alpha += 255 / 60;
	        SDL_SetTextureAlphaMod(text, text_alpha);
	    }

	    if ((currentframe > 240) && (currentframe < 300))
	    {
	        text_alpha -= 250 / 60;
	        SDL_SetTextureAlphaMod(text, text_alpha);
	        bar_alpha -= 0.016;
	        SDL_SetWindowOpacity(window, bar_alpha);
	    }

	    SDL_RenderCopy(renderer, text, nullptr, &recttext);

	    if (currentframe == 360)
	        loop = false;
	    else
	        currentframe++;

	    SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(text);
	SDL_Quit();

	return 0;
}
