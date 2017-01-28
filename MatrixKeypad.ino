#include <Keyboard.h>

int col_0 = 2;
int col_1 = 3;
int col_2 = 4;

int col_count = 3;

int row_0 = 8;
int row_1 = 9;
int row_2 = 10;

int row_count = 3;

int columns[] = {col_0, col_1, col_2};
int rows[] = {row_0, row_1, row_2};

bool readout[3][3] ;
bool prev_readout[3][3] ;

char keys[3][3] = {


  {'7','8','9'},
    {'4','5','6'},
      {'1','2','3'}
};

void tur()
{  
  Serial.println("tur");
}

void zebra()
{  
  Serial.println("zebra");
  //Keyboard.print("zebra");
}

bool changed = false;

void (*funcs[3][3])() = {
  {zebra, zebra, tur},
  {tur, tur, tur},
  {tur, zebra, zebra}
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i < col_count; i++)
  {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], LOW);
  }
  for(int i = 0; i < row_count; i++)
  {
    pinMode(rows[i], INPUT_PULLUP);
  }
  
  for(int i = 0; i < col_count; i++)
  {
    for(int j = 0; j < row_count; j++)
    {
      readout[i][j] = false;
    }
  }
}

void readState()
{
  for(int i = 0; i < col_count; i++)
  {
    digitalWrite(columns[i], HIGH);
    for(int j = 0; j < row_count; j++)
    {   
      bool pinRead = digitalRead(rows[j]);
      if(!changed && (pinRead != readout[i][j]))
      {
        changed = true;
      }      
      readout[i][j] = pinRead;
    }   
    digitalWrite(columns[i], LOW);
  }
}

void printReadout()
{
  if(!changed)
  {
    return;
  }
  
  String state = "";
  for(int i = 0; i < col_count; i++)
  {
    for(int j = 0; j < row_count; j++)
    {      
      if(readout[i][j])
      {
        //Keyboard.write(keys[i][j]);
        state += keys[i][j];
        (*funcs[i][j])();
        
      }      
    }
  }
  Serial.println(state);  
  changed = false;
}

void loop() {
  readState();  
  printReadout();
  delay(50);
}
