/*
  SphynxWiFi library: https://github.com/Snootic/sphynx_network_tools/tree/main/wifi-scan
  For Sphynx: https://github.com/PedroVidalDev/esp32-sphynx
  code to create AP mode for ESP32, host a web server and scan local wifi in the area.
  Connect to the web page on 192.168.4.1 or sphynx-dev.local and connect to a WiFi.
  By: Snootic & PedroVidalDev - 2024
*/
#include "SphynxWiFi.h"
#include "Arduino.h"

AsyncWebServer apServer(80);
Preferences WiFiPreferences;

String html = R"HTML(
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
            flex-direction: column;
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
            scrollbar-gutter: stable;
            overflow-y: scroll;
        }

        .atualizar{
            position: relative;
            margin-top: 15px;
            width: 20%;
            height: 30px;
            border: 0;
            border-radius: 15px;
            cursor: pointer;
            transition: .5s;
        }

        .atualizar:hover{
            background-color: #022534;
            color: white;
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
            cursor: pointer;
            transition: .5s;
        }

        .correct:hover{
            color:aquamarine;
        }

        table.content-table tbody tr{
            border-bottom: 1px solid var(--color-background);
            text-align: center;
        }

        .container::-webkit-scrollbar{
            width: 10px;
        }

        .container::-webkit-scrollbar-track {
            border-radius: 10px;
            background-color: gray;
        }
        
        .container::-webkit-scrollbar-thumb {
            background: #fff; 
            border-radius: 10px;
        }

        .conn-container{
            position: absolute;
            display: none;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            width: 90%;
            height: 90%;
            background-color: #022534af;
            border-radius: 15px;
            backdrop-filter: blur(10px);
            z-index: 99;
        }

        .conn-container form{
            position: relative;
            display: flex;
            flex-direction: column;
            width: 100%;
            height: 100%;
            align-items: center;
            justify-content: center;
        }

        .conn-container form label{
            color: white;
            font-size: 20px;
        }

        .password{
            border: 0;
            border-radius: 10px;
            width: 40%;
            height: 30px;
            font-size: 20px;
            padding: 10px;
        }

        .password:focus-visible{
            outline: none;
        }

        .submit{
            position: relative;
            margin-top: 20px;
            width: 20%;
            height: 30px;
            border-radius: 10px;
            border: 0;
            cursor: pointer;
        }

        .conn-exit{
            position: relative;
            margin-bottom: 30px;
            width: 20%;
            height: 30px;
            border: 0;
            border-radius: 15px;
            cursor: pointer;
            transition: .5s;
        }

        .conn-exit:hover{
            background-color: #022534;
            color: white;
        }

        @media(max-width: 935px){
            table.content-table th, table.content-table td {
            font-size: 0.6rem;
            }

            table.content-table th, table.content-table td a{
                font-size: 0.6rem;
            }

            .container{
            height: 70%;
            }
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
                            <th> Força do sinal </th>
                            <th> Conectar </th>
                        </tr>
                    </thead>
                    <tbody>)HTML";

String htmlFooter = R"footer(</tbody>
    </table>
        </div>
        <button class="atualizar"> Atualizar </button>
        <div class="conn-container">
            <form action="/post" method="POST">
                <input type="hidden" name="ssid" id="ssid">
                <label class="label" for="password">Digite a senha:</label>
                <input class="password" type="password" name="password">
                <input class="submit" type="submit" value="Enviar">
            </form>
            <button class="conn-exit"> Sair </button>
        </div>
    </main>
    <footer></footer>
    </body>
    <script>
        const buttons = document.querySelectorAll(".correct");
        const widget = document.querySelector(".conn-container");
        const label = document.querySelector(".label");
        const buttonExit = document.querySelector(".conn-exit")
        const buttonUpdate = document.querySelector(".atualizar");
        const ssidInput = document.querySelector("#ssid");

        buttons.forEach(element => {
            element.addEventListener("click", () => {
                const ssid = element.parentNode.parentNode.querySelectorAll("td")[0].textContent;
                
                ssidInput.value = ssid.trim();

                widget.style.display = "flex";
                label.textContent = "Digite a senha da rede " + ssid + ":"
            })
        });

        buttonExit.addEventListener("click", () => {
        widget.style.display = "none";
        })

        buttonUpdate.addEventListener("click", () => {
        location.reload();
        })
    </script>
    </html>)footer";

