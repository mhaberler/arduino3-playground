#ifdef WEBSERVER

#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFiMulti.h>
#include <WiFi.h>

#include "lv_setup.hpp"
#include "subjects.hpp"
#include "lv_util.h"

static WiFiMulti wifiMulti;
static WebServer *http_server;
static MDNSResponder *mdns_responder;

static void wifi_event_cb(WiFiEvent_t event, WiFiEventInfo_t info);
static void drawGraph(void);
static void handleRoot(void);
static void handleNotFound(void);

void webserver_loop(void)
{
  if (http_server)
    http_server->handleClient();
}

void webserver_setup(void)
{
  // lvgl_msg_send_prot(MSG_WIFI_UNCONFIGURED, NULL);

  WiFi.mode(WIFI_STA);
  WiFi.enableProv(true);
  WiFi.onEvent(wifi_event_cb);

  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
#if defined(WIFI_SSID11)
  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);
#endif
#if defined(WIFI_SSID2)
  wifiMulti.addAP(WIFI_SSID2, WIFI_PASSWORD2);
#endif
#if defined(WIFI_SSID3)
  wifiMulti.addAP(WIFI_SSID3, WIFI_PASSWORD3);
#endif
#if defined(WIFI_SSID4)
  wifiMulti.addAP(WIFI_SSID4, WIFI_PASSWORD4);
#endif
  wifiMulti.run();
}

static void start_net_services(void)
{

  if (!mdns_responder)
  {
    mdns_responder = new MDNSResponder();
  }
  if (mdns_responder->begin("esp32"))
  {
    Serial.printf("MDNS responder started\n");
  }
  if (!http_server)
  {
    http_server = new WebServer(80);
    http_server->on("/", handleRoot);
    http_server->on("/test.svg", drawGraph);
    http_server->on("/inline",
                    []()
                    { http_server->send(200, "text/plain", "this works as well"); });
    http_server->onNotFound(handleNotFound);
    http_server->begin();
    Serial.printf("HTTP server started\n");
    lvgl_acquire();
    lv_subject_set_int(&http_status, STATUS_HTTP_IDLE);
    lvgl_release();
  }
}

static void stop_net_services(void)
{

  if (http_server)
  {
    delete http_server;
  }
  if (mdns_responder)
  {
    delete mdns_responder;
  }
  lvgl_acquire();
  lv_subject_set_int(&http_status, STATUS_HTTP_STOPPED);
  lvgl_release();
}

static void drawGraph(void)
{
  lvgl_acquire();
  lv_subject_set_int(&http_status, STATUS_HTTP_ACTIVE);
  lvgl_release();

  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
         "width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" "
         "stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10)
  {
    int y2 = rand() % 130;
    sprintf(
        temp,
        "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n",
        x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  http_server->send(200, "image/svg+xml", out);
}

static void handleRoot(void)
{
  lvgl_acquire();
  lv_subject_set_int(&http_status, STATUS_HTTP_ACTIVE);
  lvgl_release();

  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           hr, min % 60, sec % 60);
  http_server->send(200, "text/html", temp);
}

static void handleNotFound(void)
{
  lvgl_acquire();
  lv_subject_set_int(&http_status, STATUS_HTTP_ACTIVE);
  lvgl_release();
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += http_server->uri();
  message += "\nMethod: ";
  message += (http_server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += http_server->args();
  message += "\n";

  for (uint8_t i = 0; i < http_server->args(); i++)
  {
    message += " " + http_server->argName(i) + ": " + http_server->arg(i) + "\n";
  }

  http_server->send(404, "text/plain", message);
}

static void wifi_event_cb(WiFiEvent_t event, WiFiEventInfo_t info)

{
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event)
  {
  case ARDUINO_EVENT_WIFI_READY:
    Serial.println("WiFi interface ready");
    break;
  case ARDUINO_EVENT_WIFI_SCAN_DONE:
    Serial.println("Completed scan for access points");
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_SCAN_COMPLETE);
    lvgl_release();
    break;
  case ARDUINO_EVENT_WIFI_STA_START:
    Serial.println("WiFi client started");
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_STARTED);
    lvgl_release();
    break;
  case ARDUINO_EVENT_WIFI_STA_STOP:
    Serial.println("WiFi clients stopped");
    break;
  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
    Serial.println("Connected to access point");
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_CONNECTED);
    lvgl_release();
    break;
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    Serial.println("Disconnected from WiFi access point");
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_DISCONNECTED);
    lvgl_release();
    break;
  case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
    Serial.println("Authentication mode of access point has changed");
    break;
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    Serial.print("Obtained IP address: ");
    Serial.println(WiFi.localIP());
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_GOT_IP);
    lvgl_release();
    start_net_services();
    break;
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    Serial.println("Lost IP address and IP address is reset to 0");
    lvgl_acquire();
    lv_subject_set_int(&wifi_status, STATUS_WIFI_LOST_IP);
    lvgl_release();
    stop_net_services();
    break;
  case ARDUINO_EVENT_WPS_ER_SUCCESS:
    Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
    break;
  case ARDUINO_EVENT_WPS_ER_FAILED:
    Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
    break;
  case ARDUINO_EVENT_WPS_ER_TIMEOUT:
    Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
    break;
  case ARDUINO_EVENT_WPS_ER_PIN:
    Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
    break;
  case ARDUINO_EVENT_WIFI_AP_START:
    Serial.println("WiFi access point started");
    break;
  case ARDUINO_EVENT_WIFI_AP_STOP:
    Serial.println("WiFi access point  stopped");
    break;
  case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
    Serial.println("Client connected");
    break;
  case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
    Serial.println("Client disconnected");
    break;
  case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
    Serial.println("Assigned IP address to client");
    break;
  case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
    Serial.println("Received probe request");
    break;
  case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
    Serial.println("AP IPv6 is preferred");
    break;
  case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
    Serial.println("STA IPv6 is preferred");
    break;
  default:
    break;
  }
}

#else
void webserver_setup(void) {}
void webserver_loop(void) {}
#endif
