/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */
#define LATCHPIN 8      //Pin connected to ST_CP of 74HC595
#define CLOCKPIN 12     //Pin connected to SH_CP of 74HC595
#define DATAPIN 11      //Pin connected to DS of 74HC595
#define DEFAULTLED 13   //Use default led on pin 13 for troubleshooting
#define DEBOUNCE 500    // Min delay between button presses

//holders for infromation you're going to pass to shifting function
byte leftScore;
byte dataArray[10];

//Buttons
const byte ButtonLeftPin = 2;
const byte ButtonRightPin = 3;

// prevent button debounce
long prevLeftPress = millis();

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(LATCHPIN, OUTPUT);
  pinMode(DEFAULTLED, OUTPUT);
  pinMode(ButtonLeftPin, INPUT_PULLUP);
//  pinMode(ButtonLeftPin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(ButtonLeftPin), incLeftScore, FALLING);
  Serial.begin(9600);

  // The binary patterns for each decimal digit
  dataArray[0] = 0b01111110;
  dataArray[1] = 0b00001100;
  dataArray[2] = 0b00110111;
  dataArray[3] = 0b00011111;
  dataArray[4] = 0b01001101;
  dataArray[5] = 0b01011011;
  dataArray[6] = 0b01111011;
  dataArray[7] = 0b00001110;
  dataArray[8] = 0b01111111;
  dataArray[9] = 0b01011111;

  // all on for 1 second
  digitalWrite(LATCHPIN, 0);
  shiftOut(DATAPIN, CLOCKPIN, 0x00);
  digitalWrite(LATCHPIN, 1);
  delay(1000);

  leftScore = 0;
}

void loop() {
  // Check if left button is being pressed
  if(prevLeftPress + DEBOUNCE < millis() && !digitalRead(ButtonLeftPin)){
    leftScore = (leftScore + 1) % 10; // increment 1, range is 0-9 inc
    Serial.println(leftScore);
    prevLeftPress = millis();
  }

  digitalWrite(LATCHPIN, 0);
  shiftOut(DATAPIN, CLOCKPIN, leftScore);
  digitalWrite(LATCHPIN, 1);
  delay(100);

  /*
  for (int j = 0; j < 10; j++) {
    //load the light sequence you want from array
    data = dataArray[j];
    //ground LATCHPIN and hold low for as long as you are transmitting
    digitalWrite(LATCHPIN, 0);
    shiftOut(DATAPIN, CLOCKPIN, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(LATCHPIN, 1);
    delay(1000);
  }

  // all off for 3 seconds
  digitalWrite(LATCHPIN, 0);
  shiftOut(DATAPIN, CLOCKPIN, 0x00);
  digitalWrite(LATCHPIN, 1);
  delay(3000);
  */
}

/*
void incLeftScore(){
    data = (data + 1) % 10; // increment 1, range is 0-9 inc
    Serial.println(data);
}
*/


// the heart of the program
void shiftOut(int myDATAPIN, int myCLOCKPIN, byte score) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low
  score = dataArray[score];

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myCLOCKPIN, OUTPUT);
  pinMode(myDATAPIN, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDATAPIN, 0);
  digitalWrite(myCLOCKPIN, 0);

  //for each bit in the byte score�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myCLOCKPIN, 0);

    //if the value passed to score and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( score & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDATAPIN, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myCLOCKPIN, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDATAPIN, 0);
  }

  //stop shifting
  digitalWrite(myCLOCKPIN, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(LATCHPIN, 0);
  shiftOut(DATAPIN, CLOCKPIN, 0);
  shiftOut(DATAPIN, CLOCKPIN, 0);
  digitalWrite(LATCHPIN, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(LATCHPIN, 0);
    shiftOut(DATAPIN, CLOCKPIN, 255);
    shiftOut(DATAPIN, CLOCKPIN, 255);
    digitalWrite(LATCHPIN, 1);
    delay(d);
    digitalWrite(LATCHPIN, 0);
    shiftOut(DATAPIN, CLOCKPIN, 0);
    shiftOut(DATAPIN, CLOCKPIN, 0);
    digitalWrite(LATCHPIN, 1);
    delay(d);
  }
}
