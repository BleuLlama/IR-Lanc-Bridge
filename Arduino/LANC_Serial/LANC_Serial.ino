/*  Sony VTR IR -> LANC interface
 *   
 *   2020-04-30 yorgle@gmail.com
 *   
 * Remote Codes:
 * Sony VTR codes can be found in my spreadsheet here:
 *   https://docs.google.com/spreadsheets/d/1t3htAkWQzkkg0GTj4lvOOrYaGf4XkqisefB_9nhRVms/edit?usp=sharing
 * Other remote codes: 
 *   http://lirc.sourceforge.net/remotes/
 * 
 * LANC code started from the arduino-lANC-RS232 code
 * IR code started from example code with that library
 * Lookup table stuff is all me.
 */

// IR Stuffs
#include <IRremote.h>
#define RECV_PIN (2)
IRrecv irrecv(RECV_PIN);

decode_results results;


/*
Arduino LANC<->RS232 interface
Version 1.0
For communicating with cameras via LANC
For the interface circuit interface see 
http://controlyourcamera.blogspot.com/2011/02/arduino-controlled-video-recording-over.html

"LANC" is a registered trademark of SONY.
CANON calls their LANC compatible port "REMOTE".

Written by L.Rosén

------------------------------------------------------------------------------------------
Comments regarding service mode for Sony second generation D8 camcorders:
DCR-TRV8000E, DCR-TRV8100E, DCR-TRV120E, DCR-TRV125E, DCR-TRV320E, DCR-TRV325E
DCR-TRV420E, DCR-TRV520E, DCR-TRV620E, DCR-TRV725E

LANC message layout when reading/writing EEPROM(8 bytes each sent with LSB first)
B0 B1 B2 B3 B4 B5 B6 B7

B0 = First sent byte from our adapter
B1 = Second sent byte from our adapter
B2
B3
B4 = The 4 highest bits b7..b4 tells which page in the EEPROM you are at
B5 = Confirmation that the command has been received, read command confirmed with F0h, write commands confirmed with F1h
B6 = Tells which address in the EEPROM you are at
B7 = Data at address

The following commands is used to navigate the EEPROM and change data
B1 B2
FFh 00h = Read command, tells you current page:address:data without changing anything
FFh 67h = Increase page by 1
FFh 65h = Decrase page by 1
FFh 38h = Increase address by 1
FFh 36h = Decrase address by 1
FFh 34h = Increase data by 1
FFh 30h = Decrase data by 1
FFh 32h = STORE command

Metod for checksums (PAGE:ADDRESS:DATA):
1) enable changes in memory: 00:01:00 to 00:01:01 (Store)
2) change data on page D, how You need (all with STORE).
3) read new value: 
"xx" from 02:F0
"yy" from 02:F1
4) enable update and visibility of checksums on (0F:FE and 0F:FF):
00:FF:00 -> 00:FF:02 (STORE)
00:01:01 -> 00:01:80 (STORE)
5) update new checksums:
write to address:
0F:FF data "xx" ( from 02:F0 ) (STORE)
0F:FE data "yy" ( from 02:F1 ) (STORE)
6) disable changes:
00:FF:02 -> 00:FF:00 (STORE)
00:01:80 -> 00:01:00 (STORE)

Links to more information:
http://lea.hamradio.si/~s51kq/DV-IN.HTM
http://www.sps.volyne.cz/set1394/anin/code20.html

------------------------------------------------------------------------------------------
*/



// The code uses fast I/O write because its time critical,
// therefore setting pins are done by writing directly to the registers:
#define cmdPinON (PORTD = B10000000)   // Set digtal pin 7 (PD7)
#define cmdPinOFF (PORTD = B00000000)  // Reset digtal pin 7 (PD7)
#define ledON (PORTB = B00100000)      // Set LED pin 13 (PB5)
#define ledOFF (PORTB = B00000000)     // Reset LED pin 13 (PB5)
#define lancPinREAD (PINB & B00001000) // Reads pin 11 (PB3)
#define lancPin 11

