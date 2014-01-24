
#include "FrontendView.h"
 
// Global static pointer used to ensure a single instance of the class.
KBOT::FrontendView* KBOT::FrontendView::m_pInstance = nullptr;

/** This function is called to create an instance of the class.
Calling the constructor publicly is not allowed. The constructor
is private and is only called by this Instance function.

SINGLENTON DE LA PANTALLA
*/

void set_screen(int w, int h, int fw = 0, int fh = 0) {
    SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, fw, 0, fh);
    glClearColor(0.0, 0.0 ,0.0, 0);
}

KBOT::FrontendView::FrontendView()
{
	SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    SDL_WM_SetCaption("bots", "bots");

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    set_screen(500, 500);

}


/**
 * No pinta bien la pantalla si las dimensiones no son cuadradas!
 * (El tamaño no importa mientras sea proporcional ;P xD)
 */
void KBOT::FrontendView::draw(bots & bots, boost::mutex & state_mutex,int & sx,int & sy)
{
	while (!gameover){
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					std::cout << "QUIT SDL" << std::endl;
					gameover = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							std::cout << "QUIT KEY PRESSED" << std::endl;
							gameover = true;
							break;
					}
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		{
			// segfaults if it draws during a state update (drawing +
			// incomplete state is fatal)
			boost::mutex::scoped_lock lock(state_mutex);
			if (_sx!=sx || _sy!=sy) {
				_sx=sx;
				_sy=sy;
				set_screen(500,500,sx,sy);
			}
			bots.for_each_bot([&bots] (const bot & the_bot) {
				auto t = the_bot.get_team() + 1;

				glColor3f(t * 0.2, t * 0.1, t * 0.1);
				const bot::position & pos = the_bot.get_position();

				// WARNING deprecated OpenGL!
				glLoadIdentity();
				glTranslatef(pos.first, pos.second, 0);

				glBegin(GL_QUADS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, 0.0f);
				glVertex3f(1.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, 1.0f, 0.0f);
				glEnd();
			});
		}

		SDL_GL_SwapBuffers();
	}
}

KBOT::FrontendView* KBOT::FrontendView::getInstance()
{
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new FrontendView;

   return m_pInstance;
}
