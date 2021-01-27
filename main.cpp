#define SDL_MAIN_HANDLED
#include "NewImp/window/window.h"
#define FPS 70.0F

int main(int argc, char** argv) {	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	Bool_t isOpen = TRUE;
	Surface_t* surface = Surface_ctor("poop.png");

	ResourceManager_add(StringInt_ctor("poop"), surface);
	

	//SceneManager_init(LoadingScene_ctor);
	mainWindow = Window_ctor(50, 50, "test window", 0);
	Window_refreshSurface(mainWindow, surface);


	float SPF = (1000.0F * 1.0F) / FPS; // time for 1 frame
	unsigned int ticks = SDL_GetTicks();
	unsigned int elapsedTime = 0;
	unsigned char FCF_FPS = 5;
	unsigned char FPS_CNT = 0;
	unsigned int FPS_NUM = 0;
	Point2D_t p = Point2D_ctor(0, 0);

	char* strfps = (char*) malloc(5);
	strfps[0] = 0;

	while(isOpen) {
		while(Window_pollEvent(mainWindow)) {
			switch(Window_getEventType(mainWindow)) {
				case SDL_QUIT:
					isOpen = FALSE;
					break;
			}
		}

		// if elapsed time is greater or equal to the SFP, render; else, wait the remaining time.
		elapsedTime = SDL_GetTicks() - ticks;

		if(elapsedTime >= SPF) {
			ticks = SDL_GetTicks();

			Window_refresh(mainWindow);
			// Render begins here:

			//SceneManager_renderScene();
			//SceneManager_verifyScenes();
			Window_renderFullTexture(mainWindow, p, surface->lock);

			// Render ends here.
			Window_present(mainWindow);

			if(FPS_CNT == FCF_FPS) {
				FPS_CNT = 0;
				FPS_NUM = (unsigned int) (1000.0F / ((float) elapsedTime));
				strfps = itoa(FPS_NUM, strfps, 10);
				Window_setTitle(mainWindow, strfps);
			}
			else 
				FPS_CNT++; 
		}
		else 
			SDL_Delay(SPF - elapsedTime);
	}
	return 0;
}