int bitDura = 104;           // Duration of one LANC bit in microseconds, orginal 104
int halfbitDura = 52;        // Half of bitDura
byte strPointer = 0;         // Index when receiving chars
char inString[5];            // A string to hold incoming data
char outString[25];          // A string to hold outgoing data
boolean strComplete = false; // Indicator to see if the string is complete
boolean lancCmd[16];         // Array for the lancCmd in bits
boolean lancMessage[64];     // Array for the complete LANC message in bits

void setup() {
  DDRD = DDRD | B10000000;    // Config cmdPin as output
  DDRB = DDRB & B11110111;    // Config lancPin as input
  DDRB = DDRB | B00100000;    // Config ledPin as output
  pinMode(lancPin, INPUT);    // Listens to the LANC line, used for pulseIn function 
  cmdPinOFF;                  // Reset LANC control pin so that the LANC line is unaffected(HIGH)
  Serial.begin(57600);        // Start serial port  
  Serial.println("Welcome to the Arduino LANC-RS232 interface");
  Serial.println("Send two bytes in hex form etc. 02AF and wait for reply from camera");

  Serial.println( "Starting IR Receiver." );
  
  irrecv.enableIRIn(); // Start the receiver
}

// code lookup table
//  for each item;
//    [0] is the Sony IR code for VTR1
//    [1] is the Sony IR code for VTR2
//    [2] is the Sony IR code for VTR3
//    [3] is the LANC code to send (or special code)
//    if [0] == 0xffff then that's the end of the list
//    if [3] == 0xf001 then it's 'switch to vtr1'
//    if [3] == 0xf002 then it's 'switch to vtr2'
//    if [3] == 0xf003 then it's 'switch to vtr3'

unsigned int codelut[][4] = 
{
  // TV
  // VTR1,  VTR2,  VTR3,  LANC 
  { 0xc90, 0xc90, 0xc90, 0x1826 }, // TV volume -
  { 0x490, 0x490, 0x490, 0x1824 }, // TV volume +
  
  // VTR 1
  { 0x548, 0x55c, 0x55a, 0x1876 }, // megabass
 //{0x548, 0x55c, 0x55a, 0x1854 }, // TV/VTR (IGNORED)
  { 0xa88, 0xa9c, 0xa9a, 0x185e }, // power
  { 0xf28, 0xf3c, 0xf3a, 0x189e }, // input select
  
  { 0x1a8, 0x1bc, 0x1ba, 0x18b0 }, // tape speed
  { 0x5a8, 0x5bc, 0x5ba, 0x18b4 }, // data screen
  { 0x628, 0x63c, 0x63a, 0x188c }, // counter reset
  
  { 0x6e5, 0x6f3, 0x6eb, 0x1876 }, // voice boost / megabass
  { 0x888, 0x89c, 0x89a, 0x1826 }, // channel - (volume -)
  { 0x088, 0x09c, 0x09a, 0x1824 }, // channel + (volume +)
  
  { 0xa68, 0xa7c, 0xa7a, 0x183c }, // timer rec (recessed) -> record-pause
  
  { 0x588, 0x59c, 0x59a, 0x1834 }, // play
  { 0x188, 0x19c, 0x19a, 0x1830 }, // stop
  { 0xd88, 0xd9c, 0xd9a, 0x1836 }, // rew
  { 0x388, 0x39c, 0x39a, 0x1838 }, // ffwd
  { 0x148, 0x15c, 0x15a, 0x1850 }, // search rew
  { 0x948, 0x95c, 0x95a, 0x1852 }, // search ffwd
  { 0x988, 0x99c, 0x99a, 0x1832 }, // pause
  { 0xc48, 0xc5c, 0xc5a, 0x1846 }, // slow
  { 0x288, 0x29c, 0x29a, 0x1828 }, // X2
  { 0xb88, 0xb9c, 0xb9a, 0x183a }, // REC
  { 0x0c8, 0x0dc, 0x0da, 0x1860 }, // direction <  step
  { 0x8c8, 0x8dc, 0x8da, 0x1862 }, // direction >

  { 0x008, 0x01c, 0x01a, 0xF001 }, // keypad 1 -> VTR1
  { 0x808, 0x81c, 0x81a, 0xF002 }, // keypad 2 -> VTR2
  { 0x408, 0x41c, 0x41a, 0xF003 }, // keypad 3 -> VTR3
  
  { 0xffff, 0xffff }, // end of list.
};

