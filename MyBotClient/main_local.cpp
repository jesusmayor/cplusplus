#include "bots.h"
#include "botclient.h"
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

void set_screen(int w, int h, int fw, int fh) {

    SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, fw, 0, fh);
    glClearColor(0.0, 0.0 ,0.0, 0);

}

int main()
{


    const int field_width = 2;
    const int field_height = 2;

    const int win_width = 500;
    const int win_height = 500;

    bots bots(field_width, field_height);

    bots.generate(2, 2);

    bot_client ia(bots);

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);
    /* Quit SDL properly on exit */
    atexit(SDL_Quit);

    /* set the title bar */
    SDL_WM_SetCaption("bots", "bots");

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* create window */
    set_screen(win_width, win_height, field_width, field_height);


    glShadeModel( GL_SMOOTH );


    SDL_Event event;
    bool gameover = false;

    Uint32 previous_time = SDL_GetTicks();

    int acc_time = 0;

    /* message pump */
    while (!gameover) {

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
                    // resizing
                case SDL_VIDEORESIZE:
                    set_screen(event.resize.w, event.resize.h, field_width, field_height);
                    break;

            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);

        Uint32 current_time = SDL_GetTicks();

        Uint32 delta = current_time - previous_time;

        previous_time = current_time;

        acc_time += delta;

        // one step each n milliseconds
        if (acc_time > 150) {

            ia.perform(5);

            bots.step();

            //bots.clean();
            acc_time = 0;
        }



        bots.for_each_bot([&bots] (const bot & the_bot) {


                auto t = the_bot.get_team() + 1;

                glColor3f(t * 0.1, t * 0.3, t * 0.5);

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
                }
                );

        SDL_GL_SwapBuffers();

        gameover = gameover || bots.game_over();
    }

    if(bots.game_over()) {
        if(bots.bot_count().size() != 1) {
            std::cout << "it's a tie!" << std::endl;
        }
        else {
            for(auto kv : bots.bot_count()) {
                std::cout << kv.first << " wins!" << std::endl;
            }
        }
    }

    /* cleanup SDL */
    SDL_Quit();


    return 0;
}

