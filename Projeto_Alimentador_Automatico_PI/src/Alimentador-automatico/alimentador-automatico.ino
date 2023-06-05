// Bibliotecas usadas
#include <Stepper.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "Base64.h"
#include "esp_camera.h"

// Definicao dos pinos do ESP32 CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Numeros dos pinos usados pelo Sensor de distancia.
const int trig = 12;
const int echo = 13;

// Variaveis para o calculo da distancia em CM 
int duracaoPulso;
float velocidadeDoSom;
float distanciaCm;

// Configuracoes do Motor de passo.
// pinos usados 4 2 14 15
// IN1 IN2 IN3 IN4
const int passosPorRevolucao = 2048; 
Stepper myStepper(passosPorRevolucao, 4, 14, 2, 15);
float anguloDesejado = 90;
float passosAnguloDesejado = (passosPorRevolucao * anguloDesejado) / 360;

// Usuario e Senha do Wifi.
const char* ssid = "CKK";
const char* password = "senha123";

//
String myDeploymentID = "AKfycbzLulu1xOtx6XpRuF7twXN1UYSsEqm8_V5KryekEbmcvCwuouLdhABfIf4TEdduXdy8Zw";
String myMainFolderName = "ESP32-CAM";

// Variaveis usadas para as pausas no loop
unsigned long comecoTempo1 = 0;
unsigned long comecoTempo2 = 0;
const int intervalo1 = 3000;
const int intervalo2 = 30000;
//

WiFiClientSecure client;

// Funcoes

void pulsoUltrassonico() {
  digitalWrite(trig, HIGH); // Envia o pulso ultrassonico
  delayMicroseconds(10);
  digitalWrite(trig, LOW); // Corta o pulso ultrassonico
}

void sentidoHorario() {
  myStepper.step(passosAnguloDesejado); 
}

void sentidoAntiHorario() {
  myStepper.step(-passosAnguloDesejado); 
}

void tirandoEEnviandoFotos() {
  const char* host = "script.google.com";
  
  client.setInsecure();

  //  Conexao, captura e envio de fotos para o google drive 
  if (client.connect(host, 443)) {
    
    for (int i = 0; i <= 3; i++) {
      camera_fb_t * fb = NULL;
      fb = esp_camera_fb_get();
       if(!fb) {
          delay(1000);
          ESP.restart();
          return;
        } 
      esp_camera_fb_return(fb);
      delay(200);
    }
  
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if(!fb) {
      delay(1000);
      ESP.restart();
      return;
    } 

    // Enviando a foto para o google drive
    String url = "/macros/s/" + myDeploymentID + "/exec?folder=" + myMainFolderName;

    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Transfer-Encoding: chunked");
    client.println();

    int fbLen = fb->len;
    char *input = (char *)fb->buf;
    int chunkSize = 3 * 1000; 
    int chunkBase64Size = base64_enc_len(chunkSize);
    char output[chunkBase64Size + 1];

    int chunk = 0;
    for (int i = 0; i < fbLen; i += chunkSize) {
      int l = base64_encode(output, input, min(fbLen - i, chunkSize));
      client.print(l, HEX);
      client.print("\r\n");
      client.print(output);
      client.print("\r\n");
      delay(100);
      input += chunkSize;
      Serial.print(".");
      chunk++;
      if (chunk % 50 == 0) {
      }
    }
    client.print("0\r\n");
    client.print("\r\n");

    esp_camera_fb_return(fb);

    //
    long int StartTime = millis();
    while (!client.available()) {
      Serial.print(".");
      delay(100);
      if ((StartTime + 10 * 1000) < millis()) {
        break;
      }
    }
    while (client.available()) {
      Serial.print(char(client.read()));
    }
    
  }
  else {
    Serial.println("Connected to " + String(host) + " failed.");
    
  }
  client.stop();
}



void setup() {
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  delay(1000);
  
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
  digitalWrite(trig, LOW);
  
  myStepper.setSpeed(12);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10; 
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 8; 
    config.fb_count = 1;
  }
  
  // Inicializacao da camera 
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    delay(1000);
    ESP.restart();
  }

  sensor_t * s = esp_camera_sensor_get();

  // Detalhes das resolucoes da camera
  // -UXGA   = 1600 x 1200 pixels
  // -SXGA   = 1280 x 1024 pixels
  // -XGA    = 1024 x 768  pixels
  // -SVGA   = 800 x 600   pixels
  // -VGA    = 640 x 480   pixels
  // -CIF    = 352 x 288   pixels
  // -QVGA   = 320 x 240   pixels
  // -HQVGA  = 240 x 160   pixels
  // -QQVGA  = 160 x 120   pixels
  s->set_framesize(s, FRAMESIZE_SXGA); 

  delay(2000);
} 


void loop() {
  
  pulsoUltrassonico();
  duracaoPulso = pulseIn(echo, HIGH); // Funcao que calcula o tempo do pulso (pulso em microssegundos)
  velocidadeDoSom = 0.0343; 
  distanciaCm = (duracaoPulso * velocidadeDoSom) / 2; 
  
  if(distanciaCm >= 3 && distanciaCm <= 20) {
    
    tirandoEEnviandoFotos();

    sentidoHorario();

    comecoTempo1 = millis();
    while (millis() < comecoTempo1 + intervalo1) {
        // Nao ha codigo aqui, e apenas uma pausa de 3s (tempo para racao cair).
    }
    
    sentidoAntiHorario();

    comecoTempo2 = millis();
    while (millis() < comecoTempo2 + intervalo2) {
      // Nao ha codigo aqui, apenas uma pausa de 30s (tempo para o cachorro comer e nao ser detectado pelo sensor, evitando a liberacao de racao sem parar).
    }
  }
} 
