#include <ESP8266WIFI.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Create an instance of the server
ESP8266WebServer server(80);

// Create an LED matrix object
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void handleRoot() {
  // If POST request, read the input data
  if (server.method() == HTTP_POST) {
    String input = server.arg("input");
    int inputLength = input.length();
    // Scroll the input on the LED matrix
    for (int i = 0; i <= inputLength; i++) {
      matrix.clear();
      matrix.setCursor(0, 0);
      matrix.print(input.substring(i));
      matrix.writeDisplay();
      delay(100);
    }
    // Send response to the client
    server.send(200, "text/plain", "Input received");
  }
  // If GET request, send the HTML page to the client
  else {
    String html = "<html><body><form method='post'>Input: <input type='text' name='input'><br><br><input type='submit' value='Submit'></form></body></html>";
    server.send(200, "text/html", html);
  }
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.on("/", handleRoot);
  server.begin();
  
  // Initialize the LED matrix
  matrix.begin(0x70);
  matrix.setTextWrap(false);
  matrix.setBrightness(15);
}

void loop() {
  server.handleClient();
}
