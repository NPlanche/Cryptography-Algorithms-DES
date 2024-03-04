// DES Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;
//Global Vars
string keys[16];

//identifiers
void key_generation(string K);
string pc_1(string key);
string shift_once_left(string key_part);
string shift_twice_left(string key_part);

int main()
{
    std::cout << "Hello World!\n";
    //Stepts 
    // 1. Key Generation
    // 2. Plaintext (64 bits) to Initial Permutation
    // 3. Initial Permutation (64 bits) 
    // 4. Divide into Left0 and Right0 (32 bits)
    // 5. F function given K1 (48 bits) and Right0 --> result 32bit
    // 6. XOR Left0 and result 32bit
    // 7. Switch Orientation (L0 -> R1 and R0 -> L1)
    // 8. Repeat 5-7 steps with k2 - k16
    // 9. ---> L16 and R16
    // 10. Unite L16 and R16 for Final Permutation
    // 11. Final Permutation 
    // 12. Ciphertext

   // 1. Key Generation
   //TODO: change key: randomize it
   //64 bit key
    string key =  "1010101010111011000010010001100000100111001101101100110011011101";
    key_generation(key);
}

void key_generation(string K) {

    //TODO: Change PC-1 AND PC-2 tables
    //PC-1 table 56 
    string new_key = pc_1(K);

    //Divide key into two halfs
    string left = new_key.substr(0, 28);
    string right = new_key.substr(28, 28);

    //16 rounds of shifting
    for (int i = 0; i < 16; i++) {
        // Rounds 1, 2,9,16 rotate the two halves once
        if (i == 0 || i == 1 || i == 8 || i == 15) {
            left = shift_once_left(left);
            right = shift_once_left(right);
        }
        //Rotate keys, two bits left 
        else {
            left = shift_twice_left(left);
            right = shift_twice_left(right);
        }
        string combined_key = left.substr(0, 28) + right.substr(0, 28);
        string key_num = "";

        //PC-2 table 48
        int pc_2[48] = {
            14, 17, 11, 24, 1, 5, 3, 28,
            15, 6, 21, 10, 23, 19, 12, 4,
            26, 8, 16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55, 30, 40,
            51, 45, 33, 48, 44, 49, 39, 56,
            34, 53, 46, 42, 50, 36, 29, 32
        };

        for (int j = 0; j < 48; j++) {
            //cout << "j: " << j << ", pc_2[j] - 1: " << pc_2[j] - 1 << endl;
            key_num += combined_key[pc_2[j] - 1];
        }

        keys[i] = key_num;
        cout << "Key " << i + 1 << ": " << keys[i] << endl;
    }
    
}

string pc_1(string key) {
    //PC-1 table 56 
    int pc_1[56] = {
        57, 49, 41, 33,25,17, 9,
        1 ,58 ,50 ,42 ,34 ,26 ,18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11 ,3 ,60 ,52 ,44 ,36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };


    // Trim down from 64 -> 56 bits 
    string new_key = "";

    for (int i = 0; i < 56; i++) {
        new_key += key[pc_1[i] - 1];
    }

    return new_key;
}
string shift_once_left(string key_part) {
    string shifted = "";
    // Shift every bit once to the left 
    for (int i = 1; i < 28; i++) {
        shifted += key_part[i];
    }
    // Add the first bit to the end
    shifted += key_part[0];
    return shifted;
}
string shift_twice_left(string key_part) {
    //shift bits twice
    return shift_once_left(shift_once_left(key_part));
}




