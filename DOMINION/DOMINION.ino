#define RADIO_ENABLED false

#if RADIO_ENABLED == true
#include <ELECHOUSE_CC1101.h>
#endif

#include <LiquidCrystal.h>

                            //1  GND
                            //2  VCC 3.3V
#define RADIO_GD0     2     //3
#define RADIO_CSN_SS  10    //4
#define RADIO_SCK     13    //5
#define RADIO_SI_MOSI 11    //6
#define RADIO_SO_MISO 12    //7
                            //8 DGO2 NOT USED

#if RADIO_ENABLED == false
  #define RED_LCD_RS_PIN 12
  #define RED_LCD_EN_PIN 11
  #define RED_LCD_D4_PIN 10
  #define RED_LCD_D5_PIN 9
  #define RED_LCD_D6_PIN 8
  #define RED_LCD_D7_PIN 7
#else
  #define RED_LCD_RS_PIN A2
  #define RED_LCD_EN_PIN A3
  #define RED_LCD_D4_PIN A4
  #define RED_LCD_D5_PIN 9
  #define RED_LCD_D6_PIN 8
  #define RED_LCD_D7_PIN 7
#endif

#if RADIO_ENABLED == false
  #define BLUE_LCD_RS_PIN 6
  #define BLUE_LCD_EN_PIN 5
  #define BLUE_LCD_D4_PIN 4
  #define BLUE_LCD_D5_PIN 3
  #define BLUE_LCD_D6_PIN 2
  #define BLUE_LCD_D7_PIN 1
#else
  #define BLUE_LCD_RS_PIN 6
  #define BLUE_LCD_EN_PIN 5
  #define BLUE_LCD_D4_PIN 4
  #define BLUE_LCD_D5_PIN 3
  #define BLUE_LCD_D6_PIN A5
  #define BLUE_LCD_D7_PIN 0
#endif

#define RED_SWITCH_PIN A0
#define BLUE_SWITCH_PIN A1

enum class Squad
{
  Red,
  Blue,
  None
};

//RS, ENABLE, D4,D5,D6,D7

LiquidCrystal g_RedLCD( RED_LCD_RS_PIN,  RED_LCD_EN_PIN,  RED_LCD_D4_PIN,  RED_LCD_D5_PIN,  RED_LCD_D6_PIN,  RED_LCD_D7_PIN); // Define LCD1 pins
LiquidCrystal g_BlueLCD(BLUE_LCD_RS_PIN, BLUE_LCD_EN_PIN, BLUE_LCD_D4_PIN, BLUE_LCD_D5_PIN, BLUE_LCD_D6_PIN, BLUE_LCD_D7_PIN); // Define LCD2 pins

int g_RedPin = RED_SWITCH_PIN; // Connect your switch to analog pin A0
int g_BluePin = BLUE_SWITCH_PIN; // Connect your switch to analog pin A0

unsigned long g_RedCounter = 0; // Counter 1
unsigned long g_BlueCounter = 0; // Counter 2

int g_RedSwitchState = 0;
int g_BlueSwitchState = 0;

Squad g_LastSquad = Squad::None;

unsigned long g_CurrentTime = 0;

unsigned long g_DisplayTime = 0;
unsigned long g_DisplayInterval = 1000; // Update interval in milliseconds (1 second)

unsigned long g_TransmitTime = 0;
unsigned long g_TransmitInterval = 1000; // Update interval in milliseconds (1 second)
unsigned long g_ALHOA = 1;

#define BUFFER_LEN
//R000000
byte g_Buffer[BUFFER_LEN] = "";

#define ID_LEN 8
byte g_SessionID[ID_LEN] ;

void generate_session_id()
{
  for (int i=0;i<ID_LEN;i++)
      g_SessionID[ID_LEN]= random(65, 90);
}

void setup() 
{
  g_RedLCD.begin(16, 2); // Initialize LCD1
  g_BlueLCD.begin(16, 2); // Initialize LCD2
  
  pinMode(g_RedPin, INPUT);
  pinMode(g_BluePin, INPUT);
  
  #if RADIO_ENABLED
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    ELECHOUSE_cc1101.Init(F_433);
    generate_session_id();
  #endif

}

#if RADIO_ENABLED
void Transmit ()
{
    g_TransmitTime = g_CurrentTime;

    //check if other are transmitting
    if (ELECHOUSE_cc1101.CheckReceiveFlag())
    {
      g_ALHOA *= 10; //increase transmit exp time 
    }
    else 
    {
      size_t  index = 0;
      //SSSSSSSS
      memcpy(g_Buffer, g_SessionID, ID_LEN);
      index+=ID_LEN;
      
      //Rtttt
      g_Buffer[index] = g_LastSquad == Squad::Red ? 1:0; 
      index++;
      memcpy(&g_Buffer[index], &g_RedCounter, sizeof(g_RedCounter));
      index += sizeof(g_RedCounter);
      
      //Btttt
      g_Buffer[index] = g_LastSquad == Squad::Blue ? 1:0; 
      index++;
      
      memcpy(&g_Buffer[index], &g_BlueCounter, sizeof(g_BlueCounter));
      index += sizeof(g_BlueCounter);
      
      //NULL
      g_Buffer[index] = '\0';
      index++;
      
      //send buffer
      ELECHOUSE_cc1101.SendData(g_Buffer, index);
    }
}
#endif


void Display()
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
    
  if (g_CurrentTime >= g_DisplayTime + g_DisplayInterval )
  {
    Display();
  }

  #if RADIO_ENABLED
  if (g_CurrentTime >= g_TransmitTime + g_TransmitInterval + g_ALHOA)
  {
    Transmit();
  }
  #endif
}
