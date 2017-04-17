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
  shiftOut(0x00);
  delay(1000);

  leftScore = 0;
}

void loop() {
/*
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
*/

  for (int j = 0; j < 10; j++) {
    shiftOut(dataArray[j]);
    delay(1000);
  }

  // All off for 3 seconds
  shiftOut(0x00);
  delay(3000);
}

/*
void incLeftScore(){
    data = (data + 1) % 10; // increment 1, range is 0-9 inc
    Serial.println(data);
}
*/


void shiftOut(byte score) {
  digitalWrite(LATCHPIN, 0);
  
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (byte i=7; i>=0; i--)  {
    digitalWrite(CLOCKPIN, 0);

    //if the value passed to score and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
//    if ( score & (1<<i) ) {
//      pinState= 1;
//    }
//    else {  
//      pinState= 0;
//    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(DATAPIN, (score & (1<<i))?1:0);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(CLOCKPIN, 1);
  }

  //stop shifting
  digitalWrite(LATCHPIN, 1);
}