String indexHtml;
String ssid;
String senha;

SphynxWiFiClass::SphynxWiFiClass(){}

int SphynxWiFiClass::connect() {
    WiFiPreferences.begin("WiFiCred", true);

    ssid = WiFiPreferences.getString("ssid", "");
    senha = WiFiPreferences.getString("senha", "");

    WiFiPreferences.end();

    if (ssid == "" || senha == ""){
        return 1;
    }
    WiFi.begin(ssid, senha);
    // Tries to connect to the specified wifi
    int tentativas = 0;
    while (tentativas <= 15 && WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.println("Esperando conexão wifi...");
        tentativas++;
    }
    if (tentativas > 15){
        Serial.println("Não foi possível se conectar ao WiFI");
        WiFi.disconnect();
        return 1;
    }else{
        try {
            if (WiFi.softAPdisconnect(true) != 0) {
                throw "Nenhum AP encontrado";
            }
            apServer.end();
        } catch (const char* error) {
            Serial.println(error);
        }
        Serial.print("Endereço IP:");
        Serial.println(WiFi.localIP());
        Serial.print("Endereço MAC:");
        Serial.println(WiFi.macAddress());
        WiFi.setHostname("Sphynx");
        Serial.print("Hostname:");
        Serial.println(WiFi.getHostname());
        return 0;
    }
}

void SphynxWiFiClass::saveCredentials(){
    apServer.on("/post", HTTP_POST,[](AsyncWebServerRequest * request){
        if(request->hasParam("ssid", true) && request->hasParam("password", true)){
            Serial.println("RECEBEU OS PARAMETROS");
            ssid = request->getParam("ssid", true)->value();
            Serial.println("OK");
            senha = request->getParam("password", true)->value();
            Serial.println("OK");

        }else{
            Serial.println("Não foi possível pegar SSID e SENHA");
            return;
        }

        WiFiPreferences.begin("WiFiCred", false);
        WiFiPreferences.clear();
        WiFiPreferences.putString("ssid", ssid);
        WiFiPreferences.putString("senha", senha);

        WiFiPreferences.end();
        SphynxWiFi.connect();
    });
}

void SphynxWiFiClass::scan() {
  // WiFi config Web Server
  apServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   String linhas = ""; 
   if(WiFi.scanComplete() == -2){
    WiFi.scanNetworks(true);
    linhas += ("<tr><td>Procurando</td><td>Procurando</td><td> <a class='correct'>Procurando</a> </tr>");
   }else if (WiFi.scanComplete() == -1){
    linhas += ("<tr><td>Procurando</td><td>Procurando</td><td> <a class='correct'>Procurando</a> </tr>");
   }else{
    Serial.print("Encontrado ");
    Serial.print(WiFi.scanComplete());
    Serial.println(" redes WiFi na area");
    for (int i = 0; i < WiFi.scanComplete(); ++i) {
      linhas += ("<tr><td>" + WiFi.SSID(i) + "</td><td>" + WiFi.RSSI(i) + "dBm</td><td> <a class='correct'> Connect </a> </tr>");
    }
    WiFi.scanDelete();
  }
  indexHtml = html + linhas + htmlFooter;
  linhas = "";
  request->send_P(200, "text/html", indexHtml.c_str());
  });
}

void SphynxWiFiClass::setupWiFi() {
    Serial.println("Configurando Access Point...");
    WiFi.softAP(ssidAP, senhaAP);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("Endereco IP do AP: ");
    Serial.println(IP);

    if (!MDNS.begin("Sphynx-dev")) {
        Serial.println("Erro ao iniciar mDNS");
        return;
    }
    apServer.begin();
    scan(); //Starts AP Mode server. Connects to 192.168.4.1 or sphynx-dev.local
    saveCredentials();
}

int SphynxWiFiClass::status() {
    return WiFi.status();
}

SphynxWiFiClass SphynxWiFi;
