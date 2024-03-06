#include <WiFi.h>
#include <ESPmDNS.h>

// code to create AP mode for ESP32, host a web server and scan local wifi in the area.
// User must connect to the AP, go to the web page on 192.168.4.1 or sphynx-dev.local.
// User must connect esp32 to a wifi.
// This code is meant to be implemented into main.ino at https://github.com/PedroVidalDev/esp32-sphynx
// this should only be used alone for testing

const char* ssidAP = "Sphynx-WIFI";
const char* senhaAP = "12345678";

const char* html = R"HTML(
  <!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title> Sphynx - Conexão Wifi </title>

  </head>
  <style>

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
                          <th> Nome </th>
                          <th> Força do Sinal </th>
                          <th> Conectar </th>
                          <th> Desconectar </th>
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

WiFiServer serverAP(80);
WiFiClient clientAtual;
bool apServerUp = false;

void setupWiFi(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(html);
  int numRedes = WiFi.scanNetworks();
  for (int i = 0; i < numRedes; ++i) {
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

void setup() {
  Serial.begin(115200);

  Serial.print("Configurando Acess Point...");
  WiFi.softAP(ssidAP, senhaAP);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Endereco IP do AP: ");
  Serial.println(IP);

  serverAP.begin();
  apServerUp = true;

  if (!MDNS.begin("Sphynx-dev")) {
  Serial.println("Erro ao iniciar mDNS");
  return;
  }
}

void loop() {
  if (apServerUp){
    Serial.println("Server is up!");
    WiFiClient client = serverAP.available();
    if (client) {
      Serial.println("Novo Client.");
      clientAtual = client;
      while (client.connected()) {
        if (client.available()) {
            setupWiFi(clientAtual);
            break;
            }
      }
      client.stop();
      Serial.println("Client Desconectado..");
    }
    delay(500);
  }
}
