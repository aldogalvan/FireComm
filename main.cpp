#include <SerialPort.h>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

using namespace LibSerial ;

// Instantiate two SerialPort objects.
SerialPort serial_port; // serial stream seems to provide a more intuitive interface

float myArray[] = {1.23f, 4.56f, 7.89f, 2.34f, 5.67f, 8.90f, 3.45f};

/**
 * @brief This example demonstrates multiple methods to read and write
 *        serial stream data.
 */

bool readData();
bool writeData(float* data);
bool newData = false;

int main()
{

    // Open the Serial Ports at the desired hardware devices.
    serial_port.Open("/dev/ttyACM0") ;

    // Set the baud rates.
    serial_port.SetBaudRate(BaudRate::BAUD_500000) ;

    // Set the number of data bits.
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

    // Set the hardware flow control.
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;

    // Set the parity.
    serial_port.SetParity(Parity::PARITY_NONE) ; //kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk

//    //Set the stop bit.                                 //kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
//    serial_port.SetStopBits(StopBits::STOP_BITS_1) ; //kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk

//    serial_port.FlushIOBuffers();
//    std::string initialClean;
//    for(int i = 0; i < 20; i++){
//        serial_port.ReadLine(initialClean);
//    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    serial_port.FlushInputBuffer();
    while(1)
    {
        // this function reads serial data
        writeData(myArray);

        // this function writes serial data
//        readData();

//        // Variable to hold user input.
//        std::string user_input ;
//        user_input.clear() ;
//
//        // Get input from the user.
//        std::getline(std::cin, user_input) ;
//
//        if (user_input == "q" ||
//            user_input == "Q" ||
//            user_input == "")
//        {
//            break ;
//        }
    }

    serial_port.Close();

    return EXIT_SUCCESS ;
}

//bool readData()
//{
//    static bool recvInProgress = false;
//    static int ndx = 0;
//    char startMarker = '<';
//    char endMarker = '>';
//    char rc;
////    int index = 0;
//    char input[1];
//    int count = 0;
//    int numChars = 43;
//    char receiver[numChars];
//    serial_port.ReadByte(input[0]);
//    while (newData == false) {
//        rc = input[0];
//        if (recvInProgress == true) {
//            if (rc == ' ') {
//                count++;
//            }
//            if (rc != endMarker) {
//                receiver[ndx] = rc;
//                ndx++;
//                if (ndx >= numChars) {
//                    ndx = numChars - 1;
//                }
//            } else {
//                if (count == 6) {
//                    receiver[ndx] = '\0'; // terminate the string
//                    recvInProgress = false;
//                    ndx = 0;
//                    newData = true;
//                } else {
//                    count = 0;
//                    recvInProgress = false;
//                    ndx = 0;
//                }
//            }
//        } else if (rc == startMarker) {
//            recvInProgress = true;
//        }
//        serial_port.ReadByte(input[0]);
//    }
//    std::cout << receiver << std::endl;
//    serial_port.FlushInputBuffer();
//
//////    serial_port.FlushInputBuffer();
////    std::string data;
////    char filter1;
////    char filter;
////    char starter = '<';
////    bool isFirstChar = false;
////    while(1){
////        if(serial_port.IsDataAvailable()) {
////            serial_port.ReadByte(filter);
////            if((starter == filter) && (!isFirstChar)){
////                std::cout << "Please work";
////                isFirstChar = true;
////            }
////            else if(isFirstChar) {
////                serial_port.ReadLine(data);
////                break;
////            }
////        }
////    }
////    data.insert(0, "<");
//////    if((data.length() > 30) && (data.length() < 37)) {
////        std::cout << data << std::endl << std::flush;
//////    }
//////    std::cout << std::endl;
//return 1;
//}

bool writeData(float* data)
{

    char buffer[50] = "<"; // enough space for 7 floats
//    sprintf( buffer, "%.2f", data );
    int offset = 1; // An offset to track the position in the buffer

    for (int i = 0; i < 7; i++) {
        int written = std::sprintf(buffer + offset, "%.2f", data[i]); // Format the float with 2 decimal places
        offset += written;

        if (i < 6) {
            // Add a space as a delimiter for all elements except the last one
            buffer[offset] = ' ';
            offset++;
        }
    }
    buffer[offset] = '>';
    offset++;
    buffer[offset] = '\n';
    serial_port.Write(buffer);

//    std::cout << buffer << std::endl;
//    std::cout <<   "WRITE DONE" <<std::endl;

    return 1;
}