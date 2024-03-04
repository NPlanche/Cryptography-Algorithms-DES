// DES Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <bitset>
using namespace std;
//Global Vars
string keys[16];
string pt;
//identifiers
void key_generation(string K);
string pc_1(string key);
string shift_once_left(string key_part);
string shift_twice_left(string key_part);
string ptToBinary(string pt);
string Xor (string right_expansion);

int main()
{
    //Stepts 
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
    //key_generation(key);

    string plaintext = "";
    cout << "Secret Message: ";
    cin >> plaintext;

    string pt = ptToBinary(plaintext);
    cout << "Binary pt:" << pt << endl;

    string cyphertext = DES();

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
string ptToBinary(string pt) {
    string binary;
    for (char c : pt) {
        binary += bitset<8>(c).to_string();
    }

    return binary;
}

string DES() {

    string ip = initial_permutation();

    //divide into two halves
    string left = ip.substr(0, 32);
    string right = ip.substr(32, 32);

    //Encryption
    string right_expansion = expansion(right);
    string xor_exp = Xor(keys[i], right_expansion);
    

    
}

string initial_permutation() {

    int ip[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
    };

    string temp = "";

    for (int i = 0; i < 64; i++) {
        temp += pt[ip[i] - 1];
    }

    return temp;
}

string expansion(string right) {

    //expansion table
    int exp[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    string right_expansion = "";
    for (int i = 0; i < 48; i++) {
        right_expansion += right[exp[i] - 1];

    }

    return right_expansion;
}
string Xor (string key_round,string exp){
    {
        string r = "";
        for (int i = 0; i < exp.size(); i++) {
            if (key_round[i] != exp[i]) {
                r += "1";
            }
            else {
                r += "0";
            }
        }

        return r;
    }
