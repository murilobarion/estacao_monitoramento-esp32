// =======================================================================
// PROJETO: Estação de Monitoramento "Guardião do Lar"
// VERSÃO: 7.2 - Calibração de Luz Final
// AUTOR: Murilo Barion
// =======================================================================

// ==== BIBLIOTECAS ====
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==== CONFIGURAÇÕES DE REDE (MODO ACCESS POINT) ====
const char* ssid = "devbarion";
const char* password = "12345678";

// ==== MAPEAMENTO DE PINOS ====
// Sensores
#define PINO_SENSOR_DHT 23
#define TIPO_SENSOR_DHT DHT11
const int pinoLDR = 34;
// Climômetro
const int pinoLedFrio = 25;
const int pinoLedAgradavel = 26;
const int pinoLedQuente = 27;
// Semáforo de Conforto
const int pinoSemaforoVerde = 5;
const int pinoSemaforoAmarelo = 18;
const int pinoSemaforoVermelho = 19;
// LED de Status do Sistema
const int pinoLedStatus = 2;

// ==== OBJETOS E VARIÁVEIS GLOBAIS ====
DHT dht(PINO_SENSOR_DHT, TIPO_SENSOR_DHT);
WebServer server(80);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
float temperatura = 0.0, umidade = 0.0;
int valorLuz = 0;
String statusLuz = "---";
unsigned long tempoLeituraSensores = 0;
const long intervaloLeitura = 2000;

// ==== FUNÇÃO TRADUTORA DE LUZ ====
String converteLuzParaTexto(int valorLido) {
  // Limites calibrados pelo usuário.
  if (valorLido < 1300) { // <-- MUDANÇA APLICADA AQUI!
    return "Escuro";
  } else if (valorLido < 2500) {
    return "Luz Ambiente";
  } else if (valorLido < 3800) {
    return "Claro";
  } else {
    return "Luz Intensa";
  }
}

// ==== FUNÇÕES DE CONTROLE DOS LEDS ====
void atualizarClimometro(float temp) {
  digitalWrite(pinoLedFrio, LOW); digitalWrite(pinoLedAgradavel, LOW); digitalWrite(pinoLedQuente, LOW);
  if (temp < 23.0) digitalWrite(pinoLedFrio, HIGH);
  else if (temp <= 26.0) digitalWrite(pinoLedAgradavel, HIGH);
  else digitalWrite(pinoLedQuente, HIGH);
}

void atualizarSemaforoConforto(float umid) {
  digitalWrite(pinoSemaforoVerde, LOW); digitalWrite(pinoSemaforoAmarelo, LOW); digitalWrite(pinoSemaforoVermelho, LOW);
  if (umid < 40.0) digitalWrite(pinoSemaforoAmarelo, HIGH);
  else if (umid <= 60.0) digitalWrite(pinoSemaforoVerde, HIGH);
  else digitalWrite(pinoSemaforoVermelho, HIGH);
}

// ==== PÁGINA WEB E FUNÇÕES DO SERVIDOR ====
String paginaHTML() {
  String html = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Estação de Monitoramento</title>
<style>body{font-family:Arial,sans-serif;text-align:center;background-color:#2c3e50;color:#ecf0f1;padding:20px;}.container{max-width:400px;margin:auto;background-color:#34495e;padding:20px;border-radius:10px;}h1{color:#1abc9c;}.sensor{font-size:1.2em;margin:10px 0;}</style>
</head><body><div class="container"><h1>Estação de Monitoramento</h1><p class="sensor">Temperatura: <span id="temp">--</span> &deg;C</p><p class="sensor">Umidade: <span id="umid">--</span> %</p><p class="sensor">Luminosidade: <span id="luz">--</span></p></div>
<script>setInterval(function(){fetch('/dados').then(response=>response.json()).then(data=>{document.getElementById('temp').innerText=data.temperatura;document.getElementById('umid').innerText=data.umidade;document.getElementById('luz').innerText=data.luminosidade_texto;});},2000);</script>
</body></html>)rawliteral";
  return html;
}

void handleRoot() { server.send(200, "text/html", paginaHTML()); }
void handleDadosJSON() {
  String json = "{";
  json += "\"temperatura\":" + String(temperatura, 1) + ",";
  json += "\"umidade\":" + String(umidade, 1) + ",";
  json += "\"luminosidade_texto\":\"" + statusLuz + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// ==== SETUP: Executado uma vez ao ligar ====
void setup() {
  Serial.begin(115200);
  
  pinMode(pinoLedStatus, OUTPUT);
  digitalWrite(pinoLedStatus, LOW);

  pinMode(pinoLedFrio, OUTPUT); pinMode(pinoLedAgradavel, OUTPUT); pinMode(pinoLedQuente, OUTPUT);
  pinMode(pinoSemaforoVerde, OUTPUT); pinMode(pinoSemaforoAmarelo, OUTPUT); pinMode(pinoSemaforoVermelho, OUTPUT);
  
  dht.begin();
  Wire.begin(21, 22);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { Serial.println(F("Falha OLED")); }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando Ponto de Acesso...");
  display.display();
  
  WiFi.softAP(ssid, password);
  IPAddress meuIP = WiFi.softAPIP();
  Serial.println("Ponto de Acesso Criado!");
  Serial.print("Acesse em: http://"); Serial.println(meuIP);

  server.on("/", handleRoot);
  server.on("/dados", handleDadosJSON);
  server.begin();
  
  digitalWrite(pinoLedStatus, HIGH);
}

// ==== LOOP: Executado repetidamente ====
void loop() {
  server.handleClient();
  
  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoLeituraSensores >= intervaloLeitura) {
    tempoLeituraSensores = tempoAtual;
    
    // Faz a leitura de todos os sensores
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
    valorLuz = analogRead(pinoLDR);
    statusLuz = converteLuzParaTexto(valorLuz);

    // Atualiza os módulos de LED com base nos novos dados
    atualizarClimometro(temperatura);
    atualizarSemaforoConforto(umidade);

    // Atualiza o display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("--- Estacao Online ---");
    display.setCursor(0, 16);
    display.print("Temp: " + String(temperatura, 1) + " C");
    display.setCursor(0, 32);
    display.print("Umid: " + String(umidade, 1) + " %");
    display.setCursor(0, 48);
    display.print("Luz:  " + statusLuz);
    display.display();
  }
}