unsigned int vtrbank = 0;

void doCommandCode( unsigned int code )
{
  switch( code ) {
    case( 0xF001 ):
      vtrbank = 0;
      break;
      
    case( 0xF002 ):
      vtrbank = 1;
      break;
      
    case( 0xF003 ):
      vtrbank = 2;
      break;
  }
}


void handleIRCode( unsigned int ircode )
{
  char buf[10];

  // go through each item in the lookup table for the IR code 
  //  if found, send the LANC code.
  int idx = 0;
  while( codelut[ idx ][ 0 ] != 0xffff )
  {
    if( codelut[ idx ][ vtrbank ] == ircode ) {
      
      unsigned int code = codelut[ idx ][ 3 ];
      
      if( (code & 0xf000) == 0xf000 ) {
        // it's a special command code
        doCommandCode( code );
        return;
      }
      
      sendCode( code );
      return;
    }

    idx++;
  }
}

void sendCode( unsigned int code )
{
  cmdToBitArray( code );
  
  sendLanc(4);                       // The LANC command needs to be repeated 4 times
  bitarraytohexchar();               // Convert received bitarray to hex chars
  for (int i=0; i<24 ;i++) {         // Write back LANC message over serial
    Serial.print(outString[i]);
  }
  Serial.print('\n');

  for (int i=0 ; i<5 ; i++) {           // Clear input array
    inString[i] = 0; 
  }
  strComplete = false;                  // Reset data received flag
 
}

void loop() {
  
  if (irrecv.decode(&results)) {
    if( results.decode_type == SONY ) {
      handleIRCode( results.value );
    }
    irrecv.resume(); // get the next signal
  }

 if (strComplete) {                     // inString has arrived
   if (hexchartobitarray()) {           // Convert hex chars to bitarray
     sendLanc(4);                       // The LANC command needs to be repeated 4 times
     bitarraytohexchar();               // Convert received bitarray to hex chars
     for (int i=0; i<24 ;i++) {         // Write back LANC message over serial
       Serial.print(outString[i]);
     }
     Serial.print('\n');
   }
   else {
     Serial.println("Faulty input!");
   }

   for (int i=0 ; i<5 ; i++) {           // Clear input array
    inString[i] = 0; 
   }
   strComplete = false;                  // Reset data received flag
 }
 
}


void bitarraytohexchar() {
 // The bit array lancMessage contains the whole LANC message (8 bytes) with LSB first
 // This function converts them to hex chars and stores them in outString (16 chars)
 
 byte temp = 0;

 for ( int i=0 ; i<8 ; i++) {      // Byte loop
   
   for ( int j=0 ; j<4 ; j++) {    // Bit loop
     temp += (pow2(j) * lancMessage[(j+4)+i*8]);
   }
   outString[i*3] = bytetohexchar(temp);
   temp = 0;
   
   for ( int j=0 ; j<4 ; j++) {    // Bit loop
     temp += (pow2(j) * lancMessage[j+i*8]);
   }
   outString[i*3+1] = bytetohexchar(temp);
   outString[i*3+2] = ' ';
   temp = 0;
 }
 
 outString[24] = '\n';
 
}


void cmdToBitArray( unsigned int cmd )
{
  unsigned int byte1 = (cmd >> 8) & 0x0ff;
  unsigned int byte2 = (cmd     ) & 0x0ff;
  
  for (int i = 0 ; i < 8 ; i++ ) { 
    lancCmd[i] = bitRead( byte1, i );
  }
  for (int i = 0 ; i < 8 ; i++ ) { 
    lancCmd[i + 8] = bitRead( byte2, i );
  }
}

