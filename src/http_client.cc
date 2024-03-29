#include "http_client.hh"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace account_settings
{

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

// returns the content of the given http://host:port/path as string
// might throw HttpError
// implementation based on https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/example/cpp03/http/client/sync_client.cpp
// (Boost license 1.0)
std::string http_get_file(const std::string& host, int port, const std::string& path, bool tls)
{
	boost::asio::io_context io_context;

	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_context);
	tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

	tcp::socket* socket = nullptr;
	if(tls)
	{
		ssl::context ctx(ssl::context::sslv23);
		
	}else{
		// Try each endpoint until we successfully establish a connection.
		socket = new tcp::socket(io_context);
		boost::asio::connect(*socket, endpoints);
	}
	// Form the request. We specify the "Connection: close" header so that the
	// server will close the socket after transmitting the response. This will
	// allow us to treat all data up until the EOF as the content.
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "GET " << path << " HTTP/1.0\r\n"
		<< "Host: " << host << "\r\n"
		<< "Accept: */*\r\n"
		<< "Connection: close\r\n\r\n";

	// Send the request.
	boost::asio::write(*socket, request);

	// Read the response status line. The response streambuf will automatically
	// grow to accommodate the entire line. The growth may be limited by passing
	// a maximum size to the streambuf constructor.
	boost::asio::streambuf response;
	boost::asio::read_until(*socket, response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/")
	{
		throw HttpError(host, port, path, -1,  http_version);
	}
	
	if (status_code != 200)
	{
		throw HttpError(host, port, path, status_code, status_message);
	}

	// Read the response headers, which are terminated by a blank line.
	boost::asio::read_until(*socket, response, "\r\n\r\n");
/*
	// Process the response headers.
	std::string header;
	while (std::getline(response_stream, header) && header != "\r")
	  std::cout << header << "\n";
	std::cout << "\n";

	// Write whatever content we already have to output.
	if (response.size() > 0)
	  std::cout << &response;
*/
	// Read until EOF, writing data to output as we go.
	std::string content;
	
	boost::system::error_code error;
	while (boost::asio::read(*socket, response,
		  boost::asio::transfer_at_least(1), error))
	{
		auto bufs = response.data();
		auto begin = boost::asio::buffers_begin(bufs);
		content.append(begin, begin + bufs.size());
	}

	if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);
	
	return content;
}

} // end of namespace account_settings
