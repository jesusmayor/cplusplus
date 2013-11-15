#include "bots.h"
#include "botclient.h"
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <boost/archive/text_iarchive.hpp> 
#include <boost/archive/text_oarchive.hpp> 


using boost::asio::ip::tcp;

void send(tcp::socket &socket, const std::string & str) {
    boost::asio::write(socket, boost::asio::buffer(str + "\n"), boost::asio::transfer_all());
}

void set_screen(int w, int h, int fw, int fh) {

    SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, fw, 0, fh);
    glClearColor(0.0, 0.0 ,0.0, 0);

}


void game_thread(std::shared_ptr<tcp::socket> socket, bool &gameover, bots*& bots, bot::team_id &id) {
    boost::asio::streambuf buf;
    while(!gameover) {

        read_until(*socket, buf, "\n"); 

        std::string data;
        std::istream is(&buf);
        std::getline(is, data); 

        std::istringstream stream(data);

        std::string command;
        stream >> command;

        if(command == "team") {
            stream >> id;
        }
        else if(command == "state") {
            
            std::stringstream state;
            while(!stream.eof()) {
                std::string a;
                stream >> a;
                state << a << " ";
            }

            std::cout << state.str() << std::endl;
            boost::archive::text_iarchive ia(state); 
            ia >> *bots;
            std::cout << "w: " << bots->_width << std::endl;
            std::cout << "h: " << bots->_height<< std::endl;
        }

    }
}

int main(int argc, char* argv[])
{
    if(argc < 3) {
        std::cout << argv[0] << " " << "[server] [port]" << std::endl;
        return -1;
    }
    else {

        bot::team_id id = 1000;

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });

        std::shared_ptr<tcp::socket> socket(new tcp::socket(io_service));
        boost::asio::connect(*socket, endpoint_iterator);

        int field_width = 2;
        int field_height = 2;

        const int win_width = 500;
        const int win_height = 500;

        bots * bots = nullptr;

        SDL_Event event;
        bool gameover = false;


        boost::thread t = boost::thread([socket, &gameover, &bots, &id] () { game_thread(socket, gameover, bots, id); } );


        while (!gameover) {
        }

        if(bots && bots->game_over()) {
            if(bots->bot_count().size() != 1) {
                std::cout << "it's a tie!" << std::endl;
            }
            else {
                for(auto kv : bots->bot_count()) {
                    std::cout << kv.first << " wins!" << std::endl;
                }
            }
        }

        send(*socket, "goodbye");
        return 0;
    }
}

