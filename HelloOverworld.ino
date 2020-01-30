/**
 * ~~~~~ Hello Overworld ~~~~~
 */

// Define Constants

// ----- Buttons -----
// Each button corresponds to an array index so data can be setup in a standard way
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define NUM_BUTTONS 4
const int buttons[NUM_BUTTONS] = { 22, 12, 24, 23 }; //Define pins
const int button_directions_x[NUM_BUTTONS] = { -1, 1, 0, 0 }; // Define horizontal positional changes that occur on pressing a given button 
const int button_directions_y[NUM_BUTTONS] = { 0, 0, 1, -1 }; // Define vertical positional changes that occur on pressing a given button 
// -------------------


// ------- LEDS ------
// Leds are connected in a matrix style, so one dimension provides provides power  and the other goes to 0 to create a circuit
#define NUM_ROWS 5
#define NUM_COLUMNS 5

const int rows[] = { 5, 6, 9, 10, 11 }; // Define row pins - these are the power lines
const int columns[] = { 34, 36, 38, 40, 42 }; // Define column pins - these are the ground lines

#define MAX_BRIGHTNESS 150

int ledON_x = 0; // Define the x-position of the led that is currently on
int ledON_y = 0; // Define the y-position of the led that is currently on
// Initial led is the bottom left
// -------------------

void setup() {
  //Setup buttons as inputs
  for(int i=0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  //Setup matrix lines as outputs
  // Setup rows
  for(int i=0; i < NUM_ROWS; i++) {
    pinMode(rows[i], OUTPUT);
  }
  // Setup columns
  for(int i=0; i < NUM_COLUMNS; i++) {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH); //Since these are ground when an led is on, set all to high to turn them off
  }
  // Turn on the initial LED
  analogWrite(rows[ledON_y], MAX_BRIGHTNESS);
  digitalWrite(columns[ledON_x], LOW);
}

void loop() {
  int input = checkButtons();
  //If a button has been pressed, get the index of the next led in that direction
  if(input >= 0) {
    int next_x = ledON_x + button_directions_x[input];
    int next_y = ledON_y + button_directions_y[input];

    //If the next led position is within the bounds of the led grid, 
    //  send the pins of the current and next led positions to move and update the stored position of ledON
    if(next_x >= 0 && next_x < NUM_COLUMNS && next_y >= 0 && next_y < NUM_ROWS) {
      move(columns[ledON_x], rows[ledON_y], columns[next_x], rows[next_y]);
      ledON_x = next_x;
      ledON_y = next_y;
    }
  }
}

// Return the index of a button if it is being pressed
int checkButtons() {
  for(int i = 0; i < NUM_BUTTONS; i++) {
    if(digitalRead(buttons[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

// Move the led by turning off the current led and turning on the next
// Inputs:
//    current_x: the x-position of the LED that is on and needs to be turned off
//    current_y: the y-position of the LED that is on and needs to be turned off
//    next_x: the x-position of the LED needs to be turned on
//    next_y: the y-position of the LED needs to be turned on
void move(int current_x, int current_y, int next_x, int next_y) {
  // Turn the the current LED off
  digitalWrite(current_x, HIGH) // Y goes to HIGH for Off (remove connection to ground)
  analogWrite(current_y, 0);;   // X goes to LOW for Off  (remove power)
       
  //Turn the new led on
  analogWrite(next_x, LOW);             // X goes to ground
  analogWrite(next_y, MAX_BRIGHTNESS);  // Y goes to power
  delay(500); //Wait a half a second so the user can either hold to move multiple spaces or stop pressing to just move one
}
