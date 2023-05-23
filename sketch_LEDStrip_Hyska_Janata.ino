#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    14
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

#define BUTTON1  2
int button1state = 0;

#define BUTTON2  3
int button2state = 0;

int p1count = 0;
int p2count = 0;

int position = 7;

bool p1press = false;
bool p2press = false;

unsigned long goalTime = 0;

unsigned long p1color = CRGB::Red;
unsigned long p2color = CRGB::Blue;
unsigned long drawcolor = CRGB::Yellow;






void setup() {
  Serial.begin(9600);
  delay( 3000 );
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
  initializeGame();
}

void loop() {
  button1state = digitalRead(BUTTON1);
  button2state = digitalRead(BUTTON2);

  if(button1state == HIGH){// letting of button makes it pressable again (anti-hold mechanism)
    p1press = false;
  }
  else
  if(p1press == false){// press if it's not pressed down
    p1count++;
    Serial.print("P1: ");
    Serial.println(p1count);
    p1press = true;
  }
  
  if(button2state == HIGH){
    p2press = false;
  }
  else
  if(p2press == false){
    p2count++;
    Serial.print("P2: ");
    Serial.println(p2count);
    p2press = true;
  }

  if(p2count > (p1count + 1) ) { // moving the color of leds by one led
    position = position -1;
    Serial.print("Position: ");
    Serial.println(position);
    p1count = 0;
    p2count = 0;
  }
  if(p1count > (p2count + 1) ) { // moving the color of leds by one led to the other side
    position = position + 1;
    Serial.print("Position: ");
    Serial.println(position);
    p1count = 0;
    p2count = 0;
  }
  if(millis() > goalTime){ // has 15 seconds passed??
    if(position == 7) zobrazLED(1983); // draw
    else if(position > 7) zobrazLED(14); // player1 wins
    else zobrazLED(0); // player 2 wins
  }
  zobrazLED(position);
}

void initializeGame(){
  goalTime = millis() + 15000;
  Serial.println("Time reset ^^");
  position = 7;
  p1count = 0;
  p2count = 0;
}


void zobrazLED(int pos){
  //draw animation
  if (pos == 1983){
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black; // all goes black
    }
    FastLED.show();
    //two leds running across strip
    for(int i = 0; i < 2; i++){
      int dot2 = NUM_LEDS-1;
      for(int dot = 0; dot < NUM_LEDS; dot++) { 
        leds[dot] = drawcolor;
        leds[dot2] = drawcolor;
        FastLED.show();
        // clear this led for the next time around the loop
        leds[dot] = CRGB::Black;
        leds[dot2] = CRGB::Black;
        dot2--;
        delay(100);
      }
    }
    //two blinks
    for(int j = 0; j < 2; j++) { 
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = drawcolor;
      }
      FastLED.show();
      delay(500);
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }
    initializeGame();
  }
  if(pos == 14){
    //player1 win animation
    for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
      leds[dot] = p1color;
      FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(100);
    }
    for(int dot = NUM_LEDS-1; dot > 0; dot--) { 
      leds[dot] = p1color;
      FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(100);
    }
    for(int j = 0; j < 2; j++) { 
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = p1color;
      }
      FastLED.show();
      delay(500);
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }
    initializeGame();
  }
  if(pos == 0){
    //player2 win animation
    for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    for(int dot = NUM_LEDS-1; dot > 0; dot--) { 
      leds[dot] = p2color;
      FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(100);
    }
    for(int dot = 0; dot < NUM_LEDS; dot++) { 
      leds[dot] = p2color;
      FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(100);
    }
    for(int j = 0; j < 2; j++) { 
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = p2color;
      }
      FastLED.show();
      delay(500);
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }
    initializeGame();
  }
  else{
    //normal game state
    for (int i = 0; i < NUM_LEDS; i++){
      if(i < pos){
        leds[i] = p1color; FastLED.show();
      }
      else{
        leds[i] = p2color; FastLED.show();
      }
    }
  }
  

  FastLED.show();
}
