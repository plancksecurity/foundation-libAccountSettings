#ifndef ACCOUNT_SETTINGS_HTTP_CLIENT_HH
#define ACCOUNT_SETTINGS_HTTP_CLIENT_HH

#include <string>
#include <stdexcept>

namespace account_settings
{

class HttpError : public std::runtime_error
{
public:
	HttpError(const std::string& _host, int _port, const std::string& _path,
		int _error_code, const std::string& _error_message)
	: std::runtime_error{ "HTTP Error for http://" + _host + ":" + std::to_string(_port) + _path 
		+ " : Status=" + std::to_string(_error_code) + (_error_message.empty() ? "" : (": " + _error_message) )}
	, host{_host}
	, port{_port}
	, path{_path}
	, error_code{_error_code}
	, error_message{_error_message}
	{}
	
	// are public by intention
	const std::string host;
	const int port;
	const std::string path;
	const int error_code;
	const std::string error_message;
};


// returns the content of the given http://host:port/path as string
// might throw HttpError
std::string http_get_file(const std::string& host, int port, const std::string& path);

} // end of namespace account_settings

#endif // ACCOUNT_SETTINGS_HTTP_CLIENT_HH
