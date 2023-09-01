#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// WiFi credentials in the Secret tab
char ssid[] = "@prabh";           //WiFi network SSID
char pass[] = "0987654321";       // WiFi password

WiFiClient client;
BH1750 lightMeter;

char   HOST_NAME[] = "maker.ifttt.com";          // IFTTT Maker service hostname
String PATH_NAME  ="/trigger/lightsensor/with/key/dVFFne5femmXYIr0CeKYbYxZkShsgV7hadKGirvIEjo";  // IFTTT Maker webhook path
String queryString = "?value1=57&value2=25";      // Initial query parameters

void setup() {
  Serial.begin(9600);
  Serial.print("START");

  // Wait for a serial connection before proceeding
  while (!Serial);

  // Initialize WiFi connection
  WiFi.begin(ssid, pass);
  Wire.begin();

  // Attempt to establish a connection to the server
  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      // If connected to the server
      Serial.println("Connected to server");
      break;
    } else {
      // If the connection attempt failed
      Serial.println("Connection failed");
    }
    delay(500);
  }

  lightMeter.begin();
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel();

  // Construct the query string with the current light level
  queryString += "?value1=";
  queryString += lux;
  Serial.println(queryString);

  if (lux > 500) {
    // Make an HTTP request

    // Send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // End HTTP header

    while (client.connected()) {
      if (client.available()) {
        // Read an incoming byte from the server and print it to the serial monitor
        char c = client.read();
        Serial.print(c);
      }
    }

    // The server is disconnected, stop the client
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }

  // Reset the query string and wait for some time before the next loop iteration
  queryString = "";
  delay(3000);
}
