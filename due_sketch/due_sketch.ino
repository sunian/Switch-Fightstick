#define KEY_UP     'i'
#define KEY_DOWN   'k'
#define KEY_LEFT   'j'
#define KEY_RIGHT  'l'
#define KEY_UPC    'e'
#define KEY_DOWNC  'c'
#define KEY_LEFTC  'f'
#define KEY_RIGHTC LeftShift

#define KEY_A      's'
#define KEY_B      'd'
#define KEY_R      'w'
#define KEY_ZL     'a'
#define KEY_TILT   ' '
#define KEY_PLUS    0x13 // enter
#define KEY_UP_TILT      'p'
#define KEY_UP_TILT_ATK  'r'

#define KEY_X      'o'
#define KEY_Y      'u'
#define KEY_L      'z'
#define KEY_ZR     'q'
#define KEY_MINUS  '\\'
#define OEM_KEY_HOME    41 // backspace
#define OEM_KEY_UP      82 // UP
#define OEM_KEY_DOWN    81 // DOWN
#define OEM_KEY_LEFT    80 // LEFT
#define OEM_KEY_RIGHT   79 // RIGHT

#define PIN_UP      A0
#define PIN_DOWN    A1
#define PIN_LEFT    A2
#define PIN_RIGHT   A3
#define PIN_UPC     A4
#define PIN_DOWNC   A5
#define PIN_LEFTC   A6
#define PIN_RIGHTC  A7

// Require keyboard control library
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);


bool state_UP     = false;
bool state_DOWN   = false;
bool state_LEFT   = false;
bool state_RIGHT  = false;
bool state_UPC    = false;
bool state_DOWNC  = false;
bool state_LEFTC  = false;
bool state_RIGHTC = false;

bool state_A      = false;
bool state_B      = false;
bool state_ZL     = false;
bool state_R      = false;
bool state_TILT   = false;
bool state_PLUS  = false;
bool state_UP_TILT      = false;
bool state_UP_TILT_ATK  = false;
bool state_X    = false;
bool state_Y    = false;
bool state_L    = false;
bool state_ZR   = false;
bool state_MINUS   = false;
bool state_HOME   = false;

bool state_UPD    = false;
bool state_DOWND  = false;
bool state_LEFTD  = false;
bool state_RIGHTD = false;

int ledState = HIGH;

bool left = false; // is pressing LEFT
bool right = false; // is pressing RIGHT

// This function intercepts key press
void keyPressed() {
  updateKey(true);
}

// This function intercepts key release
void keyReleased() {
  updateKey(false);
}

void modChanged() {
  state_RIGHTC = keyboard.getModifiers() & KEY_RIGHTC;
}

void updateKey(bool pressed) {
  int oemKey = keyboard.getOemKey();
  if (oemKey == 0) return;
  int key = keyboard.getKey();
  if (key == KEY_UP) state_UP = pressed;
  else if (key == KEY_DOWN   ) state_DOWN = pressed;
  else if (key == KEY_LEFT   ) state_LEFT   = pressed;
  else if (key == KEY_RIGHT  ) state_RIGHT  = pressed;
  else if (key == KEY_UPC    ) state_UPC    = pressed;
  else if (key == KEY_DOWNC  ) state_DOWNC  = pressed;
  else if (key == KEY_LEFTC  ) state_LEFTC  = pressed;
  else if (key == KEY_A      ) state_A      = pressed;
  else if (key == KEY_B      ) state_B      = pressed;
  else if (key == KEY_ZL     ) state_ZL     = pressed;
  else if (key == KEY_R      ) state_R      = pressed;
  else if (key == KEY_TILT   ) state_TILT   = pressed;
  else if (key == KEY_PLUS   ) state_PLUS   = pressed;
  else if (key == KEY_UP_TILT      ) state_UP_TILT      = pressed;
  else if (key == KEY_UP_TILT_ATK  ) state_UP_TILT_ATK  = pressed;
  else if (key == KEY_X      ) state_X      = pressed;
  else if (key == KEY_Y      ) state_Y      = pressed;
  else if (key == KEY_L      ) state_L      = pressed;
  else if (key == KEY_ZR     ) state_ZR     = pressed;
  else if (key == KEY_MINUS  ) state_MINUS  = pressed;
  else if (oemKey == OEM_KEY_HOME   ) state_HOME   = pressed;
  else if (oemKey == OEM_KEY_UP     ) state_UPD   = pressed;
  else if (oemKey == OEM_KEY_DOWN   ) state_DOWND   = pressed;
  else if (oemKey == OEM_KEY_LEFT   ) state_LEFTD   = pressed;
  else if (oemKey == OEM_KEY_RIGHT  ) state_RIGHTD   = pressed;
}


