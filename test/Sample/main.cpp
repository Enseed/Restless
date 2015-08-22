#include <Restless/WebServer/RestlessServer.h>
#include <Restless/WebServices/WebService.h>
#include <Restless/HTTP/HttpMediaType.h>
#include <Restless/HTTP/HttpResult.h>
#include <iostream>

class HelloWorldWebService : public restless::WebService
{
public:
	HelloWorldWebService()
	{
		onGet("/world", [](const HttpRequest &request, const HttpArguments &args) {
			return (new HttpResult())->setBytes("Hello World!!!", HttpMediaType::TEXT_PLAIN);
		});
	}


};


class WebServer : public restless::RestlessServer
{
public:
	WebServer()
		: restless::RestlessServer(8081)
	{
		delegateSubPath("/hello", WebServiceRef(new HelloWorldWebService));
	}
};

int main(int argc, const char *argv[])
{
	WebServer webServer;
	std::cout << "waiting for connections: http://localhost:8081/hello/world" << std::endl;
	Sleep(1000000);
	return 0;
}