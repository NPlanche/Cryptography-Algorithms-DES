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

string DES();
string initial_permutation();
string expansion(string right);
string Xor(string key_round, string exp);
int binaryToDec(string b);
string decimalToBin(int dec);
string final_permutation(string combined);
string decryption(string ciphertext);
string generateBinaryString(int N);

int main()
{
   // 1. Key Generation ( 64 bit key )
    string key = generateBinaryString(64);
    key_generation(key);

    string plaintext = "";
    cout << "Secret Message: ";
    cin >> plaintext;

    pt = ptToBinary(plaintext);
    cout << "Binary pt:" << pt << endl;

    string cyphertext = DES();
    cout << "Cyphertext:" << cyphertext << endl;

    string decrypted_pt = decryption(cyphertext);
    cout << "Decrypted plaintext: " << decrypted_pt << endl;
}

/// <summary>
/// key_generation: This fuction is the Key Scheduler and generates keys 1 to 16 
/// </summary>
/// <param name="K"></param>
void key_generation(string K) {

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
        //cout << "Key " << i + 1 << ": " << keys[i] << endl;
    }
    
}
/// <summary>
/// pc_1: Compressess the key using the pc-1 table
/// </summary>
/// <param name="key"></param>
/// <returns>new_key</returns>
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
/// <summary>
/// shift_once_left: Shift the given bits to the left once
/// </summary>
/// <param name="key_part"></param>
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
/// <summary>
/// shift_twice_left: Shift the given bits to the left twice
/// </summary>
/// <param name="key_part"></param>
string shift_twice_left(string key_part) {
    //shift bits twice
    return shift_once_left(shift_once_left(key_part));
}
/// <summary>
/// ptToBinary: converts from text to binary
/// </summary>
/// <param name="pt"></param>
/// <returns>binary</returns>
string ptToBinary(string pt) {
    string binary;
    
    for (char c : pt) {
        // Convert each character into its binary representation (8 bits)
        binary += std::bitset<8>(c).to_string();
    }

    // Pad with leading zeros if necessary to make it 64 bits
    while (binary.length() < 64) {
        binary = "0" + binary;
    }

    return binary;
}
/// <summary>
/// DES: Implements the DES encryption algorithm
/// </summary>
/// <returns>ciphertext</returns>
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

    // 2. Plaintext (64 bits) to Initial Permutation
    string ip = initial_permutation();

    // The permutation table
    int permutation_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
    };

    // 4. Divide into Left0 and Right0 (32 bits)
    string left = ip.substr(0, 32);
    string right = ip.substr(32, 32);

    //Encryption 16 rounds 
    for (int i = 0; i < 16; i++) {

        // 5. F function given K1 (48 bits) and Right0 --> result 32bit
        string right_expansion = expansion(right);
        // 6. XOR Left0 and result 32bit
        string xor_exp = Xor(keys[i], right_expansion);
        string result = "";


        //s-boxes
        for (int j = 0; j < 8; j++) {
            string r = xor_exp.substr(j * 6, 1) + xor_exp.substr(j * 6 + 5, 1);
            int row = binaryToDec(r);

            string c = xor_exp.substr(j * 6 + 1, 1) + xor_exp.substr(j * 6 + 2, 1) + xor_exp.substr(j * 6 + 3, 1) + xor_exp.substr(j * 6 + 4, 1);;
            int col = binaryToDec(c);
            int val = s_boxes[j][row][col];
            result += decimalToBin(val);
        }

        //second permutation
        string second_perm = "";
        for (int z = 0; z < 32; z++) {
            second_perm += result[permutation_table[z] - 1];
        }

        xor_exp = Xor(second_perm, left);

        // 7. Switch Orientation (L0 -> R1 and R0 -> L1)

        left = xor_exp;

        if (i < 15) {
            string x = right;
            right = xor_exp;
            left = x;
        }
    }

    //combine the two halves of plaintext
    string combined = right + left;
    // 10. Unite L16 and R16 for Final Permutation
    string ciphertext = final_permutation(combined);
    // 12. Ciphertext
    return ciphertext;
}
/// <summary>
/// initial_permutation: 3Performs the initial permutation
/// </summary>
/// <returns></returns>
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
/// <summary>
/// expansion: preform the expansion of the right side of the plain text 
/// </summary>
/// <param name="right"></param>
/// <returns>right_expansion</returns>
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
/// <summary>
/// Xor: preforms an XOR operation
/// </summary>
/// <param name="key_round"></param>
/// <param name="exp"></param>
/// <returns>r</returns>
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
/// <summary>
/// binaryToDec: convert the given binary into decimal
/// </summary>
/// <param name="b"></param>
/// <returns>decimal</returns>
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
/// <summary>
/// decimalToBin: converts the given decimal into binary
/// </summary>
/// <param name="dec"></param>
/// <returns>binary</returns>
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
/// <summary>
/// final_permutation: 
/// </summary>
/// <param name="combined"></param>
/// <returns></returns>
string final_permutation(string combined) {
    string cipher = "";
    int ip_inverse[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58 ,26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    for (int i = 0; i < 64; i++) {
        cipher += combined[ip_inverse[i] - 1];
    }
    return cipher;
}
/// <summary>
/// decryption: Decrypt the given cypher text by reversing the order the keys and passing the results through the DES algorithm/// </summary>
/// <param name="ciphertext"></param>
/// <returns>decrypted_pt</returns>
string decryption(string ciphertext) {

    pt = ciphertext;

    int keysSize = sizeof(keys) / sizeof(keys[0]);

    int keys_start = 0;
    int keys_end = keysSize - 1;
    while (keys_start < keys_end) {
        swap(keys[keys_start], keys[keys_end]);
        keys_start++;
        keys_end--;
    }

    string decrypted_pt = DES();
    return decrypted_pt;
}
/// <summary>
/// The functions below where source from geeksforgeeks.
/// Given this was appropiate because generating a key was not the focus of this assigment
///  and the professor said we could utilize extenal souces or functions 
/// </summary>
/// <returns></returns>
int findRandom()
{
    // Generate the random number
    int num = ((int)rand() % 2);

    // Return the generated number
    return num;
}
string generateBinaryString(int N)
{
    srand(time(NULL));

    // Stores the empty string
    string S = "";

    // Iterate over the range [0, N - 1]
    for (int i = 0; i < N; i++) {

        // Store the random number
        int x = findRandom();

        // Append it to the string
        S += to_string(x);
    }

    // Print the resulting string
    return S;
}