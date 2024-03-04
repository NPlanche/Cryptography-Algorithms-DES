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

    //Sunstitution Boxes
    int s_boxes[8][4][16] = {
        {
            //S1
            14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
            0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
            4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
            15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        {
            //S2
            15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
            3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
            0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
            13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        {
            //S3
            10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
            13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
            13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12 ,5, 10, 14, 7,
            1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        {
            //S4
            7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
            13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
            10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
            3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7 ,2 ,14

        },
        {
            //S5
            2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
            14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
            4, 2, 1 ,11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
            11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        {
            //S6
            12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
            10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
            9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
            4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        {
            //S7
            4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
            13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
            1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
            6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 1
        },
        {
            //S8
            13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
            1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
            7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
            2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        },
    };

    //initial permutations
    string ip = initial_permutation();

    //second permutation
    int permutation_table[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25
    };

    //divide into two halves
    string left = ip.substr(0, 32);
    string right = ip.substr(32, 32);

    //Encryption 16 rounds 

    for (int i = 0; i < 16; i++) {
        string right_expansion = expansion(right);
        string xor_exp = Xor(keys[i], right_expansion);
        string r = "";
        //s-boxes
        for (int i = 0; i < 8; i++) {
            string r = xor_exp.substr(i * 6, 1) + xor_exp.substr(i * 6 + 5, 1);
            int row = binaryToDec(r);

            string c = xor_exp.substr(i * 6 + 1, 1) + xor_exp.substr(i * 6 + 2, 1) + xor_exp.substr(i * 6 + 3, 1) + xor_exp.substr(i * 6 + 4, 1);;
            int col = binaryToDec(c);
            int val = s_boxes[i][row][col];
            r += decimalToBin(val);

        }

        //second permutation
        string second_perm = "";
        for (int i = 0; i < 32; i++) {
            second_perm += r[permutation_table[i] - 1];
        }
    }

    
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
int binaryToDec(string b) {
    int decimal = 0;
    int count = 0;

    for (int i = b.length() - 1; i >= 0; i--) {
        if (b[i] == '1') {
            decimal += pow(2, count);
        }
        count++;
    }

    return decimal;
};
string decimalToBin(int dec) {
    string binary;
    while (dec != 0) {
        binary = (dec % 2 == 0 ? "0" : "1") + binary;
        dec = dec / 2;
    }
    while (binary.length() < 4) {
        binary = "0" + binary;
    }
    return binary;
}