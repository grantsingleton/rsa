#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <map>

//General goal, take the provided banks and pad them to 256 bytes using the
//provided format. 
//Then encrypt and encode and look for exact matches within the provided encoded text.

using namespace std;

int main(){
    map<int, string> output;    //map to be used to format output

    //read in the banks list. Throw an error if unable to
    ifstream bankTxt;
    bankTxt.open("banks.txt");
    if(!bankTxt){
        cerr << "infile error";
        exit(1);
    }

    //vector of banks filled from the list
    vector<string> banks;
    string bankIn;
    while(bankTxt){
        getline(bankTxt, bankIn);
        banks.push_back(bankIn);
    }

    //for each bank, format and encrypt it and compare with the messages from 
    //the problem
    string template1 = "How much to ";      //template string to match format
    for(int i = 0; i<banks.size(); ++i){
        string messageFilled = template1 + banks.at(i) + "?\n"; //str to compare
        char paddedMessage[256] = {char (0)};   //256 byte string of null
        for(int j = 0; j < messageFilled.size(); ++j){
            paddedMessage[j] = messageFilled.at(j); //plug in message, leaving filled rightmost bytes
        }

        string filledString(paddedMessage); //convert to string for display purposes at the end

        ofstream ofile;
        ofile.open("filledMsg.txt");    //write to file to be encrypted

        //ofile << paddedMessage;

        for(int i = 0; i<sizeof(paddedMessage); ++i){
            ofile << paddedMessage[i];
        }

        ofile.close();

        //encrypts the file using RSA using linux commands
        string encryptCommand = "openssl rsautl -encrypt -raw -in filledMsg.txt -out message.txt -inkey presidentpub.pem -pubin";
        const char * encrypt = encryptCommand.c_str();
        system(encrypt);

        //converts to base 64 
        string encodeCommand = "openssl base64 -in message.txt -out encryptedMsg.txt";
        const char * encode = encodeCommand.c_str();
        system(encode);

        //read in the 2 files
        ifstream input1, input2;
        input1.open("messages.txt");
        input2.open("encryptedMsg.txt");

        string problemCode, myCode;
        getline(input2, myCode);
        int counter = 0;

        //iterate through the provided message and look for matches
        while(input1){
            getline(input1, problemCode, '=');
            if(problemCode.length() > 1){
                counter++;
                if(myCode.substr(0,5) == problemCode.substr(0,5)){
                    cout << "Matched: " << filledString << " at line " << counter << endl;
                    //output[counter] = filledString; //if match, print to console and push to map
                    output[counter] = banks.at(i);
                }
            }
        }
        //close inputs
        input1.close();
        input2.close();
    }
    //use ordered map to cleanly print output
    //while iterating through the map, also print the bank names to txt file
    //txt file then formatted when decrypting the amounts in the next c++ prog.
    ofstream ofile2;
    ofile2.open("bankList.txt");
    for(map<int, string >::const_iterator it = output.begin(); it != output.end(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
        ofile2 << it->second <<"\n";
    }
    ofile2.close();
    return 0;
}
