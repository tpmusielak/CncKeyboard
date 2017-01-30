#include <Keyboard.h>

#define COLUMN_0 2
#define COLUMN_1 3
#define COLUMN_2 4

#define ROW_0 8
#define ROW_1 9
#define ROW_2 10

int col_count = 3;
int row_count = 3;

int columns[] = { COLUMN_0, COLUMN_1, COLUMN_2 };
int rows[] = { ROW_0, ROW_1, ROW_2};

bool readout[3][3];
bool prev_readout[3][3];

bool changed = false;

byte enabled = 0;
byte disabled = 0;

byte enabled_keys[9];
byte disabled_keys[9];

void onEnabled(const byte key)
{
   switch (key)
   {
   case 0:
      Keyboard.press(KEY_LEFT_SHIFT);
      break;
   default:
      Serial.print("Key pressed: ");
      Serial.println(key);
      break;
   }
}

void onDisabled(const byte key)
{
   switch (key)
   {
   case 0:
      Keyboard.release(KEY_LEFT_SHIFT);
      break;
   default:
      Serial.print("Key released: ");
      Serial.println(key);
      break;
   }
}

void setup() 
{
   for (int i = 0; i < col_count; i++)
   {
      pinMode(columns[i], OUTPUT);
      digitalWrite(columns[i], LOW);
   }
   for (int i = 0; i < row_count; i++)
   {
      pinMode(rows[i], INPUT_PULLUP);
   }

   for (int i = 0; i < col_count; i++)
   {
      for (int j = 0; j < row_count; j++)
      {
         readout[i][j] = false;
      }
   }

   Serial.begin(9600);
}

void readState()
{
   for (int i = 0; i < col_count; i++)
   {
      digitalWrite(columns[i], HIGH);

      for (int j = 0; j < row_count; j++)
      {
         bool pinRead = digitalRead(rows[j]);
         bool previousReadout = readout[i][j];

         if (pinRead != previousReadout)
         {
            byte key_number = row_count * i + j;
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

void loop()
{
   readState();
   actionState();
   delay(50);
}
