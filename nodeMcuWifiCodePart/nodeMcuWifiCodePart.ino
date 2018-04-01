#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> // for handling json request sent from server

//############# WIFI shield connection ################
//String host = "http://192.168.0.101";
String host = "http://apphousebd.com";

// change this according to own wifi-router settings
const char* ssid = "lenovo-300";
const char* password = "aa11aa11@";

//################# Channel control ###################
// setting the pins for controlling the relay channels
int channel1 = D5, channel2 = D6, channel3 = D7, channel4 = D8;

// bool values to detect if any movement was found after the led was turn on by user using android app
bool foundUserMovement[4];
bool countdownStated[4];
int countDownStartValue[4];

int timeout = 15000;

//################# PIR Sensor #####################

int pirReceiverPin[4] = {D1, D2, D3, D4};
long lowStartTime[4];
long waitingTime = 10000; // 10 sec
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;
bool ledIsOn[4] = {false}, startCountdown[4] = {true};

//############### HTTP request ##################
// variables for handling the server response
String request;
String channel_1_state, channel_2_state, channel_3_state, channel_4_state;

HTTPClient http;  //Declare an object of class HTTPClient

//#################################### Main logical Coding ############################################
void setup() {

  Serial.begin(115200);

  // using the built-in led of the shield to see the status of the wifi connection, if it's blinking means wifi isn't connected,
  // but it's trying to connect the wifi, if the led is on means shield is connected with the wifi network and ready to use
  // built-in led is active low
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

  // setting the pins of the pir sensor signal receiver
  for (int i = 0 ; i < 4; i++) {
    pinMode(pirReceiverPin[i], INPUT);
    digitalWrite(pirReceiverPin[i], LOW);
  }

  // setting this pin for output just to turn the default led on, by which we can show the user that wifi shield is turned on
  pinMode(D4, OUTPUT);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);

  // if the wifi shield is connected to any network, then disconnecting it first, it's not mandatory but it's good practice.
  // if the shield is not connected to any network, then it will just ignore it
  WiFi.disconnect();
  delay(200);

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

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

  for (int i = 0 ; i < 4 ; i++) {

    String currentChannel;
    int currentChannelPin;
    switch (i) {
      case 0:
        currentChannel = "channel_1";
        currentChannelPin = channel1;
        break;
      case 1:
        currentChannel = "channel_2";
        currentChannelPin = channel2;
        break;
      case 2:
        currentChannel = "channel_3";
        currentChannelPin = channel3;
        break;
      case 3:
        currentChannel = "channel_4";
        currentChannelPin = channel4;
        break;
      default:
        currentChannel = "channel_1";
        currentChannelPin = channel1;
    }
    /*
        Serial.print("Checking channel: ");
        Serial.println(currentChannel);

        Serial.print("channel pin state: ");
        Serial.println(digitalRead(pirReceiverPin[i]));
    */

    //  checking if the pir sensor receiver pin is high, high means somekind of movement has been detected
    if (digitalRead(pirReceiverPin[i]) == HIGH) {
      //    Serial.println("PIR receiver is high");
      // check if the led is already on or not, if on, then do nothing, if off, then turn it on and set the check to true
      if (!ledIsOn[i]) {
        ledIsOn[i] = true;
        startCountdown[i] = true;
        //        Serial.println("LED is on");

        digitalWrite(currentChannelPin, HIGH);
        http.begin(host + "/wifi/api/updateChannelStates.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpPost = http.POST("submit=submit&state='led=on'&channel=" + currentChannel);
        http.writeToStream(&Serial);
        http.end();
        delay(500); // wating for some time for the database to get updated, so that we get the latest data in the immediate request
      }

      if (!startCountdown[i] && ledIsOn[i]) {
        startCountdown[i] = true;
        Serial.println("Count down reset");
      }
    }
    // if pir signal is low, that means movement is not detected, here we need to consider some situations
    /*
       if motion/movement is not detected, then we should not turn the led off immediately, as movement stop doesn't mean
       no body is present in room, so we should wait for some time before turning off the led.if we detect any movement in
       that between time, then we have to reset the timer when the signal is again low.
    */
    if (digitalRead(pirReceiverPin[i]) == LOW) {

      //    Serial.println("PIR receiver is low");

      if (startCountdown[i] && ledIsOn[i]) {
        Serial.print("waiting time started for : ");
        Serial.print(currentChannel);
        Serial.print(" time: ");
        lowStartTime[i] = millis();
        startCountdown[i] = false;
        Serial.println(lowStartTime[i] / 1000);
      }

      /* checking for the delay: when the pir sensor is low it means no more movement is detected, so led should be off. but we will wait for
          for a certain amount of time before turning off the led to confirm that no body is present in the room
      */
      if (ledIsOn[i] && millis() - lowStartTime[i] >= waitingTime) {
        Serial.print("LED is off for channel: ");
        Serial.println(currentChannel);
        digitalWrite(currentChannelPin, LOW);
        ledIsOn[i] = false;

        Serial.print("sensor is off, channel: ");
        Serial.println(currentChannel);
        http.begin(host + "/wifi/api/updateChannelStates.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpPost = http.POST("submit=submit&state='led=off'&channel=" + currentChannel);
        //        http.writeToStream(&Serial);
        http.end();
      }
      else if (ledIsOn[i]) {
        Serial.print("Time left for channel ");
        Serial.print(currentChannel);
        Serial.print(" is: ");
        Serial.print(millis() - lowStartTime[i]);
        Serial.print("   waiting time: " );
        Serial.println(waitingTime);
      }
    }

  }
}


void handleChannels() {

  //  ####################### Channel status with get() response from website/database ############################

  /*
      Requesting for / getting the current status of each channel from website
      the respond is in json format, so we 1st receive the data, get it by HTTPClient getString()
      then decode the json respond with ArduinoJson and appling the status of each channel in wifi shield
  */
  http.begin(host + "/wifi/api/getChannelStates.php");  //Specify request destination
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.GET(); //Send the request in GET

  if (httpCode > 0) { //Check the returning code

    request = http.getString();   //Get the request response payload
    //Serial.println(request);                     //Print the response payload

    // setting up ArduinoJson
    DynamicJsonBuffer jsonBuffer;

    if (request.indexOf("failed") == -1) {

      String jsonData = request.substring(request.indexOf("{"), request.indexOf("}") + 1);
      //Serial.print("Json data is:");
      //Serial.println(jsonData);

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
  /*
    Serial.print("Channel 1 state: ");
    Serial.println(channel_1_state);

    Serial.print("Channel 2 state: ");
    Serial.println(channel_2_state);

    Serial.print("Channel 3 state: ");
    Serial.println(channel_3_state);

    Serial.print("Channel 4 state: ");
    Serial.println(channel_4_state);
  */
  // Match the request
  if (channel_1_state == "on") {
    digitalWrite(channel1, HIGH);

    /*
       if the led was turned on from android app, and no movement was found in room, then start countdown
       this countdown is for warning the user about the fact that light is on, but no one is in room.
       when countdown ends a notification will be sent to user, but if any movement is found during the countdown,
       no notification will be sent, as led will auto turn off after the movement is gone
    */
    if (!ledIsOn[0] && !countdownStated[0] && !foundUserMovement[0]) {
      countdownStated[0] = true;
      countDownStartValue[0] = millis();
    }
  }
  else if (channel_1_state == "off") {
    digitalWrite(channel1, LOW);

    foundUserMovement[0] = false;
    countdownStated[0] = false;
  }

  if (channel_2_state == "on") {
    digitalWrite(channel2, HIGH);
    if (!ledIsOn[1] && !countdownStated[1] && !foundUserMovement[1]) {
      countdownStated[1] = true;
      countDownStartValue[1] = millis();
    }
  }
  else if (channel_2_state == "off") {
    digitalWrite(channel2, LOW);
    foundUserMovement[1] = false;
    countdownStated[1] = false;
  }

  if (channel_3_state == "on") {
    digitalWrite(channel3, HIGH);
    if (!ledIsOn[2] && !countdownStated[2] && !foundUserMovement[2]) {
      countdownStated[2] = true;
      countDownStartValue[2] = millis();
    }
  }
  else if (channel_3_state == "off") {
    digitalWrite(channel3, LOW);
    foundUserMovement[2] = false;
    countdownStated[2] = false;
  }

  if (channel_4_state == "on") {
    digitalWrite(channel4, HIGH);
    if (!ledIsOn[3] && !countdownStated[3] && !foundUserMovement[3]) {
      countdownStated[3] = true;
      countDownStartValue[3] = millis();
    }
  }
  else if (channel_4_state == "off") {
    digitalWrite(channel4, LOW);
    foundUserMovement[3] = false;
    countdownStated[3] = false;
  }


  /// handling user movement not detected
  for (int i = 0 ; i < 4; i++) {

    String ch;
    switch (i) {
      case 0:
        ch = "channel 1";
        break;
      case 1:
        ch = "channel 2";
        break;
      case 2:
        ch = "channel 3";
        break;
      case 3:
        ch = "channel 4";
        break;
      default:
        ch = "channel 1";

    }

    Serial.print("Checking for channel timeout: ");
    Serial.println(i + 1);
    Serial.print("check value: ");
    Serial.print(foundUserMovement[i]);
    Serial.print(" :: ");
    Serial.println(countdownStated[i]);
    if (!foundUserMovement[i] && countdownStated[i]) {
      if (millis() - countDownStartValue[i] >= timeout) {
        Serial.println("time out for channel 1");
        countdownStated[i] = false; foundUserMovement[0] = true;
        http.begin(host + "/wifi/api/sendUserWarning.php");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpPost = http.POST("channel=" + ch);
        http.writeToStream(&Serial);
        http.end();
      }
      else {
        Serial.print("wating for time out for channel : ");
        Serial.println(millis() - countDownStartValue[0]);
      }
    }

  }

}

