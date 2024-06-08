#define DATA_PIN          8    // Row data line
#define CLOCK_PIN         9    // Clock sequence
#define LATCH_PIN         10   // Latch pin
#define MAX_ROWS          8    // Max number of rows in LED matrix
#define JOY_BUTTON_PIN    2    // Button of the joystick
#define JOY_X             A0   // X-axis of joystick
#define JOY_Y             A1   // Y-axis of joystick
#define MAX_CLICK_COUNT   3    // Max number of times to click button

const int NUM_ROWS = 8;
const int NUM_COLUMNS = 8;
int pic[MAX_ROWS] = {1,0,0,0,0,0,0,0};
int bitNum[8] = {1,2,4,8,16,32,64,128};
double xValue, yValue, xMap, yMap;
int clickCount = 0;
unsigned long lastDebounceTime = 0;  // The last time the output pin was toggled
unsigned long debounceDelay = 50;    // The debounce time
int buttonState;                     // the current reading from the input pin
int lastButtonState = LOW;           // the previous reading from the input pin
int reading;

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);

  clearLedMatrix();
}

// the loop function runs over and over again forever
void loop() 
{
  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);
  xMap = mmap(xValue, 0, 1023, 0, 7);
  yMap = mmap(yValue, 0, 1023, 0, 7);
 
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);

  debounceButtonPress();
  setLedMatrixGrid();
}

long mmap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

int binaryStringToDecimal(char *s) {
  int result = 0;

  while(*s) 
  {
    result <<= 1;
    if(*s++ == '1') result |= 1;
  }

  return result;
}


void setRow(int value) 
{
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 128 >> value);
}

void setColumn(int value)
{
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, ~bitNum[value]);
}

void store()
{
  digitalWrite(LATCH_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(LATCH_PIN, LOW);
  delayMicroseconds(10);
}

void setLedMatrixGrid()
{
  for (int i = 0; i <8; i++)
  {
    switch(clickCount)
    {
      case 1:
        useRedLeds();
        break;
      case 2:
        useGreenLeds();
        break;
      case 3:
        useOrangeLeds();
        break;
      default:
        clickCount = 1;
        useRedLeds();
        break;
    }
    //useOrangeLeds();
    setRow(xMap);
    store();
  }  
}

void useRedLeds()
{
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 255);
    setColumn(yMap);
}

void useGreenLeds()
{
    setColumn(yMap);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 255);
}

void useOrangeLeds()
{
  setColumn(yMap);
  setColumn(yMap);
}

void debounceButtonPress()
{
  reading = digitalRead(JOY_BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) 
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) 
    {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) 
      {
        clickCount++;
      }
    }
  }

  lastButtonState = reading;
}

void clearLedMatrix()
{
  for (int i = 0; i < 8; i++)
  {
    pic[i] = 0;
  }
}
