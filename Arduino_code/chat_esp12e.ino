#include <ESP8266WiFi.h>

//wificredentials
const char* ssid = "ssid";
const char* password = "pwd";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
String device;
String stat;
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(D0, 0);
  digitalWrite(D1, 0);

  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  int sensorValue = analogRead(A0);
  String m;
  //API for device one off
  if (req.indexOf("/D0/0") != -1){
    device="fan";
    stat = "Off";
    digitalWrite(D0, 0);
  }
  //API for device one on
  else if (req.indexOf("/D0/1") != -1){
    device="fan";
    stat = "On";
    digitalWrite(D0, 1);
  }
  //API for device two off
  else if (req.indexOf("/D1/0") != -1){
    device="light";
    stat = "Off";
    digitalWrite(D1, 0);
  }
  //API for device two off
  else if (req.indexOf("/D1/1") != -1){
    device="light";
    stat = "On";
    digitalWrite(D1, 1);
  }
  //API for reading sensor data
  else if (req.indexOf("/A0") != -1){
    device="Sensor";
    stat = sensorValue;
  }
  else {
    Serial.println("invalid request");
    client.stop();
    m = "Invalid Request";
  }

  
  
  client.flush();

  // Prepare the response

  // Send the response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("");
   
  //send json response
  client.print("{\""+device+"\": \""+stat+"\"}");
  
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
