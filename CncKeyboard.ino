#include <Keyboard.h>

#define DEBUG_MODE 0

#define COLUMN_0 9
#define COLUMN_1 8
#define COLUMN_2 7
#define COLUMN_3 6

#define ROW_0 2
#define ROW_1 3
#define ROW_2 4
#define ROW_3 5

#define COLUMNS 4
#define ROWS 4

#define KEY_SPACE 0x20

// Key - input mapping
#define SPACE          0
#define XY_DOWN        1
#define XY_LEFT        2
#define XY_RIGHT       3
#define XY_UP         15
#define Z_DOWN         4
#define Z_UP           5
#define SPEED_10       6
#define SPEED_1        8
#define SPEED_01      11
#define ESCAPE         7

int columns[] = { COLUMN_0, COLUMN_1, COLUMN_2, COLUMN_3 };
int rows[] = { ROW_0, ROW_1, ROW_2, ROW_3 };

bool readout[COLUMNS][ROWS];
bool prev_readout[COLUMNS][ROWS];

bool changed = false;

byte enabled = 0;
byte disabled = 0;

byte enabled_keys[ROWS * COLUMNS];
byte disabled_keys[ROWS * COLUMNS];

void pressKey(const byte key)
{
  Keyboard.press(key);
}

void releaseKey(const byte key)
{
  Keyboard.release(key);
}

byte getInputNumber(const byte row, const byte column)
{
  return row * COLUMNS + column;
}

void onEnabled(const byte key)
{
  onChange(key, &pressKey);
}

void onDisabled(const byte key)
{
  onChange(key, &releaseKey);
}

void onChange(const byte key, void (*action)(byte))
{
  switch (key)
  {
    case SPACE:
      action(KEY_SPACE);
      break;
    case XY_DOWN:
      action(KEY_DOWN_ARROW);
      break;
    case XY_UP:
      action(KEY_UP_ARROW);
      break;
    case XY_LEFT:
      action(KEY_LEFT_ARROW);
      break;
    case XY_RIGHT:
      action(KEY_RIGHT_ARROW);
      break;
    case Z_UP:
      action(KEY_PAGE_UP);
      break;
    case Z_DOWN:
      action(KEY_PAGE_DOWN);
      break;
    case SPEED_10:
      action(KEY_RIGHT_CTRL);
      action(KEY_RIGHT_SHIFT);
      break;
    case SPEED_1:
      action(KEY_RIGHT_SHIFT);
      break;
    case SPEED_01:
      action(KEY_RIGHT_CTRL);
      break;
    case ESCAPE:
      action(KEY_ESC);    
      break;
    default:
      break;
  }

  if (DEBUG_MODE)
  {
      Serial.print("Key state changed: ");
    Serial.println(key);
  }

}

void setup()
{
  for (int i = 0; i < COLUMNS; i++)
  {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], LOW);
  }
  for (int i = 0; i < ROWS; i++)
  {
    pinMode(rows[i], INPUT_PULLUP);
  }

  for (int i = 0; i < COLUMNS; i++)
  {
    for (int j = 0; j < ROWS; j++)
    {
      readout[i][j] = false;
    }
  }

  if (DEBUG_MODE)
  {
    Serial.begin(9600);
  }

}

void readState()
{
  for (int i = 0; i < COLUMNS; i++)
  {
    digitalWrite(columns[i], HIGH);

    for (int j = 0; j < ROWS; j++)
    {
      bool pinRead = digitalRead(rows[j]);
      bool previousReadout = readout[i][j];

      if (pinRead != previousReadout)
      {
        byte key_number = getInputNumber(j, i);
        changed = true;

        if (pinRead) // Enabled
        {
          enabled_keys[enabled] = key_number;
          ++enabled;
        }
        else // Disabled
        {
          disabled_keys[disabled] = key_number;
          ++disabled;
        }
      }

      readout[i][j] = pinRead;
    }
    digitalWrite(columns[i], LOW);
  }
}

void actionState()
{
  if (!changed)
  {
    return;
  }

  for (byte i = 0; i < enabled; ++i)
  {
    onEnabled(enabled_keys[i]);
  }

  for (byte i = 0; i < disabled; ++i)
  {
    onDisabled(disabled_keys[i]);
  }

  changed = false;
  enabled = 0;
  disabled = 0;
}

unsigned long previousMillis = 0;
long debugInterval = 1000;

void printDiagnostics()
{
  if (!DEBUG_MODE)
  {
    return;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= debugInterval)
  {
    String state;
    for (int i = 0; i < ROWS; i++)
    {
      for (int j = 0; j < COLUMNS; j++)
      {
        readout[j][i] ? state += getInputNumber(i, j    ) : state += ". ";
      }
    }
    Serial.println(state);

    previousMillis = currentMillis;
  }
}

void loop()
{
  readState();
  actionState();
  printDiagnostics();
  delay(250);
}