void setup()
{

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  pinMode(PIN_UP, OUTPUT);
  pinMode(PIN_DOWN, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  pinMode(PIN_UPC, OUTPUT);
  pinMode(PIN_DOWNC, OUTPUT);
  pinMode(PIN_LEFTC, OUTPUT);
  pinMode(PIN_RIGHTC, OUTPUT);

  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  pinMode(13, OUTPUT);

  delay(200);
}


void loop()
{

  if (state_DOWN) { // down takes priority (should make wavedashing easier)
    digitalWrite(PIN_UP, LOW);
    digitalWrite(PIN_DOWN, HIGH);
  } else {
    digitalWrite(PIN_DOWN, LOW);
    if (state_UP || state_UP_TILT || state_UP_TILT_ATK) digitalWrite(PIN_UP, HIGH); else digitalWrite(PIN_UP, LOW);
  }

  if (state_RIGHT) {
    if (left) {
      digitalWrite(PIN_LEFT, LOW);
      digitalWrite(PIN_RIGHT, HIGH);
      left = false;
      right = true;
    } else {
      if (state_LEFT) {
        if (right) {
          digitalWrite(PIN_RIGHT, LOW);
          digitalWrite(PIN_LEFT, HIGH);
          right = false;
          left = true;
        } else {
          digitalWrite(PIN_LEFT, LOW);
          digitalWrite(PIN_RIGHT, LOW);
        }
      } else {
        digitalWrite(PIN_LEFT, LOW);
        digitalWrite(PIN_RIGHT, HIGH);
        right = true;
      }
    }
  } else {
    digitalWrite(PIN_RIGHT, LOW);
    if (state_LEFT) {
      digitalWrite(PIN_LEFT, HIGH);
      left = true;
    } else {
      digitalWrite(PIN_LEFT, LOW);
      left = false;
    }
    right = false;
  }

  if (state_UPC) { // up smash takes priority (shouldn't ever matter)
    digitalWrite(PIN_DOWNC, LOW);
    digitalWrite(PIN_UPC, HIGH);
  } else {
    digitalWrite(PIN_UPC, LOW);
    if (state_DOWNC) digitalWrite(PIN_DOWNC, HIGH); else digitalWrite(PIN_DOWNC, LOW);
  }

  if (state_LEFTC) { // left smash takes priority (the button is bigger)
    digitalWrite(PIN_RIGHTC, LOW);
    digitalWrite(PIN_LEFTC, HIGH);
  } else {
    digitalWrite(PIN_LEFTC, LOW);
    if (state_RIGHTC) digitalWrite(PIN_RIGHTC, HIGH); else digitalWrite(PIN_RIGHTC, LOW);
  }

  if (state_Y ) digitalWrite(2, HIGH); else digitalWrite(2, LOW);
  if (state_B   ) digitalWrite(3, HIGH); else digitalWrite(3, LOW);
  if (state_A || state_UP_TILT_ATK) digitalWrite(4, HIGH); else digitalWrite(4, LOW);
  if (state_X ) digitalWrite(5, HIGH); else digitalWrite(5, LOW);
  if (state_L ) digitalWrite(6, HIGH); else digitalWrite(6, LOW);
  if (state_R ) digitalWrite(7, HIGH); else digitalWrite(7, LOW);
  if (state_ZL ) digitalWrite(8, HIGH); else digitalWrite(8, LOW);
  if (state_ZR ) digitalWrite(9, HIGH); else digitalWrite(9, LOW);
  
  
  if (state_TILT || state_UP_TILT || state_UP_TILT_ATK) digitalWrite(A8, HIGH); else digitalWrite(A8, LOW);
  if (state_PLUS ) digitalWrite(A9, HIGH); else digitalWrite(A9, LOW);
  if (state_MINUS ) digitalWrite(A10, HIGH); else digitalWrite(A10, LOW);
  if (state_HOME ) digitalWrite(A11, HIGH); else digitalWrite(A11, LOW);

  if (state_UPD ) digitalWrite(14, HIGH); else digitalWrite(14, LOW);
  if (state_DOWND ) digitalWrite(15, HIGH); else digitalWrite(15, LOW);
  if (state_LEFTD ) digitalWrite(16, HIGH); else digitalWrite(16, LOW);
  if (state_RIGHTD ) digitalWrite(17, HIGH); else digitalWrite(17, LOW);

  // Process USB tasks
  usb.Task();
  digitalWrite(13, ledState); // flash the LED. just for funzies
  ledState = ledState == HIGH ? LOW : HIGH;

}
