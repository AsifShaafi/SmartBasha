#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> // for handling json request sent from server

//const char* host = "http://192.168.0.101";
String host = "http://apphousebd.com";

//const char* ssid = "lenovo-300";
//const char* password = "aa11aa11@";
const char* ssid = "dewDrops_walton";
const char* password = "1122aa11";

// setting the pins for controlling the relay channels
int channel1 = D5, channel2 = D6, channel3 = D7, channel4 = D8;

//################# PIR Sensor ####################

int pirReceiverPin = D1;
long lowStartTime;
long waitingTime = 10000; // 10 sec
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;
bool ledIsOn = false, startCountdown = true;

int arduinoResetPin = D2;

//// fop sensor
//bool isSensorOn = false;

String request;
String channel_1_state, channel_2_state, channel_3_state, channel_4_state;


HTTPClient http;  //Declare an object of class HTTPClient

void setup() {

  Serial.begin(115200);

  pinMode(arduinoResetPin, OUTPUT);
  digitalWrite(arduinoResetPin, LOW);

  pinMode(D4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(channel1, OUTPUT);
  pinMode(channel2, OUTPUT);
  pinMode(channel3, OUTPUT);
  pinMode(channel4, OUTPUT);
  digitalWrite(channel1, LOW);
  digitalWrite(channel2, LOW);
  digitalWrite(channel3, LOW);
  digitalWrite(channel4, LOW);


  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);

  // if the wifi shield is connected to any network, then disconnecting it first, it's not mandatory but it's good practice.
  // if the shield is not connected to any network, then it will just ignore it
  WiFi.disconnect();
  delay(200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
   WiFi.mode(WIFI_STA);
  // beginning to connect to the network
  WiFi.begin(ssid, password);

  // checking if the shield is able to connect to network. It will try to connect untill it is connected.
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);

    digitalWrite(arduinoResetPin, HIGH);
  }

  Serial.println("");
  Serial.println("WiFi connected to ");
  Serial.println(WiFi.localIP());

}

void loop() {

  handleSensor();
  handleChannels();
  Serial.println("Client disonnected");
  Serial.println("");

  delay(3000);

}

