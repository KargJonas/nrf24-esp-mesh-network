#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

typedef std::function<void(void)> HandlerFunction;

ESP8266WebServer server(80);

void serverSetup()
{
  WiFi.softAP(AP_NAME, AP_PASS);
  server.begin();
}

void handleRoute(
    String route,
    String response,
    String responseType = "text/html",
    uint8 responseStatus = 200)
{
  server.on(route, [response, responseType, responseStatus]() {
    server.send(responseStatus, responseType, response);
  });
}