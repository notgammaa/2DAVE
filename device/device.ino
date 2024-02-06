#include <WiFi.h>
#include <ArduinoJson.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ChatGPT.hpp>
#include <base64.h>

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 10
#define SIOD_GPIO_NUM 40
#define SIOC_GPIO_NUM 39
#define Y9_GPIO_NUM 48
#define Y8_GPIO_NUM 11
#define Y7_GPIO_NUM 12
#define Y6_GPIO_NUM 14
#define Y5_GPIO_NUM 16
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 17
#define Y2_GPIO_NUM 15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM 47
#define PCLK_GPIO_NUM 13
#define LED_GPIO_NUM 21

const char* ssid = "DESKTOP-2J4Q4DQ 3710";
const char* password = "5Kd296]3";

const char* openaiApiKey = "Bearer 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";
const char* host = "192.168.1.252";
const int port = 5000;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

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
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  Serial.print("Camera: ");
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("False");
    return;
  }
  Serial.println("True");

  Serial.print("WiFi: ");
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected");

  Serial.print("Connection: ");
  WiFiClient client;
  HTTPClient http;
  // client.setInsecure();
  if (!http.begin(client, host, port, "/", false)) {
    Serial.println("False");
    return;
  }
  Serial.println("True");

  Serial.print("Image: ");
  camera_fb_t* fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("False");
  }
  String image = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  Serial.println("True");

  Serial.print("Request: ");
  String payloadStr = "{\"image\":\"" + image + "\"}";
  http.addHeader("Content-Type", "application/json");
  // http.addHeader("Authorization", openaiApiKey);
  // http.addHeader("Host", host);
  http.addHeader("Content-Length", String(payloadStr.length()));
  Serial.println("True");

  Serial.print("Response: ");
  int responseCode = http.POST(payloadStr.c_str());
  Serial.println(responseCode);

  if (responseCode < 0) {
    return;
  }

  String responseStr = http.getString();
  Serial.println(responseStr);

  // JsonDocument response;
  // deserializeJson(response, responseStr);


  // String responseContent = response["choices"][0]["message"]["content"];
  // Serial.println(responseContent);
}

void loop() {
  delay(10000);
}