boolean hexchartobitarray() {
 // The hex code in char (4 chars) is located in inString
 // This function fills the lancCmd array with the bits in the order they should be sent
 // First byte 1 then byte 2 but with LSB first for both bytes

 int byte1, byte2;
 
 for (int i = 0 ; i < 4 ; i++ ) {
  if (!(isHexadecimalDigit(inString[i]))) {
    return 0;
  }
 }

 byte1 = (hexchartoint(inString[0]) << 4) + hexchartoint(inString[1]);
 byte2 = (hexchartoint(inString[2]) << 4) + hexchartoint(inString[3]);

 for (int i = 0 ; i < 8 ; i++ ) { 
   lancCmd[i] = bitRead(byte1,i);      // Reads one bit from a number, x is number, n is position (0 is LSB)
 }
 for (int i = 0 ; i < 8 ; i++ ) { 
   lancCmd[i + 8] = bitRead(byte2,i);  // Reads one bit from a number, x is number, n is position (0 is LSB)
 }
 
 return 1;
}


void sendLanc(byte repeats) {
   // This function is time critical and optimized for Arduino Pro Mini
   // It takes ~3.2us for the arduino to set a pin state with the digitalWrite command
   // It takes ~80ns for the arduino to set pin state using the direct register method
   // delayMicroseconds(50) ~ 49us, delayMicroseconds(100) ~ 99us

   int i = 0;
   int bytenr = 0;

noInterrupts();

   while (pulseIn(lancPin, HIGH) < 5000) {   
   // Sync to next LANC message
   // "pulseIn, HIGH" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V 
   // "pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
   }

   while (repeats) {
   
   i = 0;
   bytenr = 0;
   
   ledON;                                       // LANC message LED indicator on

   
   for ( ; bytenr<8 ; bytenr++) {
     delayMicroseconds(bitDura-7);              // LOW after long pause means the START bit of Byte 0 is here, wait START bit duration
     for ( ; i<(8*(bytenr+1)) ; i++) {
       if (bytenr<2) {
         if (lancCmd[i]) {                      // During the first two bytes the adapter controls the line and puts out data
           cmdPinON;                            
         }
         else {
           cmdPinOFF;
         }
       }
       delayMicroseconds(halfbitDura);
       lancMessage[i] = !lancPinREAD;           // Read data line during middle of bit
       delayMicroseconds(halfbitDura); 
     }
     cmdPinOFF;
     if (bytenr == 7) {
       ledOFF;
     }
     delayMicroseconds(halfbitDura);            // Make sure to be in the stop bit before waiting for next byte
     while (lancPinREAD) {                      // Loop as long as the LANC line is +5V during the stop bit or between messages
     }
   }
   repeats--;
   }      
   interrupts();               
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
 while (Serial.available()) {
   char inChar = (char)Serial.read();                               // Get the new byte
   inString[strPointer++] = inChar;                                 // Add it to the input string
   if ((inChar == '\n') || (inChar == '\r') || (strPointer > 4)) {  // If the incoming character is a newline, carriage return or 4 bytes has been received flag so the main loop can act
     strComplete = true;
     strPointer = 0;
   }
 } 
}


int pow2(int x) {
  switch (x) {
     case 0:
       return 1;
       break;
     case 1:
       return 2;
       break;
     case 2:
       return 4;    
       break;
     case 3:
       return 8;
       break;
     case 4:
       return 16;
       break;
     case 5:
       return 32;
       break;
     case 6:
       return 64;    
       break;
     case 7:
       return 128;
       break;
     default:
       return 0;
       break;
  }
}


char bytetohexchar(byte hexbyte) {
  switch (hexbyte) {
   case 15:
     return 'F';
     break;
   case 14:
     return 'E';
     break;
   case 13:
     return 'D';    
     break;
   case 12:
     return 'C';
     break;
   case 11:
     return 'B';
     break;
   case 10:
     return 'A';
     break;
   default:
     return (hexbyte + 48);
     break;
 }
}


int hexchartoint(char hexchar) {
 switch (hexchar) {
   case 'F':
     return 15;
     break;
   case 'f':
     return 15;
     break;
   case 'E':
     return 14;
     break;
   case 'e':
     return 14;
     break;
   case 'D':
     return 13;    
     break;
   case 'd':
     return 13;    
     break;
   case 'C':
     return 12;
     break;
   case 'c':
     return 12;
     break;
   case 'B':
     return 11;
     break;
   case 'b':
     return 11;
     break;
   case 'A':
     return 10;
     break;
   case 'a':
     return 10;
     break;
   default:
     return (int) (hexchar - 48);
    break;
 }
}
