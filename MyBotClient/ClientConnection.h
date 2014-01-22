/*
 * ClientConnection.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kasios
 */
#include "bots/bots.h"
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#ifndef _KBOT_CLIENTCONNECTION_
#define _KBOT_CLIENTCONNECTION_
namespace KBOT{
	class ClientConnection {
	private:
		boost::asio::ip::tcp::resolver _resolver;
		boost::asio::ip::tcp::socket _socket;
		boost::asio::streambuf _packetToSend;
		boost::asio::streambuf _receiver;
		bots & _bots;
		boost::mutex & _state_mutex;
		bot::team_id & _id;
	public:
		ClientConnection(boost::asio::io_service&, const std::string&,
				const std::string&, bots &, boost::mutex &, bot::team_id &);
		void handle_write_request(const boost::system::error_code& );
		void handle_read_command(const boost::system::error_code&);
		void handle_connect(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);
		void handle_resolve(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);
		virtual ~ClientConnection();
	};
}
#endif /* _KBOT_CLIENTCONNECTION_ */
