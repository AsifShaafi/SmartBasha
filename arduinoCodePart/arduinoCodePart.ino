
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
  
  digitalWrite(channel1, LOW);
  digitalWrite(channel2, LOW);
  digitalWrite(channel3, LOW);
  digitalWrite(channel4, LOW);
}

void loop() {

  // if pin state is low, that means the signal is off, so deactivate the relay channel
  /*
     here what we are doing is checking the wifi receiving pins to get the pin state. if we send high signal from wifi, that means
     we want that perticular channel to be opne/on. so if we find the receiver pin state is high, then we will send high signal
     to the coresponding channel pin and vice versa.

     digitalRead(pin) returns HIGH or LOW, according to the pin state
  */
  digitalWrite(channel1, digitalRead(wifiD5));
  digitalWrite(channel2, digitalRead(wifiD6));
  digitalWrite(channel3, digitalRead(wifiD7));
  digitalWrite(channel4, digitalRead(wifiD8));
}
