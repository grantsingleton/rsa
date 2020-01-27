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

    ifstream inFile;
    inFile.open("banks.txt");

    if (!inFile) {
        cerr << "Unable to open banks.txt";
        exit(1);
    }

    string incompleteMessage = "How much to ";
    string message = "";
    string bank = "";

    while (inFile) {
        // plug in bank and encrypt
        getline(inFile, bank);
        message = incompleteMessage + bank + "?\n";

        const char* messageArray = message.c_str();

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
        string encryptCommand = "openssl rsautl -encrypt -raw -in paddedMessage.txt -out message.txt -inkey presidentpub.pem -pubin";
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
            //cout << "Test: " <<  testLine.substr(0,3) << endl;

        string encryptedline = "";
        while (in) {
            // plug in bank and encrypt
            getline(in, encryptedline, '=');
            //cout << "Test: " <<  testLine[0] << endl;
            //cout << encryptedline.substr(0,3) << endl;

            //cout << "Line: " << endl << encryptedline << endl << endl;
            
            if (encryptedline.length() > 1) {
                //cout << "line: " <<  encryptedline.substr(0,3) << endl;

                if (testLine.substr(0,2) == encryptedline.substr(0,2)) {
                    cout << "MATCH" << endl;
                    cout << paddedString << endl;
                    cout << "test: " << endl << testLine << endl << endl;
                    cout << "line: " << endl << encryptedline << endl;
                }
                //cout << "line: " <<  encryptedline.length() << endl << encryptedline << endl << endl;             
            } 


        }
        //return 0;
        in.close();
        in2.close();
    }


    inFile.close();

    return 0;
}