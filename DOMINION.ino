#include <LiquidCrystal.h>

enum class Squad{
  Red,
  Blue,
  None
};

//RS, ENABLE, D4,D5,D6,D7

LiquidCrystal g_RedLCD(12, 11, 10, 9, 8, 7); // Define LCD1 pins
LiquidCrystal g_BlueLCD(6, 5, 4, 3, 2, 1); // Define LCD2 pins

int g_RedPin = A0; // Connect your switch to analog pin A0
int g_BluePin = A1; // Connect your switch to analog pin A0

int g_RedCounter = 0; // Counter 1

int g_BlueCounter = 0; // Counter 2

int g_RedSwitchState = 0;
int g_BlueSwitchState = 0;

Squad g_LastSquad = Squad::None;

unsigned long g_CurrentTime = 0;

unsigned long g_DisplayTime = 0;

unsigned long g_DisplayInterval = 1000; // Update interval in milliseconds (1 second)

void setup() {
  g_RedLCD.begin(16, 2); // Initialize LCD1
  g_BlueLCD.begin(16, 2); // Initialize LCD2
  pinMode(g_RedPin, INPUT);
  pinMode(g_BluePin, INPUT);
}

void loop() 
{
  g_CurrentTime = millis();

  // Read the state of the switch
  g_RedSwitchState = digitalRead(g_RedPin);
  g_BlueSwitchState = digitalRead(g_BluePin);

  if (g_RedSwitchState)
    g_LastSquad = Squad::Red;
    
  if (g_BlueSwitchState)
    g_LastSquad = Squad::Blue;
    
  
  if (g_CurrentTime >= g_DisplayTime +g_DisplayInterval )
  {
    g_DisplayTime = g_CurrentTime;

    int hours   = 0;
    int minutes = 0;
    int seconds = 0;

  // If switch is in position 1, increment counter 1
    if (g_LastSquad == Squad::Red) 
    {
      g_RedCounter++;
    }

    if (g_LastSquad == Squad::Blue) 
    {
      g_BlueCounter++;
    }

    
    // Calculate and display the time for counter 1
    g_RedLCD.clear();
    g_RedLCD.setCursor(0, 0);
    g_RedLCD.print(" RED SQUAD: ");
    
    hours = (g_RedCounter / 3600) % 24;
    minutes = (g_RedCounter / 60) % 60;
    seconds = (g_RedCounter) % 60;

    g_RedLCD.setCursor(0, 1);
    g_RedLCD.print("Time: ");
    g_RedLCD.print(hours);
    g_RedLCD.print(":");
    if (minutes < 10) {
      g_RedLCD.print("0");
    }
    g_RedLCD.print(minutes);
    g_RedLCD.print(":");
    if (seconds < 10) {
      g_RedLCD.print("0");
    }
    g_RedLCD.print(seconds);

    // Calculate and display the time for counter 2
    g_BlueLCD.clear();
    g_BlueLCD.setCursor(0, 0);
    g_BlueLCD.print("BLUE SQUAD: ");


    g_BlueLCD.setCursor(0, 1);
    hours = (g_BlueCounter / 3600) % 24;
    minutes = (g_BlueCounter / 60) % 60;
    seconds = (g_BlueCounter) % 60;
    g_BlueLCD.print("Time: ");
    g_BlueLCD.print(hours);
    g_BlueLCD.print(":");
    if (minutes < 10) {
      g_BlueLCD.print("0");
    }
    g_BlueLCD.print(minutes);
    g_BlueLCD.print(":");
    if (seconds < 10) {
      g_BlueLCD.print("0");
    }
    g_BlueLCD.print(seconds);
  }
}