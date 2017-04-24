/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * TPIC6B596 shift register 
 * LEDs attached to each of the outputs of the shift register `
 */
#define DATAPIN 13      // SER IN (Serial In) on 596
#define LATCHPIN 12     // RCK (Register Clock) on 596
#define CLOCKPIN 11     // SRCK (Shift-Register Clock) on 596
#define REDBUTTON 10     // Red button
#define BLUEBUTTON 9    // Blue button
#define DEBOUNCE 500    // Min delay between button presses

//holders for infromation you're going to pass to shifting function
byte redScore, blueScore;
byte digits[10];

// prevent button debounce
long gameTimer, prevRedPress, prevBluePress;

void setup()
{
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
  digits[0] = 0b1111110;
  digits[1] = 0b0001100;
  digits[2] = 0b0110111;
  digits[3] = 0b0011111;
  digits[4] = 0b0001101;
  digits[5] = 0b0111011;
  digits[6] = 0b1111011;
  digits[7] = 0b0001110;
  digits[8] = 0b1111111;
  digits[9] = 0b0111111;
  digits[100] = 0x00;
  digits[0xFF] = 0x00;

  // Reset game scores
  redScore = 0;
  blueScore = 0;

  // Reset button timers
  prevRedPress = millis();
  prevBluePress = prevBluePress;
  gameTimer = prevBluePress;

  // all off for 1 second
  pushLED(100);
  pushLED(100);
  delay(1000);
  pushLED(redScore);
  pushLED(blueScore);
}

void loop()
{
  // Check if red button is being pressed
  if(prevRedPress + DEBOUNCE < millis() && !digitalRead(REDBUTTON)){
    redScore = (redScore + 1) % 6; // increment 1, range is 0-5 inc
    if(redScore==0 && blueScore==0){
      // If both teams change to a zero score, reset the game.
      // all on for 1 second
      gameTimer = millis();
      pushLED(0xFF);
      pushLED(0xFF);
      delay(1000);
    }
    // Either way, push the new scores
    Serial.println(redScore);
    pushLED(redScore);
    pushLED(blueScore);
    // reset red button timer
    prevRedPress = millis();
  }

    // Check if blue button is being pressed
  if(prevBluePress + DEBOUNCE < millis() && !digitalRead(BLUEBUTTON)){
    blueScore = (blueScore + 1) % 6; // increment 1, range is 0-5 inc
    if(redScore==0 && blueScore==0){
      // If both teams change to a zero score, reset the game.
      // all on for 1 second
      gameTimer = millis();
      pushLED(0xFF);
      pushLED(0xFF);
      delay(1000);
    }
    // Either way, push the new scores
    Serial.println(blueScore);
    pushLED(redScore);
    pushLED(blueScore);
    // reset blue button timer
    prevBluePress = millis();
  }

  // Blink scoreboard when game time runs out
  if(millis() - gameTimer > 900000){
    // 15 minute games, after that blink the scores every half second
    if(millis() % 1000 > 500){
      pushLED(100);
      pushLED(100);
    } else {
      pushLED(redScore);
      pushLED(blueScore);
    }
  }
}

void pushLED(byte score)
{
  Serial.println(score);
  digitalWrite(LATCHPIN, 0);
  //NOTICE WE ARE COUNTING DOWN
  for (int i=7; i>=0; i--){
    digitalWrite(DATAPIN, digits[score] & (1<<i));
    //register shifts bits on upstroke of clock pin
    digitalWrite(CLOCKPIN, 0);
    digitalWrite(CLOCKPIN, 1);
  }
  //stop shifting
  digitalWrite(LATCHPIN, 1);
}
