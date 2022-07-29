/*
 * Code file for an RGB LED torch, made using the atTiny85 microcontroller.
 * Press button to switch between preset light colours.
 * Hold button to smoothly cycle through the colour spectrum.
 */

// Input pin for the button
const int pin_button = 2;

// PWM output pins for the RGB LED
const int pin_r = 0;
const int pin_g = 1;
const int pin_b = 4;

// Interval between each slight change in the smooth cycle (Milliseconds, lower is faster)
const int cycleInterval = 10;

// Maximum brightness value for the LED(0 = OFF, 255 = MAX)
const int maxValue = 50;

// Logic for smooth cycle loop
int loopCount = 0;
int selector = 0;

// Individual values for each LED pin
int rvalue = maxValue;
int gvalue = 0;
int bvalue = 0;

// Logic for button input
int buttonState = LOW;
int buttonPressed = LOW;
int buttonCount = 0;

// Button input detection without delay
const int waitTime = 2;
int secondCounter = 0;
unsigned long previousMillis = 0;

void setup()
{
  pinMode(pin_button, INPUT);
  
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);

  // Light the RGB LED white
  SetWhite();
}

void loop()
{
  // Read the state of the button
  buttonState = digitalRead(pin_button);

  // If the button is pressed
  if(buttonState == HIGH)
  {  
    // Start a timer
    unsigned long currentMillis = millis();
    
    // Count up in seconds from button pressed
    if (currentMillis - previousMillis >= 1000)
    {
      previousMillis = currentMillis;
      secondCounter++;
    }

    // If a set amount of time has passed
    if (secondCounter >= waitTime)
    {
      // Smooth cycle through the colour spectrum
      CycleColours();
    }

    // If button has not been pressed already
    if (buttonPressed == LOW)
    {
      // Register button as pressed
      buttonPressed = HIGH;

      // Add to the count of how many times the button has been pressed
      buttonCount++;

      // Set light colour according to how many times button has been pressed
      switch(buttonCount)
      {
        case 1:
          SetGreen();
          break;
        case 2:
          SetBlue();
          break;
        case 3:
          SetYellow();
          break;
        case 4:
          SetTurquoise();
          break;
        case 5:
          SetPurple();
          break;
        case 6:
          SetRed();
          break;
        case 7:
          SetWhite();
          buttonCount = 0;
          break;
      }
    }
    else
    {
      // Delay for cycle smoothing
      // Strategically placed here to help buffer out the main loop for performance reasons
      delay(cycleInterval);
    }
  }
  else
  {
    // Reset second counter
    secondCounter = 0;

    // Register button as no longer pressed
    buttonPressed = LOW;
  }
}

void SetWhite()
{
  rgb(255, 255, 255);
}

void SetRed()
{
  rgb(maxValue, 0, 0);
}

void SetGreen()
{
  rgb(0, maxValue, 0);
}

void SetBlue()
{
  rgb(0, 0, maxValue);
}

void SetYellow()
{
  rgb(maxValue, maxValue, 0);
}

void SetTurquoise()
{
  rgb(0, maxValue, maxValue);
}

void SetPurple()
{
  rgb(maxValue, 0, maxValue);
}

// Smooth cycle through colour spectrum when button is held
void CycleColours()
{
  rgb(rvalue, gvalue, bvalue);
  
  loopCount++;
  
  if(selector == 0)
  {
    rvalue--;
    bvalue++;
  }

  if(selector == 1)
  {
    gvalue++;
    bvalue--;
  }

  if(selector == 2)
  {
    rvalue++;
    gvalue--;
  }

  if(loopCount == maxValue)
  {
    loopCount = 0;

    if(selector == 2)
    {
      selector = 0;
    }
    else
    {
      selector++;
    }
  }
}

// Apply the colour to the LED
void rgb(int rv, int gv, int bv)
{
  analogWrite(pin_r, rv);
  analogWrite(pin_g, gv);
  analogWrite(pin_b, bv);
}
