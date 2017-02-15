# Crivit_ChestBelt_TX_Emulator

```cpp
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
```
