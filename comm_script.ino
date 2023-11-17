
// void setup() {
//   Serial.begin(500000);
// }

// void loop() {
//   Serial.println("<1.23 4.56 7.89 2.34 5.67 8.90 3.45 >");
//   delay(1000);
// }

#include <digitalWriteFast.h>
#include "DualVNH5019MotorShield.h"
#include <pt.h>   // include protothread library
#define BAUD_RATE 500000
#define POT_PIN A2
#define c_LeftEncoderInterrupt 3
#define c_LeftEncoderPinA 3
#define c_LeftEncoderPinB 13  
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
volatile long _LeftEncoderTicks = 0;

float data[] = {10.23, 40.56, 70.89, 20.34, 50.67, 80.90, 30.45};

// DualVNH5019MotorShield md;
int thetaY = 0;
int thetaZ = 0;
int newthetaY = 0;
int newthetaZ = 0;
int newTorque1 = 0;
int newTorque2 = 0;
int motorTorque1 = 0;
int motorTorque2 = 0;
int current = 0;
int newcurrent = 0;
const int buttonPin = 5;
int buttonState = 0;
const byte numChars = 36;
char receivedChars[numChars];
String inputString = "";
boolean stringComplete = false;
boolean newData = false;
int dataNumber = 0;
static struct pt pt3;


void setup() {
  // pinMode(c_LeftEncoderPinA, INPUT);      // sets pin A as input
  // digitalWrite(c_LeftEncoderPinA, LOW);  // turn on pullup resistors
  // pinMode(c_LeftEncoderPinB, INPUT);      // sets pin B as input
  // digitalWrite(c_LeftEncoderPinB, LOW);  // turn on pullup resistors
  // pinMode(buttonPin, INPUT);            // make button pin an input pin
  // // attachInterrupt(digitalPinToInterrupt(c_LeftEncoderInterrupt), HandleLeftMotorInterruptA, RISING);
  // inputString.reserve(200);
  // md.init();
  // PT_INIT(&pt1);  // initialise the two
  // PT_INIT(&pt2);  // protothread variables
  PT_INIT(&pt3);  // initialise the two
  // PT_INIT(&pt4);  // protothread variables
  Serial.begin(BAUD_RATE);
}
// static int protothreadAngle(struct pt *pt, int interval) {
//   static unsigned long timestamp = 0;
//   PT_BEGIN(pt);

//   Serial.println("Angle Protothread started");

//   while(1) {
//     PT_WAIT_UNTIL(pt, micros() - timestamp > interval);
//     timestamp = micros();
//     newthetaZ = analogRead(POT_PIN);
//     newthetaZ = map(newthetaZ,186,531,-90,90);
//     newthetaY = _LeftEncoderTicks;
//     newcurrent = md.getM1CurrentMilliamps();
//   }

//   Serial.println("Angle Protothread ended");

//   PT_END(pt);
// }
// static int protothreadMotor (struct pt *pt, int interval) {
//   static unsigned long timestamp = 0;
//   PT_BEGIN(pt);

//   Serial.println("Motor Protothread started");

//   while(1) {
//     PT_WAIT_UNTIL(pt, newTorque1 != motorTorque1 || (micros() - timestamp > interval));
//     timestamp = micros();
//     if (newTorque1 == 1000) {
//       md.setM1Brake(-400);
//     } else {
//       if (abs(newTorque1) > 60) {
//         newTorque1 = -60; 
//       }
//       motorTorque1 = newTorque1;
//       md.setM1Speed(motorTorque1);
//     }
//   }

//   Serial.println("Motor Protothread ended");

//   PT_END(pt);
// }

static int protothreadInput(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  // Serial.println("Input Protothread started");

  while(1) {
    PT_WAIT_UNTIL(pt,(micros() - timestamp > interval));
    timestamp = micros();
    // Serial.flush();
    recvWithStartEndMarkers();
    if (newData == true) {
      // dataNumber = 0;
      // dataNumber = atoi(receivedChars);
      // newTorque1 = dataNumber;
      newData = false;
    }
    Serial.println(receivedChars);
    // Serial.println("work!");
  }

  Serial.println("Input Protothread ended");

  PT_END(pt);
}

// static int protothreadOutput(struct pt *pt, int interval) {
//   static unsigned long timestamp = 0;
//   PT_BEGIN(pt);

//   // Serial.println("Output Protothread started");
//   static String deliveredString = "";
//   while(1) {
//     PT_WAIT_UNTIL(pt,(micros() - timestamp > interval));
//     timestamp = micros();
    
//     deliveredString += "<";
//     for(int i = 0; i < 7; i++){
//       if(i<6) {
//         deliveredString += String(String(data[i]).toFloat()) + " ";
//       } else {
//         deliveredString += String(String(data[i]).toFloat());
//       }
//     }
//     deliveredString += ">";
    
//     Serial.print(deliveredString);
//     deliveredString = "";
//   }

//   Serial.println("Output Protothread ended");

//   PT_END(pt);
// }

// // Interrupt service routines for the left motor's quadrature encoder
// void HandleLeftMotorInterruptA()
// {
//   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
//   _LeftEncoderBSet = digitalReadFast(c_LeftEncoderPinB);   // read the input pin
 
