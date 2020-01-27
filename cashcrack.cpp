#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h> 

using namespace std;

int main() {

    //Process: 
    // 1. Convert message to binary and add bull bytes to get 256 byts. 
    // 2. Encrypt the message
    // 3. encode the encrypted message in base64

    string cashString = "";
    string message = "";

    for (int cash = 1; cash <= 999; cash++) {
        // plug in cash and encrypt
       //for (int decimal = 0; decimal <= 9; decimal++) {
            cashString = '$' + to_string(cash) + "B\n";

            const char* messageArray = cashString.c_str();

            string msgString(messageArray);
            //cout << "msg: " << msgString << endl << "length: " << msgString.length();

            char padded[256] = {char (0)};
            for (int i = 0; i < msgString.length(); i++) {
                padded[i] = messageArray[i];
            }

            string paddedString(padded);
            //cout << "padded: " << paddedString << endl;

            ofstream myfile;
            myfile.open ("paddedMessage.txt");

            for (int i = 0; i < sizeof(padded); i++) {
                myfile << padded[i];

            }
            myfile.close();

/*
            ifstream in3;
            in3.open("paddedMessage.txt");
            string str = "";
            getline(in3, str);
            cout << str << endl;
            in3.close();
*/

            // Command for encrypting. Needs public key, binary input file ('key.bin' and output file name)
            string encryptCommand = "openssl rsautl -encrypt -raw -in paddedMessage.txt -out message.txt -inkey treasurypub.pem -pubin";
            const char * encrypt = encryptCommand.c_str();
            system(encrypt);

            string encodeCommand = "openssl base64 -in message.txt -out encodedmessage.txt";
            const char * encode = encodeCommand.c_str();
            system(encode);

            ifstream in;
            in.open("messages.txt");

            ifstream in2;
            in2.open("encodedMessage.txt");
            string testLine = "";
            getline(in2, testLine, '=');
            //cout << "last: " << testLine[testLine.length() - 1] << endl;
            //cout << "test: " << testLine.length() << endl << testLine << endl;
            //cout << "Test: " <<  testLine << endl;
                //cout << paddedString << endl;

            string encryptedline = "";
            int msgNum = 1;
            while (in) {
                // plug in bank and encrypt
                getline(in, encryptedline, '=');
                //cout << "Test: " <<  testLine[0] << endl;
                //cout << encryptedline << endl << endl;

                //cout << "Line: " << endl << encryptedline << endl << endl;s
                
                if (encryptedline.length() > 1) {
                    //cout << "test: " <<  testLine.substr(0,3) << endl;
                    //cout << "line: " <<  encryptedline.substr(0,3) << endl;
                    msgNum++;
                    if (testLine.substr(0,10) == encryptedline.substr(0,10)) {
                        cout << "MATCH" << endl;
                        cout << paddedString << endl;
                        cout << "Message Number: " << msgNum << endl;
                        cout << "----------------------------------------" << endl;
                    }
                    //cout << "line: " <<  encryptedline.length() << endl << encryptedline << endl << endl;             
                } 


            }
            //return 0;
            in.close();
            in2.close();
       // }
    }

    return 0;
}
