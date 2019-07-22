// Include low level code for Adafruit color display (by Adafruit)
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Value vars
const int shape_options[] = { 0, 1, 2, 3, 4, 5 };
int shape_options_index = 0;
const int process_step[] = { 0, 1, 2, 3, 4 };
int process_step_index = 0;
int prev_x = 0;
int prev_y = 0;
boolean inverting;

// Time Tracking
const int time_between_updates = 25;
int last_update;

// Joystick pins
const int joy_x = A0;
const int joy_y = A1;
const int joy_btn = 2;

// Button pins
const int up_pin = 7;
const int down_pin = 6;
const int enter_pin = 5;

// Color LCD output pins
const int cs_pin = 10;
const int reset_pin = 9;
const int dc_pin = 8;

// Initialize the colorful LCD screen display
Adafruit_ST7735 tft = Adafruit_ST7735(cs_pin,  dc_pin, reset_pin);

void setup() {
  Serial.begin(9600);

  pinMode(up_pin, INPUT_PULLUP);
  pinMode(down_pin, INPUT_PULLUP);
  pinMode(enter_pin, INPUT_PULLUP);
  pinMode(joy_btn, INPUT_PULLUP);

  // Initialize the 1.44" tft ST7735S chip with a white background
  tft.initR(INITR_144GREENTAB);
  tft.fillScreen(ST7735_BLACK);

  last_update = millis();
}

void loop() {
  show_shape_option();
  shape_selection();
  shape_placement();

  if(process_step_index > 4 || process_step_index < 0) {
    process_step_index = 0;
  }
}

void show_shape_option() {
  tft.setCursor(17, 7);
  tft.setTextColor(ST7735_WHITE);
  
  switch(shape_options_index) {
    case 0:
      tft.print("Line - Verticle");
      break;
    case 1:
      tft.print("Line - Horizontal");
      break;
    case 2:
      tft.print("Square - Empty");
      break;
    case 3:
      tft.print("Square - Filled");
      break;
    case 4:
      tft.print("Circle - Empty");
      break;
    case 5:
      tft.print("Circle - Filled");
      break;
  }

  tft.drawLine(10, 17, 118, 17, ST7735_WHITE);
}

void shape_selection() {
  if(process_step_index != 0) return;
  boolean up = digitalRead(up_pin);
  if(up == LOW) {
    shape_options_index += 1;
    tft.fillScreen(ST7735_BLACK);
    delay(300);
  }

  boolean down = digitalRead(down_pin);
  if(down == LOW) {
    shape_options_index -= 1;
    tft.fillScreen(ST7735_BLACK);
    delay(300);
  }

  boolean enter = digitalRead(enter_pin);
  if(enter == LOW) {
    process_step_index += 1;
    tft.fillScreen(ST7735_BLACK);
    delay(300);
  }
  
  if(shape_options_index > 5 || shape_options_index < 0) {
    shape_options_index = 0;
  }
}

void shape_placement() {
  if(process_step_index != 1) return;
  
  int x_value = analogRead(joy_x);
  x_value = map(x_value, 0, 1023, 0, 128);

  int y_value = analogRead(joy_y);
  y_value = map(y_value, 0, 1023, 0, 128-20) + 20;

  int size = 30;

  if(prev_x != x_value || prev_y != y_value) {
    switch(shape_options_index) {
      case 0:
        tft.drawLine(prev_x, prev_y-size, prev_x, prev_y+size, ST7735_BLACK);
        break;
      case 1:
        tft.drawLine(prev_x-size, prev_y, prev_x+size, prev_y, ST7735_BLACK);
        break;
      case 2:
        tft.drawRect(prev_x-size, prev_y-size, prev_x, prev_y, ST7735_BLACK);
        break;
      case 3:
       tft.fillRect(prev_x-size, prev_y-size, prev_x, prev_y, ST7735_BLACK);
        break;
      case 4:
        tft.drawCircle(prev_x, prev_y, size, ST7735_BLACK);
        break;
      case 5:
        tft.fillCircle(prev_x, prev_y, size, ST7735_BLACK);
        break;
    }
    
    prev_x = x_value;
    prev_y = y_value;

    switch(shape_options_index) {
      case 0:
        tft.drawLine(x_value, y_value-size, x_value, y_value+size, ST7735_WHITE);
        break;
      case 1:
        tft.drawLine(x_value-size, y_value, x_value+size, y_value, ST7735_WHITE);
        break;
      case 2:
        tft.drawRect(x_value-size, y_value-size, x_value, y_value, ST7735_WHITE);
        break;
      case 3:
        tft.fillRect(x_value-size, y_value-size, x_value, y_value, ST7735_WHITE);
        break;
      case 4:
        tft.drawCircle(x_value, y_value, size, ST7735_WHITE);
        break;
      case 5:
        tft.fillCircle(x_value, y_value, size, ST7735_WHITE);
        break;
    }
  }

  if(millis() - last_update >= time_between_updates) {
    last_update = millis();

    boolean btn = digitalRead(joy_btn);
    if(btn == LOW) {
      process_step_index += 1;
      delay(300);
    }
  }
}

