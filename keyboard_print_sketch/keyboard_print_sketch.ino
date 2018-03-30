
// Require keyboard control library
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);

int ledState = HIGH;

// This function intercepts key press
void keyPressed() {
  printKey(true);
}

// This function intercepts key release
void keyReleased() {
  printKey(false);
}

void modChanged() {
  
}


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
  if (mod & LeftShift)
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
  Serial.write(key);
  
  Serial.println();
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Program started");

  pinMode(13, OUTPUT);

  delay(200);
}


void loop()
{
  // Process USB tasks
  usb.Task();
  digitalWrite(13, ledState); // flash the LED. just for funzies
  ledState = ledState == HIGH ? LOW : HIGH;

}