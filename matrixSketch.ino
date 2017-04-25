#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pin 6 is an analog pin on the NANO
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      100

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
const byte interruptPin = 2;
const int aggieBlue = pixels.Color(0,0,150);

// The LEDs are soldered together in a row
// this matrix defines them as a 2D array
// to make it easier to use.
const int matrix[10][10] = {
  {0,1,2,3,4,5,6,7,8,9},
  {19,18,17,16,15,14,13,12,11,10},
  {20,21,22,23,24,25,26,27,28,29},
  {39,38,37,36,35,34,33,32,31,30},
  {40,41,42,43,44,45,46,47,48,49},
  {59,58,57,56,55,54,53,52,51,50},
  {60,61,62,63,64,65,66,67,68,69},
  {79,78,77,76,75,74,73,72,71,70},
  {80,81,82,83,84,85,86,87,88,89},
  {99,98,97,96,95,94,93,92,91,90}
};

int state = 1;

void clearPixels(){
  for(int j = 0; j < 100; j++){
    pixels.setPixelColor(j, pixels.Color(0,0,0));
  }
}

// alternative flashing blue and left and red on right
int alternating = 0;
void blueRedFlash(){
  for(int i = 0; i < 10; i++){
    if(alternating == 1){
      for(int j = 5; j < 10; j++){
        pixels.setPixelColor(matrix[i][j], pixels.Color(150,0,0));
      }
      for(int j = 0; j < 5; j++){
        pixels.setPixelColor(matrix[i][j], pixels.Color(0,0,0));
      }
    }
    else{
      for(int j = 0; j < 5; j++){
        pixels.setPixelColor(matrix[i][j], pixels.Color(0,0,150));
      }
      for(int j = 5; j < 10; j++){
        pixels.setPixelColor(matrix[i][j], pixels.Color(0,0,0));
      }
    }
  }
  if(alternating == 1){
    alternating = 0;
  }
  else{
    alternating = 1;
  }
}

// a square appears in the middle and expands with
// every loop
int squareOutCount = 0;
void squareOut(){
  int a, b, k, m;
  
  switch(squareOutCount){
    case 0:
      a = 4;
      b = 4;
      k = 6;
      m = 6;
      break;
    case 1:
      a = 3;
      b = 3;
      k = 7;
      m = 7;
      break;
    case 2:
      a = 2;
      b = 2;
      k = 8;
      m = 8;
      break;
    case 3:
      a = 1;
      b = 1;
      k = 9;
      m = 9;
      break;
    case 4:
      a = 0;
      b = 0;
      k = 10;
      m = 10;
      break;
    default:
      a = 3;
      b = 3;
      k = 7;
      m = 7;
  }

  for(int i = a;i < k; i++){
    for(int j = b;j < m; j++){
      pixels.setPixelColor(matrix[i][j], aggieBlue);
    }
  }
  
  if(squareOutCount < 4){
    squareOutCount++;
  }
  else{
    squareOutCount = 0;
  }
}

void displayA(){
  // Left column
  for(int i = 0; i < 2; ++i){
    for(int j = 2; j < 10; j++){
       pixels.setPixelColor(matrix[i][j], aggieBlue);
    }
  }
  // Right Column
  for(int i = 8; i < 10; ++i){
    for(int j = 2; j < 10; j++){
       pixels.setPixelColor(matrix[i][j], aggieBlue);
    }
  }
  // Top
  for(int i = 2; i < 8; i++){
    for(int j = 0; j < 2; j++){
      pixels.setPixelColor(matrix[i][j], aggieBlue);
    }
  }
  // Middle Bar
  for(int i = 2; i < 8; i++){
    for(int j = 4; j < 6; j++){
      pixels.setPixelColor(matrix[i][j], aggieBlue);
    }
  }
  // The linkers
  pixels.setPixelColor(matrix[1][1], aggieBlue);
  pixels.setPixelColor(matrix[7][7], aggieBlue);
}

void changeState(){
  if(state < 2){
    state++;
  }
  else{
    state = 0;
  }
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  // This is the input pin that cycles the states
  // It's set on an interrupt so it's responsive
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeState, RISING);
  
  pixels.begin(); // This initializes the NeoPixel library.
}

// The loop clears all pixels and executes the
// pixels.show function with every loop
void loop() {
  clearPixels();
  switch(state){
    case 0:
      blueRedFlash();
      break;
     case 1:
      squareOut();
      break;
     case 2:
      displayA();
      break;
     default:
      blueRedFlash();
      break;
  }
  pixels.show();
  delay(delayval);
}