//   // and adjust counter + if A leads B
//   #ifdef LeftEncoderIsReversed
//     _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
//   #else
//     _LeftEnc0oderTicks += _LeftEncoderBSet ? -1 : +1;
//   #endif
// }

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    static byte count = 0;
    char rc;

    while (Serial.available() && newData == false) {
       rc = Serial.read();
       if (recvInProgress == true) {
           if (rc == ' ') {
               count++;
           }
           if (rc != endMarker) {
               receivedChars[ndx] = rc;
               ndx++;
               if (ndx >= numChars) {
                   ndx = numChars - 1;
               }
           } else {
               if (count == 6) {
                   receivedChars[ndx] = '\0'; // terminate the string
                   recvInProgress = false;
                   ndx = 0;
                   newData = true;
               } else {
                   count = 0;
                   recvInProgress = false;
                   ndx = 0;
               }
           }
       } else if (rc == startMarker) {
           recvInProgress = true;
       }
   }

    // while (Serial.available() > 0 && newData == false) {
    //     rc = Serial.read();
    //     if (recvInProgress == true) {
    //         if (rc != endMarker) {
    //             receivedChars[ndx] = rc;
    //             ndx++;
    //             if (ndx >= numChars) {
    //                 ndx = numChars - 1;
    //             }
    //         }
    //         else {
    //             receivedChars[ndx] = '\0'; // terminate the string
    //             recvInProgress = false;
    //             ndx = 0;
    //             newData = true;
    //         }
    //     }
    //     else if (rc == startMarker) {
    //         recvInProgress = true;
    //     }
    // }
}


void loop() {
  // stopIfFault();
  // protothreadAngle(&pt1, 500);     //Angle Proto-Thread
  protothreadInput(&pt3, 2000);     //Serial Input Proto-Thread
  // protothreadMotor(&pt2, 500);         //Motor Proto-Thread
  // protothreadOutput(&pt4, 2000);  //Serial Output Proto-Thread
}

















// #include <pt.h>

// #define BAUD_RATE 115200
// #define numVibrationMotors 6
// #define numFlexSensors 7
// static struct pt pt1, pt2;

// //Used for Serial Communication Inputting Data
// String raw_input[numVibrationMotors];
// boolean newData;
// // bool stringComplete = false;  // whether the string is complete 

// //Used to hold Flex Sensor Angle Data Input & Output
// double data_in[numVibrationMotors] = {1, 3, 6, 7, 9, 2.5}; //temporarily filled in while testing
// double data_out[numFlexSensors] = {1.01, 2.02, 3.03, 8.08, 5.05, 6.06, 7.07}; //temporarily filled in while testing

// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/

// // SEND DATA TO SERIAL PORT (INPUT PROTOTHREAD)
// static int protothreadInput(struct pt *pt, int interval) {
//   static unsigned long timestamp = 0;
//   PT_BEGIN(pt);
//   Serial.write("INIT SP1 \n");
//   while(1) {
//     Serial.write("START SP1 \n");
//     PT_WAIT_UNTIL(pt,(micros() - timestamp > interval)); // What should the condition be?
//     timestamp = micros();
//     receiveSerialData();
//     if (newData == true) {
//       processSerialData();
//       newData = false;
//     }
//     // recvWithStartEndMarkers();
//     // if (newData == true) {
//     //   dataNumber = 0;
//     //   dataNumber = atoi(receivedChars);
//     //   newTorque1 = dataNumber;
//     // newData = false;
//     // }
//     Serial.write("END SP1 \n");
//   }
//   Serial.write("CLOSE SP1 \n");
//   PT_END(pt);
// }

// void receiveSerialData() {
//   // Will receive in form [___], [___], [___], [___], [___], [___], [___]
//   static boolean recvInProgress = false;
//   static byte index = 0;
//   static String inputString = "";
//   char startElementMarker = '[';
//   char endElementMarker = ']';
//   char beginBatchMarker = 'x';
//   char endBatchMarker = 'y';
//   char temp;

//   while(Serial.available() && newData == false) {
//     char temp = (char) Serial.read();
//     //recvInProgress
//     if(recvInProgress == true) {
//       if(temp != endBatchMarker) {
//         inputString += temp;
//         if(temp == endElementMarker){
//           raw_input[index] = inputString;
//           index++;
//           inputString = "";
//         }
//       }
//       //reached endBatchMarker
//       else {
//         index = 0;
//         recvInProgress = false;
//         newData = true;
//       }
//     }
//     //recvNotInProgress (yet)
//     else if(temp == beginBatchMarker){
//       recvInProgress = true;
//     }
//   }
// }

// void processSerialData() {
//   for (int i = 0; i < (sizeof(raw_input)/sizeof(raw_input[0])); i++) {
//     raw_input[i].remove(raw_input[i].length()-1);
//     raw_input[i].remove(0);
//   }
//   for (int i = 0; i < (sizeof(raw_input)/sizeof(raw_input[0])); i++) {
//     data_in[i]=raw_input[i].toFloat();
//   }
// }

