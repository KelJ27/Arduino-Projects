#define DATA_PIN        8    // Row data line
#define CLOCK_PIN       9    // Clock sequence
#define LATCH_PIN       10   // Latch pin
#define MAX_ROWS        8    // Max number of rows in LED matrix
#define JOY_BUTTON_PIN  2    // Button of the joystick
#define JOY_X           A0   // X-axis of joystick
#define JOY_Y           A1   // Y-axis of joystick

const int NUM_ROWS = 8;
const int NUM_COLUMNS = 8;
int pic[MAX_ROWS] = {1,0,0,0,0,0,0,0};
int bitNum[8] = {1,2,4,8,16,32,64,128};
int xValue, yValue, xMap, yMap;

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  //clearLedMatrix();
}

// the loop function runs over and over again forever
void loop() 
{
  xValue = analogRead(JOY_X);
  yValue = analogRead(JOY_Y);
  xMap = map(xValue, 0, 1023, 0, 8);
  yMap = map(yValue, 0, 1023, 0, 8);
 
  //print the values with to plot or view
  Serial.print(xMap);
  Serial.print("\t");
  Serial.println(yMap);

  setLedMatrixGrid();
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

    //useRedLeds();
    //useGreenLeds();
    useOrangeLeds();
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

void clearLedMatrix()
{
  for (int i = 0; i < 8; i++)
  {
    pic[i] = 0;
  }
}
