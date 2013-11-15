#include "bots.h"
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

int main()
{

    const int width = 100;
    const int height = 100;


    bots bots(width, height);

    bots.generate(2, 5);

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Bots", "Bots");

    const int win_width = 500;
    const int win_height = 500;

    /* create window */
    SDL_Surface *screen =
	SDL_SetVideoMode(win_width, win_height, 0, SDL_OPENGL);

    SDL_Event event;
    bool gameover = false;

    Uint32 previous_time = SDL_GetTicks();

    std::function < void (void) > team_color[] = {
	[]() {
	    glColor3f(1.0f, 0.0f, 0.0);
	},[]() {
	    glColor3f(0.0f, 1.0f, 0.0);
	},[]() {
	    glColor3f(0.0f, 0.0f, 1.0);
    },};


    glViewport(0, 0, win_width, win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    /* message pump */
    while (!gameover) {

	Uint32 current_time = SDL_GetTicks();

	bots.step(current_time - previous_time);

	previous_time = current_time;

	/* look for an event */
	if (SDL_PollEvent(&event)) {
	    /* an event was found */
	    switch (event.type) {
		/* close button clicked */
	    case SDL_QUIT:
		gameover = true;
		break;
		/* handle the keyboard */
	    case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
		case SDLK_q:
		    gameover = true;
		    break;
		}
		break;
	    }
	}





	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//bots.for_each_bot([&team_color] (const bot::position & pos, std::shared_ptr<bot> const the_bot) {
	bots.for_each_bot([&team_color, &bots] (bot & the_bot) {
			  bots.move(the_bot, S);
			  team_color[the_bot.get_team()]();
			  glPushMatrix();
			  const bot::position & pos =
			  the_bot.get_position();
			  glTranslatef(pos.first, pos.second, 0);
			  glBegin(GL_QUADS); glVertex3f(0.0f, 0.0f, 0.0f);
			  glVertex3f(1.0f, 0.0f, 0.0f);
			  glVertex3f(1.0f, 1.0f, 0.0f);
			  glVertex3f(0.0f, 1.0f, 0.0f); glEnd();
			  glPopMatrix();});

	SDL_GL_SwapBuffers();
    }

    /* cleanup SDL */
    SDL_Quit();


    return 0;
}