// void printDoubleArray(double* array1) {
//   for (int i = 0; i < (sizeof(array1)/sizeof(array1[0])); i++) {
//     Serial.println("[" + String(array1[i]) + "]");
//   }
// }

// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/
// /****************************************************************************************************************************************************************************************************/


// // RECEIVE DATA FROM SERIAL PORT (INPUT PROTOTHREAD)
// static int protothreadOutput(struct pt *pt, int interval) {
//   static unsigned long timestamp = 0;

//   //Gets data_out[] in proper form to be sent out
//   static String strings_delivered[sizeof(data_out)/sizeof(data_out[0])];
//   for(int i = 0; i < (sizeof(data_out)/sizeof(data_out[0])); i++) {
//     strings_delivered[i] = ("[" + String(data_out[i]) + "]");
//   }

//   Serial.write("INIT SP2 \n");

//   PT_BEGIN(pt);
//   while (1) {
//     Serial.write("START SP2 \n");

//     PT_WAIT_UNTIL(pt,(micros() - timestamp > interval)); // What should the condition be?
//     timestamp = micros();
//     Serial.println('x');
//     //Send the flex sensor angle positions
//     for(int i = 0; i < (sizeof(strings_delivered)/sizeof(strings_delivered[0])); i++) {
//       Serial.println(strings_delivered[i]);
//     }
//     Serial.println('y');
//     break;
//     // char outputString[20];
//     // thetaY = newthetaY;
//     // thetaZ = newthetaZ;
//     // current = newcurrent;
//     // sprintf(outputString, "<%d,%d>", thetaY,thetaZ); //send current for some reason
//     // Serial.write(outputString);
//     // Serial.write("1 \n");
//       Serial.write("INIT SP2 \n");
//   }
//   Serial.write("CLOSE SP2 \n");
//   PT_END(pt);
// }

// void setup() {
//   PT_INIT(&pt1); //initialize the two
//   PT_INIT(&pt2); // protothread variables
//   Serial.begin(115200, SERIAL_8E1);
//   // reserve 200 bytes for the inputString:
//   // inputString.reserve(1000);
//   //  delay(1000);
//   //  pinMode(LED_BUILTIN, OUTPUT);
//   //  pinMode(12, OUTPUT);

//   //  digitalWrite(13, HIGH);
//   //  delay(5000);
//   //  digitalWrite(13, LOW);
// }

// void loop() {
//   // TODO: Output data_out to be read by ext
//   // int temp[5];
//   // print the string when a new line/array arrives
//   printDoubleArray(data_in);
//   protothreadInput(&pt1, 250);     //Serial Input Proto-Thread
//   protothreadOutput(&pt2, 250);  //Serial Output Proto-Thread
//   delay(10000);
//   // TODO: Read back processed data and write it out to verify

// }



// // // Declare an array
// // int data_out[5] = {1 , 2, 3, 4, 5};

// // int data_in[5];

// // String inputString = "";         // a String to hold incoming data
// // bool stringComplete = false;  // whether the string is complete

// /****************************************************************************SAMPLE CODE*******************************************************************************************************/
// /****************************************************************************SAMPLE CODE*******************************************************************************************************/
// /****************************************************************************SAMPLE CODE*******************************************************************************************************/
// /****************************************************************************SAMPLE CODE*******************************************************************************************************/
// /****************************************************************************SAMPLE CODE*******************************************************************************************************/
// /****************************************************************************SAMPLE CODE*******************************************************************************************************/

// /*
//   Serial Event example

//   When new serial data arrives, this sketch adds it to a String.
//   When a newline is received, the loop prints the string and clears it.

//   A good test for this is to try it with a GPS receiver that sends out
//   NMEA 0183 sentences.

//   NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
//   other ATmega32U4 based boards.

//   created 9 May 2011
//   by Tom Igoe

//   This example code is in the public domain.

//   https://www.arduino.cc/en/Tutorial/BuiltInExamples/SerialEvent
// */

// // void loop() {
// //   // print the string when a newline arrives:
// //   if (stringComplete) {
// //     Serial.println(inputString);
// //     // clear the string:
// //     inputString = "";
// //     digitalWrite(12, HIGH);
// //     delay(1000);
// //     digitalWrite(12, LOW);
// //     stringComplete = false;
// //   }
// //   Serial.println("Hello");
// //   delay(500);
// // }

// /*
//   SerialEvent occurs whenever a new data comes in the hardware serial RX. This
//   routine is run between each time loop() runs, so using delay inside loop can
//   delay response. Multiple bytes of data may be available.
// */
// // void serialEvent() {
// //   static boolean recvInProgress = false;
// //   static byte index = 0;
// //   char startMarker = '[';
// //   char endMarker = ']';
// //   char temp;

// //   while (Serial.available() && newData == false) {
// //     // add it to the inputString:
// //     inputString += inChar;
// //     // if the incoming character is a newline, set a flag so the main loop can do something about it:
// //     if (inChar == ']') {
// //       newdata = true;
// //     }
// // //    digitalWrite(12, HIGH);
// // //    delay(500);
// // //    digitalWrite(12, LOW);
// //   }
// // }