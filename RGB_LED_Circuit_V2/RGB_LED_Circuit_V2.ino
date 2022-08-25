/*

   VERSION 2
    - Makes use of a 2D array to store and iterate through colour values
    - 2D array is initialised with brightness variables to easily alter brightness
    - Colour cycle effect redesigned to remove unusual glitchy behaviour

 ************************************************************************

   Code file for an RGB LED torch, made using the atTiny85 microcontroller.
   Press button to switch between preset light colours.
   Hold button to smoothly cycle through the colour spectrum.

*/

// Input pin for push button
const int pin_button = 2;

// PWM output pins for the RGB LED
const int pin_r = 4;
const int pin_g = 0;
const int pin_b = 1;

// Maximum brightness for white light
const int w_brightness = 255;

// Maximum brightness for colour light
const int c_brightness = 200;

// Time required to pass before colour cycle begins
const int hold_time = 2;

// Delay interval for speed of colour cycle effect
const int cycle_interval = 5;

// Delay buffer to smooth out button press detection
const int press_buffer = 100;

// Two dimensional array containing preset colour values
const int colours[][3] =
{
  {w_brightness, w_brightness, w_brightness}, // 0 WHITE
  {c_brightness, 0, 0},                       // 1 RED
  {c_brightness, 0, c_brightness},            // 2 PURPLE
  {0, 0, c_brightness},                       // 3 BLUE
  {0, c_brightness, c_brightness},            // 4 TURQUOISE
  {0, c_brightness, 0},                       // 5 GREEN
  {c_brightness, c_brightness, 0},            // 6 YELLOW
};

// The array indexing value
int current_colour = 0;

// Button state detection flags - down and up
bool button_d = LOW;
bool button_u = LOW;

// Variables to count in seconds without using delay
unsigned long current_ms = 0;
unsigned long previous_ms = 0;
int second_counter = 0;

// Variables to store RGB values
int rv = 0;
int gv = 0;
int bv = 0;

// Run once at device start
void setup()
{
  // Set button pin to input
  pinMode(pin_button, INPUT);

  // Set RGB LED pins to outputs
  pinMode(pin_r, OUTPUT);
  pinMode(pin_g, OUTPUT);
  pinMode(pin_b, OUTPUT);

  // Set the RGB LED to bright white
  SetColour(colours[0]);
}

// runs on repeat
void loop()
{
  // Set button down flag to HIGH when button is pressed
  button_d = digitalRead(pin_button);

  // Start counting in milliseconds
  current_ms = millis();

  // If the button has been pressed
  if (button_d == HIGH)
  {
    // If a thousand milliseconds has passed
    if (current_ms - previous_ms >= 1000)
    {
      // Add a second to the second counter
      previous_ms = current_ms;
      second_counter++;
    }

    // If the hold time amount of seconds has passed
    if (second_counter >= hold_time)
    {
      // Start the colour cycle effect
      CycleColours();
    }

    // If the button has not been released yet
    if (button_u == HIGH)
    {
      // Set the button up flag to LOW
      button_u = LOW;

      // Change to the next colour in the array
      NextColour();

      // Delay for smoothness
      delay(press_buffer);
    }
  }
  else
  {
    // Button has been released, set button up flag as HIGH
    button_u = HIGH;

    // Reset the second counter
    second_counter = 0;

    // Delay for smoothness
    delay(press_buffer);
  }
}

// Change RGB LED to next colour in array when called
void NextColour()
{
  // Increase colour index value
  current_colour++;

  // If at the end of the array
  if (current_colour == sizeof(colours) / sizeof(colours[0]))
  {
    // Reset index value
    current_colour = 0;
  }

  // Set colour storage values to values in 2D array
  rv = colours[current_colour][0];
  gv = colours[current_colour][1];
  bv = colours[current_colour][2];

  // Set the colour of the RGB LED
  SetColour(colours[current_colour]);
}

// Smoothly cycle through the colour values
void CycleColours()
{
  // Do the correct course of action depending on the current colour
  switch (current_colour)
  {
    // 0: LED is Bright white, quickly transition to red
    case 0:
      gv -= 5;
      bv -= 5;

      if (rv != 100)
      {
        rv--;
      }
      if (gv == 0)
      {
        current_colour = 1;
      }
      break;

    // 1: LED is red or purple, transition to blue
    case 1:
      bv++;
    case 2:
      rv--;

      if (rv == 0)
      {
        current_colour = 3;
      }
      break;

    // 3: LED is blue or turquoise, transition to green
    case 3:
      gv++;
    case 4:
      bv--;

      if (bv == 0)
      {
        current_colour = 5;
      }
      break;

    // 5: LED is green or yellow, transition to red
    case 5:
      rv++;
    case 6:
      gv--;

      if (gv == 0)
      {
        current_colour = 1;
      }
      break;
  }

  // Set the colour of the RGB LED
  int c[] = {rv, gv, bv};
  SetColour(c);

  // Delay based on cycle interval
  delay(cycle_interval);
}

// Analog write RGB values to the pins when called
void SetColour(int values[])
{
  analogWrite(pin_r, values[0]);
  analogWrite(pin_g, values[1]);
  analogWrite(pin_b, values[2]);
}