void handleSensor() {

  //  ##################### Updating channel status with post() request to website/databse ######################
  //  ####################      only when the channel status is changed by PIR sensor      ######################

  //  checking if the pir sensor receiver pin is high, high means somekind of movement has been detected
  if (digitalRead(pirReceiverPin) == HIGH) {
    //    Serial.println("PIR receiver is high");
    // check if the led is already on or not, if on, then do nothing, if off, then turn it on and set the check to true
    if (!ledIsOn) {
      ledIsOn = true;
      startCountdown = true;
      Serial.println("LED is on");

      digitalWrite(channel3, HIGH);
      http.begin(host + "/wifi/api/updateChannelStates.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpPost = http.POST("submit=submit&state='led=on'&channel=channel_3");
      http.writeToStream(&Serial);
      http.end();
      delay(500);
    }

    if (!startCountdown && ledIsOn) {
      startCountdown = true;
      Serial.println("Count down reset");
    }
  }
  // if pir signal is low, that means movement is not detected, here we need to consider some situations
  /*
     if motion/movement is not detected, then we should not turn the led off immediately, as movement stop doesn't mean
     no body is present in room, so we should wait for some time before turning off the led.if we detect any movement in
     that between time, then we have to reset the timer when the signal is again low.
  */
  if (digitalRead(pirReceiverPin) == LOW) {

    //    Serial.println("PIR receiver is low");

    if (startCountdown && ledIsOn) {
      Serial.print("waiting time started: ");
      lowStartTime = millis();
      startCountdown = false;
      Serial.println(lowStartTime / 1000);
    }

    if (ledIsOn && millis() - lowStartTime >= waitingTime) {
      Serial.println("LED is off");
      digitalWrite(channel3, LOW);
      ledIsOn = false;

      Serial.println("sensor is off");
      http.begin(host + "/wifi/api/updateChannelStates.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpPost = http.POST("submit=submit&state='led=off'&channel=channel_3");
      http.writeToStream(&Serial);
      http.end();
    }
    else if (ledIsOn) {
      Serial.print("Time left: ");
      Serial.print(millis() - lowStartTime);
      Serial.print("   waiting time: " );
      Serial.println(waitingTime);
    }
  }
  //  if (digitalRead(D3) == HIGH) {
  //    if (digitalRead(channel3) == LOW && !isSensorOn) {
  //      digitalWrite(channel3, HIGH);
  //      isSensorOn = true;
  //      http.begin("http://192.168.0.101/wifiTest/api/updateChannelStates.php");
  //      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //      int httpPost = http.POST("submit=submit&state='LED=ON'&channel=channel_3");
  //      http.writeToStream(&Serial);
  //      http.end();
  //      delay(500);
  //    }
  //  }
  //  else if (digitalRead(D3) == LOW) { //&& digitalRead(channel3) == HIGH && isSensorOn) {
  //    Serial.println("sensor is off");
  //    isSensorOn = false;
  //    http.begin("http://192.168.0.101/wifiTest/api/updateChannelStates.php");
  //    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  //    int httpPost = http.POST("submit=submit&state='LED=OFF'&channel=channel_3");
  //    http.writeToStream(&Serial);
  //    http.end();
  //  }
}


void handleChannels() {

  //  ####################### Channel status with get() response from website/database ############################

  /*
      Requesting for / getting the current status of each channel from website
      the respond is in json format, so we 1st receive the data, get it by HTTPClient getString()
      then decode the json respond with ArduinoJson and appling the status of each channel in wifi shield
  */

  //  http.begin("http://smartbasha.000webhostapp.com/api/getChannelStates.php");  //Specify request destination
  http.begin(host + "/wifi/api/getChannelStates.php");  //Specify request destination
  //  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.GET();                                                                  //Send the request

  if (httpCode > 0) { //Check the returning code

    request = http.getString();   //Get the request response payload
    Serial.println(request);                     //Print the response payload

    // setting up ArduinoJson
    DynamicJsonBuffer jsonBuffer;

    if (request.indexOf("failed") == -1) {

      String jsonData = request.substring(request.indexOf("{"), request.indexOf("}") + 1);
      Serial.print("Json data is:");
      Serial.println(jsonData);

      // decoding the json data
      JsonObject& receivedData = jsonBuffer.parseObject(jsonData);
      if (!receivedData.success()) {
        Serial.println("Parsing failed");
      }
      else {
        String currentState = receivedData["channel_1"];
        channel_1_state = currentState;

        String currentState2 = receivedData["channel_2"];
        channel_2_state = currentState2;

        String currentState3 = receivedData["channel_3"];
        channel_3_state = currentState3;

        String currentState4 = receivedData["channel_4"];
        channel_4_state = currentState4;
      }
    }
    else {
      Serial.println("Data fetching failed");
    }
  }
  else {
    Serial.print("No payload. Error Code: ");
    Serial.println(httpCode);
    Serial.print("Error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();   //Close connection
  // Check if a client has connected

  Serial.print("Channel 1 state: ");
  Serial.println(channel_1_state);

  Serial.print("Channel 2 state: ");
  Serial.println(channel_2_state);

  Serial.print("Channel 3 state: ");
  Serial.println(channel_3_state);

  Serial.print("Channel 4 state: ");
  Serial.println(channel_4_state);

  // Match the request
  if (channel_1_state == "on") {
    digitalWrite(channel1, HIGH);
  }
  else if (channel_1_state == "off") {
    digitalWrite(channel1, LOW);
  }

  if (channel_2_state == "on") {
    digitalWrite(channel2, HIGH);
  }
  else if (channel_2_state == "off") {
    digitalWrite(channel2, LOW);
  }

  if (channel_3_state == "on") {
    digitalWrite(channel3, HIGH);
  }
  else if (channel_3_state == "off") {
    digitalWrite(channel3, LOW);
  }

  if (channel_4_state == "on") {
    digitalWrite(channel4, HIGH);
  }
  else if (channel_4_state == "off") {
    digitalWrite(channel4, LOW);
  }

}

