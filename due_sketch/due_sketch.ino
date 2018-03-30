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
#define KEY_GRAB   'w'
#define KEY_SHIELD 'a'
#define KEY_TILT   ' '
#define KEY_START  0x13
#define KEY_UP_TILT      'p'
#define KEY_UP_TILT_ATK  'r'

#define KEY_X      'o'
#define KEY_Y      'u'
#define KEY_L      'q'
#define KEY_ZR     'z'

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
bool state_GRAB   = false;
bool state_SHIELD = false;
bool state_TILT   = false;
bool state_START  = false;
bool state_UP_TILT      = false;
bool state_UP_TILT_ATK  = false;
bool state_X   = false;
bool state_Y = false;
bool state_L   = false;
bool state_ZR  = false;

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
  if (keyboard.getOemKey() == 0) return;
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
  else if (key == KEY_GRAB   ) state_GRAB   = pressed;
  else if (key == KEY_SHIELD ) state_SHIELD = pressed;
  else if (key == KEY_TILT   ) state_TILT   = pressed;
  else if (key == KEY_START  ) state_START  = pressed;
  else if (key == KEY_UP_TILT      ) state_UP_TILT      = pressed;
  else if (key == KEY_UP_TILT_ATK  ) state_UP_TILT_ATK  = pressed;
  else if (key == KEY_X      ) state_X      = pressed;
  else if (key == KEY_Y      ) state_Y      = pressed;
  else if (key == KEY_L      ) state_L      = pressed;
  else if (key == KEY_ZR     ) state_ZR     = pressed;
}

/*
void printKey(bool pressed) {
  int oemkey = keyboard.getOemKey();
  if (oemkey == 0) return;

  if (pressed) {
    Serial.print("Pressed:  ");
  } else {
    Serial.print("Released: ");
  }

  // getOemKey() returns the OEM-code associated with the key
  Serial.print(" key:");
  Serial.print(oemkey);

  // getModifiers() returns a bits field with the modifiers-keys
  int mod = keyboard.getModifiers();

  Serial.print(" => ");

  if (mod & LeftCtrl)
    Serial.print("L-Ctrl ");
  if (mod & KEY_RIGHTC)
    Serial.print("L-Shift ");
  if (mod & Alt)
    Serial.print("Alt ");
  if (mod & LeftCmd)
    Serial.print("L-Cmd ");
  if (mod & RightCtrl)
    Serial.print("R-Ctrl ");
  if (mod & RightShift)
    Serial.print("R-Shift ");
  if (mod & AltGr)
    Serial.print("AltGr ");
  if (mod & RightCmd)
    Serial.print("R-Cmd ");

  // getKey() returns the ASCII translation of OEM key
  // combined with modifiers.
  int key = keyboard.getKey();
  if (key == KEY_TILT) {
    Serial.print("SPACE");
  } else if (key == KEY_START) {
    Serial.print("ENTER");
  } else {
    Serial.write(key);
  }
  Serial.println();
}
*/

void setup()
{
  //Serial.begin(115200);
  //Serial.println("Program started");

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
  if (state_GRAB ) digitalWrite(7, HIGH); else digitalWrite(7, LOW);
  if (state_SHIELD ) digitalWrite(8, HIGH); else digitalWrite(8, LOW);
  if (state_ZR ) digitalWrite(9, HIGH); else digitalWrite(9, LOW);
  
  
  if (state_TILT || state_UP_TILT || state_UP_TILT_ATK) digitalWrite(A8, HIGH); else digitalWrite(A8, LOW);
  if (state_START ) digitalWrite(26, HIGH); else digitalWrite(26, LOW);

  // Process USB tasks
  usb.Task();
  digitalWrite(13, ledState); // flash the LED. just for funzies
  ledState = ledState == HIGH ? LOW : HIGH;

}
