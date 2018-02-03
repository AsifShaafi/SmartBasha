
//################# Relay ##########################
// arduino pin assign for relay
int channel1 = 7, channel2 = 6, channel3 = 5, channel4 = 4;


//################# WiFi Shield ####################
// arduino pin assign for wifi shield control input
/*
    as channel_1 representing pin in wifi shield is D5,
    channel_2 pin = D6, channel_3 pin = D7, channel_4 pin = D8
*/
int wifiD5 = 11, wifiD6 = 10, wifiD7 = 9, wifiD8 = 8;


//################# PIR Sensor ####################

int pirReceiverPin = 13, pirLedDisplayPin = 12;
long lowStartTime;
long waitingTime = 10000; // 10 sec
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;    
bool ledIsOn = false, startCountdown = true;

void setup() {
  Serial.begin(9600);

  // setting the pins which will get the input from wifi shield to decide which relay pin to activate/deactivate
  pinMode(wifiD5, INPUT);
  pinMode(wifiD6, INPUT);
  pinMode(wifiD7, INPUT);
  pinMode(wifiD8, INPUT);

  // setting the pins which will output the correct voltage/signal to relay according to the wifi signal
  pinMode(channel1, OUTPUT);
  pinMode(channel2, OUTPUT);
  pinMode(channel3, OUTPUT);
  pinMode(channel4, OUTPUT);

  // setting the pir sensor signal receiver pin and the pin to show led state according to the pir sensor signal
//  pinMode(pirReceiverPin, INPUT);
//  digitalWrite(pirReceiverPin, LOW);
//  pinMode(pirLedDisplayPin, OUTPUT);
//  digitalWrite(pirLedDisplayPin, LOW);
//
//  //give the sensor some time to calibrate
//  Serial.print("calibrating sensor ");
//    for(int i = 0; i < calibrationTime; i++){
//      Serial.print(".");
//      delay(1000);
//      }
//    Serial.println(" done");
//    Serial.println("SENSOR ACTIVE");
//    delay(50);
}

void loop() {

  // if pin state is low, that means the signal is off, so deactivate the relay channel
  /*
     here what we are doing is checking the wifi receiving pins to get the pin state. if we send high signal from wifi, that means
     we want that perticular channel to be opne/on. so if we find the receiver pin state is high, then we will send high signal
     to the coresponding channel pin and vice versa.

     digitalRead(pin) returns HIGH or LOW, according to the pin state
  */
  //
  //  if (digitalRead(wifiD5) == HIGH){
  //    digitalWrite(channel1, HIGH);
  //  }
  //  else {
  //    digitalWrite(channel1, LOW);
  //  }
  //
  digitalWrite(channel1, digitalRead(wifiD5));
  digitalWrite(channel2, digitalRead(wifiD6));
  digitalWrite(channel3, digitalRead(wifiD7));
  digitalWrite(channel4, digitalRead(wifiD8));
//
//  //  checking if the pir sensor receiver pin is high, high means somekind of movement has been detected
//  if (digitalRead(pirReceiverPin) == HIGH) {
////    Serial.println("PIR receiver is high");
//    // check if the led is already on or not, if on, then do nothing, if off, then turn it on and set the check to true
//    if (!ledIsOn) {
//      digitalWrite(pirLedDisplayPin, HIGH);
//      ledIsOn = true;
//      startCountdown = true;
//      Serial.println("LED is on");
//    }
//
//    if(!startCountdown && ledIsOn) {
//      startCountdown = true;
//      Serial.println("Count down reset");
//    }
//  }
//  // if pir signal is low, that means movement is not detected, here we need to consider some situations
//  /*
//     if motion/movement is not detected, then we should not turn the led off immediately, as movement stop doesn't mean
//     no body is present in room, so we should wait for some time before turning off the led.if we detect any movement in
//     that between time, then we have to reset the timer when the signal is again low.
//  */
//  if (digitalRead(pirReceiverPin) == LOW) {
//
////    Serial.println("PIR receiver is low");
//
//    if (startCountdown && ledIsOn) {
//      Serial.print("waiting time started: ");
//      lowStartTime = millis();
//      startCountdown = false;
//      Serial.println(lowStartTime / 1000);
//    }
//
//    if (ledIsOn && millis() - lowStartTime >= waitingTime) {
//      Serial.println("LED is off");
//      digitalWrite(pirLedDisplayPin, LOW);
//      ledIsOn = false;
//    }
//    else if(ledIsOn) {
//      Serial.print("Time left: ");
//      Serial.print(millis() - lowStartTime);
//      Serial.print("   waiting time: " );
//      Serial.println(waitingTime);   
//    }
//  }

}
