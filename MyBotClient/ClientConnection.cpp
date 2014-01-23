/*
 * ClientConnection.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: root
 */

#include "ClientConnection.h"
#include "ClientIA.h"
#include <boost/bind.hpp>
#include <boost/archive/text_iarchive.hpp>

using boost::asio::ip::tcp;

void KBOT::ClientConnection::handle_write_request(const boost::system::error_code& err)
{
	if (!err){}else
	{
		std::cout << "Error writing: " << err.message() << "\n";
	}
}

void KBOT::ClientConnection::handle_read_command(const boost::system::error_code& err)
{
	if (!err)
	{

		//PREPARE FOR READ A NEW PACKET
		boost::asio::async_read_until(_socket, _receiver, "\n",
			boost::bind(&KBOT::ClientConnection::handle_read_command,this,
			boost::asio::placeholders::error));

		// Check that response is OK.
		std::string data;
		std::istream _receiverstream(&_receiver);
		std::getline(_receiverstream, data);

		std::istringstream stream(data);
		std::string command;
		stream >> command;

		// Read the response headers, which are terminated by a blank line.
		if(command == "welcome") {
			stream >> _id;
			std::cout << data << std::endl;
			//ai.set_team(id);
			while(!stream.eof()) {
				std::string a;
				stream >> a;
				//state << a << " ";
			}
		}
		else if(command == "state") {

			std::stringstream state;
			while(!stream.eof()) {
				std::string a;
				stream >> a;
				state << a << " ";
			}
			boost::archive::text_iarchive ia(state);
			{
				// mutex:
				// segfaults if it draws during a state update (drawing +
				// incomplete state is fatal)
				boost::mutex::scoped_lock lock(_state_mutex);
				ia >> _bots;
			}
		}
		//TODO change size
		KBOT::ClientIA * c = new KBOT::ClientIA(_bots,_id, 10, 10);
		_bots.for_each_bot([&] (const bot & mybot) {
			if (mybot.get_team()==_id){
				const bot::position pos= mybot.get_position();
				std::ostream _packetToSendstream(&_packetToSend);
				_packetToSendstream << "move " << pos.first << " " << pos.second << " " << c->decideMovement(mybot) << "\n";
				boost::asio::async_write(_socket, _packetToSend,
					boost::bind(&ClientConnection::handle_write_request,this,
					boost::asio::placeholders::error));
			}
		});
		delete c;
	}
	else
	{
		std::cout << "Error reading command: " << err << "\n";
	}
}

void KBOT::ClientConnection::handle_connect(const boost::system::error_code& err,
  tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{

		// The connection was successful. Send the request.
		std::cout << "Retrieving..." << std::endl;
		// Read the response status line.
		boost::asio::async_read_until(_socket, _receiver, "\n",
			boost::bind(&ClientConnection::handle_read_command, this,
			boost::asio::placeholders::error));
	}
	else if (endpoint_iterator != tcp::resolver::iterator())
	{
		// The connection failed. Try the next endpoint in the list.
		_socket.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		_socket.async_connect(endpoint,
			boost::bind(&ClientConnection::handle_connect,this,
			boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

void KBOT::ClientConnection::handle_resolve(const boost::system::error_code& err,
  tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// Attempt a connection to the first endpoint in the list. Each endpoint
		// will be tried until we successfully establish a connection.
		std::cout << "Connecting to " << endpoint_iterator->host_name() << std::endl;
		tcp::endpoint endpoint = *endpoint_iterator;
		_socket.async_connect(endpoint,
			boost::bind(&ClientConnection::handle_connect,this,
			boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{
		std::cout << "Error resolving: " << err.message() << "\n";
	}
}

KBOT::ClientConnection::ClientConnection(boost::asio::io_service& io_service, const std::string& server,
		const std::string& port, bots & bts, boost::mutex & sm, bot::team_id & id)
: _resolver(io_service),
  _socket(io_service),
  _bots(bts),
  _state_mutex(sm),
  _id(id)
{
	// Start an asynchronous resolve to translate the server and service names
	// into a list of endpoints.
	std::cout << "Resolving..." << std::endl;
	tcp::resolver::query query(server, port);
	_resolver.async_resolve(query,
		boost::bind(&ClientConnection::handle_resolve,this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::iterator));
}

KBOT::ClientConnection::~ClientConnection() {
	// TODO Auto-generated destructor stub
}
