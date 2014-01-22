#include "bots.h"

#include <SDL/SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "ClientConnection.h"
#include "FrontendView.h"

using boost::asio::ip::tcp;

void windowThread(KBOT::FrontendView * window,bots & bots,boost::mutex & state_mutex){
	while(true) {
		window->draw(bots,state_mutex);
	}
}

//Asio asynchronous
//http://www.boost.org/doc/libs/1_38_0/doc/html/boost_asio/example/http/client/async_client.cpp

/**
 * Main Class of the client
 * @params argv[] Need two params: server and port.
 */
int main(int argc, char* argv[])
{
	if(argc != 3) {
		std::cout << argv[0] << " " << "[server] [port]" << std::endl;
		return -1;
	}
	else {
	    bot::team_id id = 1000;
		bots bots;
		boost::mutex state_mutex;
		try{
		    std::cout << "StartClientMain" << std::endl;
		    boost::asio::io_service io_service;
		    KBOT::ClientConnection c(io_service, argv[1], argv[2],bots, state_mutex, id);
	        KBOT::FrontendView* window = KBOT::FrontendView::getInstance();
		    boost::thread t(windowThread,window,boost::ref(bots),boost::ref(state_mutex));
	        io_service.run();
		    std::cout << "EndClientMain" << std::endl;
		}catch (std::exception& e){
			std::cout << "Main Explode! Exception:" << e.what() << std::endl;
			return -1;
		}
		return 0;
	}
};//*/

