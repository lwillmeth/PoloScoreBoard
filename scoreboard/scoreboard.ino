/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */
#define LATCHPIN 13      //Pin connected to ST_CP of 74HC595
#define CLOCKPIN 12     //Pin connected to SH_CP of 74HC595
#define DATAPIN 11      //Pin connected to DS of 74HC595
#define DEBOUNCE 500    // Min delay between button presses
#define REDBUTTON 10
#define BLUEBUTTON 9

//holders for infromation you're going to pass to shifting function
byte redScore, blueScore;
byte digits[10];


// prevent button debounce
long gameTimer, prevRedPress, prevBluePress;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  pinMode(REDBUTTON, INPUT_PULLUP);
  pinMode(BLUEBUTTON, INPUT_PULLUP);

  digitalWrite(LATCHPIN, LOW);
  digitalWrite(CLOCKPIN, LOW);
  digitalWrite(DATAPIN, LOW);

  Serial.begin(9600);
  Serial.println("Bike polo scoreboard starting up..");

  // The binary patterns for each decimal digit
  digits[0] = 0b11111110;
  digits[1] = 0b00001100;
  digits[2] = 0b01110111;
  digits[3] = 0b00111111;
  digits[4] = 0b10001101;
  digits[5] = 0b10111011;
  digits[6] = 0b11111011;
  digits[7] = 0b00001110;
  digits[8] = 0b11111111;
  digits[9] = 0b10111111;

  // Reset game scores
  redScore = 0;
  blueScore = 0;

  // Reset button timers
  prevRedPress = millis();
  prevBluePress = prevBluePress;
  gameTimer = prevBluePress;

  // all on for 1 second
  pushLED(0xFF);
  delay(1000);
  pushLED(redScore);
  //pushLED(blueScore);
}

void loop() {

  // Check if left button is being pressed
  if(prevRedPress + DEBOUNCE < millis() && !digitalRead(REDBUTTON)){
    redScore = (redScore + 1) % 10; // increment 1, range is 0-9 inc
    if(redScore==0x00 && blueScore==0x00){
      // If both teams change to a zero score, reset the game.
      // all on for 1 second
      pushLED(0xFF);
      delay(1000);
      // all off for 1 second
      pushLED(0x00);
      delay(1000);
    }
    // Either way, push the new scores
    Serial.println(redScore);
    pushLED(redScore);
    //pushLED(blueScore);
    // reset red button timer
    prevRedPress = millis();
  }

  // Blink scoreboard when game time runs out
  if(millis() - gameTimer > 900000){
    // 15 minute games, after that blink the scores every half second
    if(millis() % 1000 > 500){
      pushLED(0x00);
    } else {
      pushLED(redScore);
      //pushLED(blueScore);
    }
  }

//  for (int j = 0; j < 10; j++) {
//    pushLED(digits[j]);
//    delay(1000);
//  }
//
//  // All off for 3 seconds
//  pushLED(0x00);
//  delay(1500);
//  pushLED(0xFF);
//  delay(1500);
}

/*
void incredScore(){
    data = (data + 1) % 10; // increment 1, range is 0-9 inc
    Serial.println(data);
}
*/


void pushLED(byte score) {
  Serial.println(score);
  digitalWrite(LATCHPIN, 0);
  //NOTICE WE ARE COUNTING DOWN
  for (int i=7; i>=0; i--){
    digitalWrite(DATAPIN, score & (1<<i));
    //register shifts bits on upstroke of clock pin
    digitalWrite(CLOCKPIN, 0);
    digitalWrite(CLOCKPIN, 1);
  }
  //stop shifting
  digitalWrite(LATCHPIN, 1);
}
