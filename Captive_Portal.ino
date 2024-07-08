//This is the code for PNT form submission 
//highlights are builtinLED of Nodemcu turn on when connected to wifi , the data user enters on the submission page gets printed on the google sheet date2 present on account adityasharmasheagaon

/* \
 This code sets up the NodeMCU as a WiFi access point (AP) and DNS server, which allows it to serve a captive portal. However, it doesn't provide internet access to the connected users. This is because the NodeMCU is acting as a standalone access point and not as a bridge to an existing WiFi network with internet access.

To provide internet access, you need to configure the NodeMCU to operate in both Access Point (AP) and Station (STA) mode simultaneously. Here’s how you can modify your code to allow the NodeMCU to connect to an existing WiFi network (providing internet access) while still serving the captive portal:

*/

#include <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClientSecureBearSSL.h>
  #include <time.h>
  #include <FS.h>
  
  const char* ssid = "aditya"; // Replace with your WiFi SSID
  const char* password = "adityasharma"; // Replace with your WiFi Password
  
  const byte DNS_PORT = 53;
  IPAddress apIP(172, 217, 28, 1);
  DNSServer dnsServer;
  ESP8266WebServer webServer(80);
  
  const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbztnDJyhiLgCUoEEAii6DfOfj-YCwGQQ1Vl1EPjDNVsMY4-LoQy8CwptFzhnzj5YfjX7g/exec"; // Replace with your URL
  
  String responseHTML = R"rawliteral(
  <!DOCTYPE html><html lang='en'>
  <head>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>
      <title>Captive Portal</title>
      <link href='https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap' rel='stylesheet'>
      <style>
          body { font-family: 'Roboto', sans-serif; background: linear-gradient(to right, #ff007a, #ff6a00); color: #333; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }
          .container { max-width: 400px; width: 100%; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 0 15px rgba(0, 0, 0, 0.2); transition: transform 0.3s, box-shadow 0.3s; }
          .container:hover { transform: scale(1.05); box-shadow: 0 0 25px rgba(0, 0, 0, 0.3); }
          h1 { text-align: center; color: #ff007a; }
          form { display: flex; flex-direction: column; }
          label { margin: 10px 0 5px; }
          input[type='text'] { padding: 10px; margin: 5px 0 15px; border-radius: 5px; border: 1px solid #ccc; background-color: #fafafa; font-size: 16px; }
          input[type='submit'] { padding: 10px; margin: 10px 0; background: linear-gradient(to right, #ff007a, #ff6a00); border: none; border-radius: 5px; color: #fff; cursor: pointer; font-size: 16px; transition: background 0.3s; }
          input[type='submit']:hover { background: linear-gradient(to right, #ff6a00, #ff007a); }
      </style>
      <script>
          function validateForm() {
              var contact = document.getElementById('contact').value;
              if (contact.length != 10 || !/^\d{10}$/.test(contact)) {
                  alert('Contact number must be exactly 10 digits.');
                  return false;
              }
              return true;
          }
      </script>
  </head>
  <body>
      <div class='container'>
          <h1>PNT Robotics</h1>
          <p>Please enter your details:</p>
          <form action='/submit' method='POST' onsubmit='return validateForm()'>
              <label for='name'>Name:</label>
              <input type='text' id='name' name='name' required>
              <label for='address'>Address:</label>
              <input type='text' id='address' name='address' required>
              <label for='contact'>Contact Number:</label>
              <input type='text' id='contact' name='contact' required>
              <input type='submit' value='Submit'>
          </form>
      </div>
  </body>
  </html>
  )rawliteral";
  
  String thankYouHTML = R"rawliteral(
  <!DOCTYPE html><html lang='en'>
  <head>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>
      <title>Thank You</title>
      <link href='https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap' rel='stylesheet'>
      <style>
          body { font-family: 'Roboto', sans-serif; background: linear-gradient(to right, #1b3153, #4fbdb4); color: #fff; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; text-align: center; }
          .container { max-width: 400px; width: 100%; padding: 20px; background-color: rgba(255, 255, 255, 0.2); border-radius: 10px; box-shadow: 0 0 15px rgba(0, 0, 0, 0.2); transition: transform 0.3s, box-shadow 0.3s; }
          .container:hover { transform: scale(1.05); box-shadow: 0 0 25px rgba(0, 0, 0, 0.3); }
          h1 { font-size: 2em; margin-bottom: 20px; color: #00d4ff; }
          p { font-size: 1.2em; color: #ffdf00; }
          img { max-width: 100%; height: auto; display: block; margin: 20px auto; }
          .button { background: #ffdf00; color: #333; padding: 10px 20px; text-decoration: none; border-radius: 5px; font-weight: bold; transition: background 0.3s; }
          .button:hover { background: #ffc700; color: #000; }
      </style>
  </head>
  <body>
      <div class='container'>
          <h1>Thank You for Your Response!</h1>
          <p>PNT Robotics</p>
          <img src='/image.jpg' alt='Image'>
          <a href='/' class='button'>Go Back</a>
      </div>
  </body>
  </html>
  )rawliteral";
  
  
  void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected to WiFi. IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, LOW); // Turn on the LED
  }
  
  void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // Turn off the LED initially
  
    if (!SPIFFS.begin()) {
      Serial.println("Failed to mount SPIFFS");
      return;
    }
  
    connectToWiFi(); // Connect to WiFi network
  
    configTime(0, 0, "pool.ntp.org"); // Configure NTP
  
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("PNT Robotics");
  
    dnsServer.start(DNS_PORT, "*", apIP);
  
    webServer.on("/", HTTP_GET, []() {
      webServer.send(200, "text/html", responseHTML);
    });
  
    webServer.on("/submit", HTTP_POST, []() {
      String name = webServer.arg("name");
      String address = webServer.arg("address");
      String contact = webServer.arg("contact");
  
      // Print the details to the Serial Monitor
      Serial.print("Name: ");
      Serial.println(name);
      Serial.print("Address: ");
      Serial.println(address);
      Serial.print("Contact Number: ");
      Serial.println(contact);
  
      // Get the current time
      time_t now = time(nullptr);
      struct tm* p_tm = localtime(&now);
  
      char dateBuffer[11];
      char timeBuffer[9];
  
      strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", p_tm);
      strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", p_tm);
  
      // Create a JSON object
      String jsonData = "{";
      jsonData += "\"date\":\"" + String(dateBuffer) + "\",";
      jsonData += "\"time\":\"" + String(timeBuffer) + "\",";
      jsonData += "\"name\":\"" + name + "\",";
      jsonData += "\"address\":\"" + address + "\",";
      jsonData += "\"contact\":\"" + contact + "\"";
      jsonData += "}";
  
      // Send data to Google Sheets
      WiFiClientSecure client;
      client.setInsecure(); // Use this with caution, consider using a valid certificate
      HTTPClient http;
  
      Serial.print("Connecting to ");
      Serial.println(googleScriptURL);
  
      http.begin(client, googleScriptURL);
      http.addHeader("Content-Type", "application/json");
  
      int httpResponseCode = http.POST(jsonData);
  
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
  
      http.end();
  
      // Send the thank you response back to the client
      webServer.send(200, "text/html", thankYouHTML);
    });
  
    webServer.on("/image.jpg", HTTP_GET, []() {
      File imageFile = SPIFFS.open("/image.jpg", "r");
      if (imageFile) {
        size_t imageSize = imageFile.size();
        uint8_t* imageData = new uint8_t[imageSize];
        imageFile.read(imageData, imageSize);
        imageFile.close();
        
        webServer.sendHeader("Content-Type", "image/jpeg");
        webServer.sendHeader("Content-Length", String(imageSize));
        webServer.send(200, "image/jpeg", (const char*)imageData, imageSize);
        delete[] imageData;
      } else {
        Serial.println("Failed to open image file");
        webServer.send(500, "text/plain", "Failed to open image file");
      }
    });
  
    webServer.onNotFound([]() {
      webServer.send(200, "text/html", responseHTML);
    });
  
    webServer.begin();
  }
  
  void loop() {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
