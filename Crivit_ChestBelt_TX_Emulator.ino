#include <TimerOne.h>

/* 
 *  This program will emulate the chest belt radio-transmitter
 *    of the Crivit Sports wrist-watch Heart Rate Monito (HRM).
 *    
 *  It was designed to manually type a string of 0s and 1s in the
 *    Serial Monitor. This string will modulate the 110KHz radio carrier,
 *    and will be emitted about twice per second. The radio signal 
 *    can be received by a Crivit Sports HRM wrist watch.
 *    
 *  The value of the emitted Beats Per Minute (BPM) can be changed 
 *    by typing strings of 0s and 1s in the serial monitor.
 *    - the beginning Sync bit is always prepended
 *    - if the typed string length is less then 15, then a default radio ID
 *      string "111100" is prepended.
 *      E.g. if you type "0100010010101", then "S 111100 0100010010101" 
 *      will be transmitted. This signal will correspond to 65 BPM.
 *    - if you manually type more then 14 bits, then the whole typed string of bits 
 *      will be radio transmitted verbatim.
 *    - if you just press ENTER ('/n'), then a default valid value of 63 BPM is sent
 *      together with the default radio ID "S 111100 0011111111000" 
 *          
 *  The program was made to allow various bits combinations to be emited 
 *    in order to understand the radio protocol used by the wrist watch.
 *    
 *  The radio TX antenna consists of a very long wire or a big coil 
 *    connected to the Arduino pin 9. The pinout is set for Arduino Uno.
 *    
 *  'TimerOne' library is required, but it is not includded in the standard distribution
 *    of Arduino 1.8.1. The library must be installed in order to compile successfully. 
 *    Installation can be done by Arduino from the IDE's menu 'Sketch' -> 'Include Library' ->
 *    -> 'Manage Libraries...', or manually downlaoded from https://github.com/PaulStoffregen/TimerOne
 *    then installed from the IDE's menu 'Sketch' -> 'Include Library' -> 'Add .ZIP Library...'
 */

#define CARRIER_PIN     9     // Carrier output (antenna) pin can be 9 or 10 for Arduino Uno
#define CARRIER_DUTY    512   // 50% duty cycle. Range 0...1024
#define CARRIER_PERIOD  9     // 1/9us ~ 110KHz

char  txID[] = "111100";
String radioTxDataString = "0100010010101"; // Initial value set to display 65

String inputString = "";            // will hold the string received from the serial port
bool stringComplete = false;        // flag set when the string is complete ('\n' encounter)

// bool ledState = LOW;



void setup() {
  // put your setup code here, to run once:

  // Timer1 is set to output 110KHz at pin 9 of the Arduino Uno. This will be the radio carrier.
  Timer1.initialize(CARRIER_PERIOD);      //Radio carrier should be ~ 110KHz
  Timer1.pwm(CARRIER_PIN, CARRIER_DUTY);  //pwm() must be used at least once before setPwmDuty()
  Timer1.setPwmDuty(CARRIER_PIN, 0);      //stop carrier output
  
  Serial.begin(9600);
  inputString.reserve(200);
  Serial.println(radioTxDataString);  
  
//  pinMode(LED_BUILTIN, OUTPUT);
//  ledState = LOW;
}



void radioTxSyncBit() {
  delay(50);                                    //wait at least 50ms from any previous TX
  Timer1.setPwmDuty(CARRIER_PIN, CARRIER_DUTY); //start carrier for 5ms
  delayMicroseconds(5000);
  Timer1.setPwmDuty(CARRIER_PIN, 0);            //stop carrier
}

void radioTxDataBit(bool dataBit) {
  delayMicroseconds(4800);
  if (dataBit) {
    Timer1.setPwmDuty(CARRIER_PIN, CARRIER_DUTY); //start carrier for 3ms
  }
  delayMicroseconds(3000);
  Timer1.setPwmDuty(CARRIER_PIN, 0);              //stop carrier
}

void radioTxDataStream(String dataString) {
  for (int i=0; i<dataString.length(); i++) {
    radioTxDataBit(dataString[i]=='1');
  }
}

void radioTxIdStream(){
  radioTxSyncBit();
  radioTxDataStream(txID);
}

void radioTxByte(byte value) {
  
}

/*
 SerialEvent() is called automatically between each time loop() runs,
 but ONLY IF at least one character is available in the RX buffer.
 */
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      inputString += inChar;
    } else {
      stringComplete = true;
    }
  }
}



// main loop
void loop() {
  // put your main code here, to run repeatedly:
  
  delay(500);

  if (stringComplete) {
    radioTxDataString = inputString;

    // If you just press ENTER ('/n'), then a default valid value of 63 BPM is sent
    //   together with the default radio ID, "111100"
    if (radioTxDataString.length() == 0) {
      radioTxDataString = "0011111111000";
    }
    Serial.println(radioTxDataString);
    inputString = "";
    stringComplete = false;
  }

  // If you send more then 14 bits, then the default radio ID will not be prepended
  //   so you can specify the whole data radio stream, except for the starting SyncBit
  if(radioTxDataString.length() > 14) {
    radioTxSyncBit();
  } else {
    radioTxIdStream();
  }
  radioTxDataStream(radioTxDataString);
}

