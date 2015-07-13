#include <Enseed/Restless/WebServer/WebServer.h>
#include <Enseed/Restless/WebServices/WebService.h>
#include <Enseed/Restless/HTTP/HttpMediaType.h>
#include <Enseed/Restless/HTTP/HttpResult.h>

class HelloWorldWebService : public restless::WebService
{
public:
	HelloWorldWebService()
	{
		onGet("/", [](const HttpRequest &request, const HttpArguments &args) {
			return (new HttpResult())->setBytes("Hello World!!!", HttpMediaType::TEXT_PLAIN);
		});
	}


};


class WebServer : public restless::WebServer
{
public:
	WebServer()
		: restless::WebServer(8081)
	{
		delegateSubPath("/hello", WebServiceRef(new HelloWorldWebService));
	}
};

int main(int argc, const char *argv[])
{
	WebServer webServer;
	Sleep(1000000);
	return 0;
}