#include <WiFi.h>
#include <ESPmDNS.h>

// code to create AP mode for ESP32, host a web server and scan local wifi in the area.
// User must connect to the AP, go to the web page on 192.168.4.1 or sphynx-dev.local.
// User must connect esp32 to a wifi.
// This code is meant to be implemented into main.ino at https://github.com/PedroVidalDev/esp32-sphynx
// this should only be used alone for testing

const char* ssid = "Sphynx-WIFI";
const char* password = "12345678";
const char* html = R"HTML(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title> Sphynx - Connection </title>
    <script src="https://kit.fontawesome.com/2e476e3619.js" crossorigin="anonymous"></script>

</head>
<style>
    @import url('https://fonts.googleapis.com/css2?family=Roboto+Condensed:wght@300&family=Roboto:wght@100&display=swap');

    *{
        padding: 0;
        margin: 0;
        box-sizing: border-box;
        text-decoration: none;
        list-style: none;
    }

    html{
        font-size: 16px;
        font-family: 'Roboto';
    }

    :root{
        --color-background: #024959;
        --color-background-bright: #026773;
        --color-gray: #D9D9D9;
        --color-submenu: #022534;
    }

    main{
        position: relative;
        display: flex;
        align-items: center;
        justify-content: center;
        padding: 0 30px;
        width: 100%;
        height: 100vh;
        background-color: var(--color-background);
    }

    .container{
        position: relative;
        display: flex;
        align-items: flex-start;
        justify-content: center;
        width: 90%;
        height: 90%;
        background-color: var(--color-background-bright);
        border-radius: 15px;
        overflow-y: scroll;
    }

    table.content-table{
        border-collapse: collapse;
        font-size: 2rem;
        min-width: 90%;
        margin-top: 20px;
    }

    table.content-table thead tr{
        background-color: var(--color-background);
        color: white;
        text-align: center;
        font-weight: bold;
    }

    table.content-table th, table.content-table td {
        padding: 12px 15px;
        color: white;
    }

    table.content-table th, table.content-table td a{
        color: white;
        transition: .5s;
    }

    .correct:hover{
        color: green;
    }

    .incorrect:hover{
        color: red;
    }

    table.content-table tbody tr{
        border-bottom: 1px solid var(--color-background);
        text-align: center;
    }
</style>
<body>
    <header></header>
    <main>
        <div class="container">
            <table class="content-table">
                <thead>
                    <tr>
                        <th> Name </th>
                        <th> Signal </th>
                        <th> Connect </th>
                        <th> Disconnect </th>
                    </tr>
                </thead>
                <tbody>)HTML";

const char* htmlFooter = R"footer(                </tbody>
            </table>
        </div>
    </main>
    <footer></footer>
</body>
</html>)footer";

WiFiServer server(80);
WiFiClient currentClient;

void setup() {
  Serial.begin(115200);

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();

  if (!MDNS.begin("Sphynx-dev")) {
    Serial.println("Error starting mDNS");
    return;
  }
}

void handleClientRequest(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(html);
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; ++i) {
    client.println("                    <tr>");
    client.print("                        <td>");
    client.print(WiFi.SSID(i));
    client.println("</td>");
    client.print("                        <td>");
    client.print(WiFi.RSSI(i));
    client.println(" dBm</td>");
    client.println("                      <td>");
    client.println("<a class=\"correct\" href=\"\"> <i class=\"fa-solid fa-square-check\"></i> </a>");
    client.println("</td>");
    client.println("                      <td>");
    client.println("<a class=\"incorrect\" href=\"\"><i class=\"fa-solid fa-circle-xmark\"></i></a>");
    client.println("</td>");
    client.println("                   </tr>");
  }
  client.println(htmlFooter);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    currentClient = client;
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            handleClientRequest(currentClient);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
  delay(500);
}