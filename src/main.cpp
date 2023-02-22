/* MAX7219 LED dot matrix display*/

// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // Use Use FC-16 style hardware module
#define MAX_DEVICES 4
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS


// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


// Button input
#include <ezButton.h>
ezButton button(8);

// Button logics
unsigned long buttonCount = 0;
unsigned long buttonPreviousCount = 0;
unsigned long buttonIsPressed = 0;

unsigned long resetButtonPressCountMillis = 650;
unsigned long lastButtonPressMillis = millis();
unsigned long previousButtonPressMillis = millis();
bool buttonPressedBefore = 0;
unsigned long lastButtonPressWasLongPress = 0;


void setup() {
  // Serial
    Serial.begin(57600);

    while (!Serial) // Wait for the serial connection to be establised.
      delay(50);


  /* LED DOT MATRIX DISPLAY */
  // Intialize the object:
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(5);
  // Clear the display:
  myDisplay.displayClear();

  /* BUTTONS */
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.setCountMode(COUNT_FALLING);

  /* LED FOR TRIGGER + CAMERA TRIGGER */
  pinMode(3, OUTPUT); //Pin 3 triggers camera via optop-isolator
  pinMode(2, OUTPUT); //Pin 3 triggers camera via optop-isolator
}


// Delays
boolean delay_without_delaying(unsigned long time)
{
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time)
  {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}

boolean delay_without_delaying(unsigned long &since, unsigned long time)
{
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  unsigned long currentmillis = millis();
  if (currentmillis - since >= time)
  {
    since = currentmillis;
    return true;
  }
  return false;
}

boolean shutterTrigger()
{
    Serial.println("Starting...");
    Serial.println("3+2 HIGH");
    myDisplay.setTextAlignment(PA_LEFT);
    myDisplay.print("SMIL:D");
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    delay(3000);
    myDisplay.print("<'^^,'>");
    Serial.println("3+2 LOW");
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    delay(1000);
}

boolean shutterCountDown(int countAmount, int countEachMsDelay)
{
  delay(500);

  while (countAmount > 0)
  {

    myDisplay.setTextAlignment(PA_LEFT);
    myDisplay.print(countAmount);
    delay(countEachMsDelay);

    if (countAmount == 1)
    {
      shutterTrigger();

      myDisplay.print("Tak <3");
      delay(countEachMsDelay*4);
      myDisplay.print("");
      return 1;
    }
    countAmount = countAmount - 1;
  }
}


void loop() {
  // Button counts
  button.loop(); // MUST call the loop() function first

  if (button.isPressed())
  {
    Serial.println("The button is pressed");
    buttonIsPressed = 1;
  }

  if (button.isReleased())
  {
    Serial.println("The button is released");
    buttonIsPressed = 0;
  }


  buttonCount = button.getCount();

  if (buttonCount != buttonPreviousCount)
  {
      Serial.println(buttonCount);
      switch (buttonCount)
      {
      case 1:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("Ok...");
        break;

      case 2:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C2!");
        break;

      case 3:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C3!");
        break;

      case 4:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C4!");
        break;

      case 5:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C5!");
        break;

      case 6:
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C6!");
        break;
      }

    // If button has not been pressed before
    if (!buttonPressedBefore)
    {
      lastButtonPressMillis = millis();
    }

    previousButtonPressMillis = lastButtonPressMillis;
    lastButtonPressMillis = millis();

    buttonPreviousCount = buttonCount;
  }

  static unsigned long ledtime = 0;
  // Every 500
  if (delay_without_delaying(ledtime, 500))
  {
    // Check if button press has been too long time ago
    unsigned long currentButtonmillis = millis();
    buttonCount = button.getCount();
    if (buttonCount > 0 && currentButtonmillis - lastButtonPressMillis >= resetButtonPressCountMillis)
    {
      Serial.println("Button press max millis triggered - Now running through cases with switch");

      // Check the buttonCount
      switch (buttonCount)
      {
      case 1:
        Serial.println("Case 1");
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("Ok :D");
        shutterCountDown(5,1000);
        break;

      case 2:
        if (millis() > lastButtonPressMillis + 650)
        {

          Serial.println("Case 2");
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print("C2");
          
        }
        break;

      case 3:
        Serial.println("Case 3");
        myDisplay.setTextAlignment(PA_CENTER);
        shutterTrigger();
        break;

      case 4:
        Serial.println("Case 4");
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C4");
        break;

      case 5:
        Serial.println("Case 5");
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C5");
        break;

      case 6:
        Serial.println("Case 6");
        myDisplay.setTextAlignment(PA_CENTER);
        myDisplay.print("C6");
        break;
      }

      // Reset count
      button.resetCount();
      Serial.println("Reset Button Count - And running through switch-cases");

      // Set state of longpress button
      if (buttonIsPressed)
        lastButtonPressWasLongPress = 1;
    }
  }

  // Every 1000
  static unsigned long atime;
  if (delay_without_delaying(atime, 5000))
  {
    // myDisplay.setTextAlignment(PA_CENTER);
    //myDisplay.print("Reset");
    Serial.println("Run every 5000");
    // myDisplay.setTextAlignment(PA_LEFT);
    // myDisplay.print("Left");
    // delay(2000);
    // myDisplay.setTextAlignment(PA_RIGHT);
    // myDisplay.print("Right");
    // delay(2000);
    // myDisplay.setTextAlignment(PA_CENTER);
    // myDisplay.setInvert(true);
    // myDisplay.print("Invert");
    // delay(2000);
    // myDisplay.setInvert(false);
    // myDisplay.print(1234);
    // delay(2000);

  }
}