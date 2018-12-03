#include "app/unix/unix_app_misc.h"
#include "app/unix/unix_app.h"

static bool gInitializedSDL = false;

void UnixAppMisc::init()
{
	// Init SDL
	initSDL();

	// Super call
	//GenericAppMisc::init();
}

GenericApp * UnixAppMisc::createApp()
{
	return UnixApp::createUnixApp();
}

bool UnixAppMisc::initSDL()
{
	// Don't init again
	if (!gInitializedSDL)
	{
		// Show mouse and don't warp in relative mode
		SDL_SetHint("SDL_HINT_MOUSE_RELATIVE_MODE_SHOW_CURSOR", "1");
		SDL_SetHint("SDL_HINT_MOUSE_RELATIVE_MODE_WARP", "0");

		// Init SDL context
		if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE) != 0)
		{
			/** @todo hanlde error */
			return false;
		}

		// Get version information
		SDL_version versionCompile; SDL_VERSION(&versionCompile);
		SDL_version versionRuntime; SDL_GetVersion(&versionRuntime);

		// Video driver information
		char const * videoDriver = SDL_GetCurrentVideoDriver();

		// Push SDL_TEXTINPUT events
		SDL_StartTextInput();

		// Initialized
		gInitializedSDL = true;
	}

	return true;
}