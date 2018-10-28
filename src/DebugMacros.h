
//***************************************************************
//   Example of use: 
//   #define DEBUG  //                              <---<<< this line must appear before the include line
//
//   #include <DebugMacros.h>
// 
//If you comment the line:    #define DEBUG
//the Macro lines are defined as blank, thus would be ignored by the compiler
//#define DEBUG  // if this line is NOT commented, these macros will be included in the sketch
//examples:
//  This  converts to >>>>----------------------->  This OR a Blank Line.  
// DPRINTLN("Testing123");                          Serial.println("Testing123");  
// DPRINTLN(0xC0FFEEul,DEC);                        Serial.println(0xC0FFEEul,DEC); 
// DPRINTLN(12648430ul,HEX);                        Serial.println(12648430ul,HEX); 
// DPRINTLNF("This message came from your flash");  Serial.println(F("This message came from your flash"));
// DPRINT(myVariable);                              Serial.print(myVariable);
// DELAY(100);                                      delay(100);
// PINMODE(9600);                                   pinMode(9600);
// TOGGLEd13;                                       PINB = 0x20;  // D13 Toggle,for UNO ONLY
//
// Also, this works  #define INFO(...)  { Console->printf("INFO: "); Console->printf(__VA_ARGS__); }   >>>--->   where {} allows multiple lines of code.
// See: http://forum.arduino.cc/index.php?topic=511393.msg3485833#new

#ifdef DEBUG
//examples:
//#define DPRINT(args...)  Serial.print(args)  OR use the following syntax:
#define SERIALBEGIN(...)   Serial.begin(__VA_ARGS__)
#define DPRINT(...)        Serial.print(__VA_ARGS__)
#define DPRINTLN(...)      Serial.println(__VA_ARGS__)
#define DRINTF(...)        Serial.print(F(__VA_ARGS__))
#define DPRINTLNF(...)     Serial.println(F(__VA_ARGS__)) //printing text using the F macro
#define DELAY(...)         delay(__VA_ARGS__)
#define PINMODE(...)       pinMode(__VA_ARGS__)
#define TOGGLEd13          PINB = 0x20                    //UNO's pin D13

#define DEBUG_PRINT(...)   Serial.print(F(#__VA_ARGS__" = ")); Serial.print(__VA_ARGS__); Serial.print(F(" ")) 
#define DEBUG_PRINTLN(...) DEBUG_PRINT(__VA_ARGS__); Serial.println()

//***************************************************************
#else
#define SERIALBEGIN(...)  
#define DPRINT(...)       
#define DPRINTLN(...)     
#define DPRINTF(...)      
#define DPRINTLNF(...)    
#define DELAY(...)        
#define PINMODE(...)      
#define TOGGLEd13      

#define DEBUG_PRINT(...)    
#define DEBUG_PRINTLN(...)  

#endif
//***************************************************************
