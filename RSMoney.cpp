#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <map>

using namespace std;

int main(){
    map<int, string> output;
    for(int value = 1; value <= 999; ++value){
        string formattedVal = '$' + to_string (value) + "B\n";
        const char* valueArray = formattedVal.c_str();
        //string stringVal(valueArray);
        char nullArray[256] = {char (0)};

        for(int i = 0; i < sizeof(valueArray); ++i){
            nullArray[i] = valueArray[i];
        }

        ofstream ofile;
        ofile.open("myNum.txt");

        for(int i = 0; i < sizeof(nullArray); ++i){
            ofile << nullArray[i];
        }
        ofile.close();

        string encryptCommand = "openssl rsautl -encrypt -raw -in myNum.txt -out message.txt -inkey treasurypub.pem -pubin";
        const char * encrypt = encryptCommand.c_str();
        system(encrypt);

        string encodeCommand = "openssl base64 -in message.txt -out myEncryptedNum.txt";
        const char * encode = encodeCommand.c_str();
        system(encode);

        ifstream input1, input2;
        input1.open("messages.txt");
        //input2.open("myEncryptedNumber.txt");
        input2.open("myEncryptedNum.txt");
        if(!input1 || !input2){
            cerr << "infile error";
            exit(1);
        }
        string myNumLine, problemLine;
        getline(input2, myNumLine);
        int counter = 0;
        while(input1){
            getline(input1, problemLine, '=');
            if(problemLine.length() > 1){
                counter++;
                if(myNumLine.substr(0, 5) == problemLine.substr(0, 5)){
                    cout << "Matched: " << formattedVal << " at line " << counter << endl;
                    output[counter] = to_string(value) + "B";
                }
            }
        }
        input1.close();
        input2.close();
    }
    ofstream ofile2;
    ofile2.open("bankNums.txt");
    for(map<int, string >::const_iterator it = output.begin(); it != output.end(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
        ofile2 << it->second <<"\n";
    }
    ofile2.close();

    //code to merge the two text files
    ifstream input1;
    input1.open("bankList.txt");
    ofstream ofile3;
    ofile3.open("RSAOutput.txt");
    for(int i = 0; i<10; ++i){
        string tempBank;
        getline(input1, tempBank);
        ofile3 << tempBank << "\n" << output[2*(i+1)] << "\n";
    }
    //ofile3 << output[20];
    input1.close();
    ofile3.close();
    return 0;
}