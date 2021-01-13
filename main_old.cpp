
#define SDL_MAIN_HANDLED
#include "Headers/engine/scenes/loading.h"
#define FPS 70.0F

int main(int argc, char** argv) {	
	Bool_t isOpen = TRUE;
	Surface_t* surface = Surface_ctor("poop.png");
	SceneManager_init(LoadingScene_ctor);



	float SPF = (1000.0F * 1.0F) / FPS; // time for 1 frame
	unsigned int ticks = SDL_GetTicks();
	unsigned int elapsedTime = 0;
	unsigned char FCF_FPS = 5;
	unsigned char FPS_CNT = 0;
	unsigned int FPS_NUM = 0;

	char* strfps = (char*) malloc(sizeof(char) * 5);
	strfps[0] = 0;

	while(isOpen) {
		while(Renderer_pollEvent(renderer)) {
			switch(renderer->event.type) {
				case SDL_QUIT:
					isOpen = FALSE;
					break;
			}
		}

		// if elapsed time is greater or equal to the SFP, render; else, wait the remaining time.
		elapsedTime = SDL_GetTicks() - ticks;

		if(elapsedTime >= SPF) {
			ticks = SDL_GetTicks();

			Renderer_clear(renderer);
			// Render begins here:

			SceneManager_renderScene();
			SceneManager_verifyScenes();

			// Render ends here.
			Renderer_present(renderer);

			if(FPS_CNT == FCF_FPS) {
				FPS_CNT = 0;
				FPS_NUM = (unsigned int) (1000.0F / ((float) elapsedTime));
				strfps = itoa(FPS_NUM, strfps, 10);
				Renderer_setWindowTitle(renderer, strfps);
			}
			else 
				FPS_CNT++; 
		}
		else 
			SDL_Delay(SPF - elapsedTime);
	}
	return 0;
}