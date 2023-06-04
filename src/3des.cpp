#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <random>
#include <iostream>
#include <sstream>

#define LB32   0x00000001
#define LB64   0x0000000000000001
#define L64_MASK    0x00000000ffffffff
#define H64_MASK    0xffffffff00000000

static char IP[] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};


static char PI[] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};


static char E[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};


static char P[] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};


static char S[8][64] = { {
        /* S1 */
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
         0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
         4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },{
        /* S2 */
        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
         3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
         0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },{
        /* S3 */
        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
         1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },{
        /* S4 */
         7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
         3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },{
        /* S5 */
         2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
         4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },{
        /* S6 */
        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
         9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
         4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },{
        /* S7 */
         4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
         1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
         6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },{
        /* S8 */
        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
         1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
         7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
         2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    } };


static char PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};


static char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};


static char iteration_shift[] = {
    /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
       1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};


uint64_t des(uint64_t input, uint64_t key, char mode) {


    int i, j;

    /* 8 bit */
    char satir, sutun;

    /* 28 bits */
    uint32_t C = 0;
    uint32_t D = 0;

    /* 32 bit */
    uint32_t L = 0;
    uint32_t R = 0;
    uint32_t s_output = 0;
    uint32_t f_function_res = 0;
    uint32_t temp = 0;

    /* 48 bit */
    uint64_t sub_key[16] = { 0 };
    uint64_t s_input = 0;

    /* 56 bit */
    uint64_t permuted_choice_1 = 0;
    uint64_t permuted_choice_2 = 0;

    /* 64 bit */
    uint64_t init_perm_res = 0;
    uint64_t inv_init_perm_res = 0;
    uint64_t pre_output = 0;

    /* initial permutation */
    for (i = 0; i < 64; i++) {

        init_perm_res <<= 1;
        init_perm_res |= (input >> (64 - IP[i])) & LB64;




    }

    L = (uint32_t)(init_perm_res >> 32) & L64_MASK;
    // printf ("SOL 32 Bit: %llx\n",L);
    R = (uint32_t)init_perm_res & L64_MASK;
    // printf ("SAG 32 Bit: %llx\n",R);

    for (i = 0; i < 56; i++) {

        permuted_choice_1 <<= 1;//
        permuted_choice_1 |= (key >> (64 - PC1[i])) & LB64;

    }

    C = (uint32_t)((permuted_choice_1 >> 28) & 0x000000000fffffff);
    D = (uint32_t)(permuted_choice_1 & 0x000000000fffffff);
    //  printf ("SOL 56 Bit: %llx\n",C);
    //  printf ("SAG 56 Bit: %llx\n",D);

    for (i = 0; i < 16; i++) {



        for (j = 0; j < iteration_shift[i]; j++) {

            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);

        }

        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t)C) << 28) | (uint64_t)D;

        sub_key[i] = 0;

        for (j = 0; j < 48; j++) {

            sub_key[i] <<= 1;//32+32
            sub_key[i] |= (permuted_choice_2 >> (56 - PC2[j])) & LB64;

        }

    }



    for (i = 0; i < 16; i++) {


        s_input = 0;

        for (j = 0; j < 48; j++) {

            s_input <<= 1;
            s_input |= (uint64_t)((R >> (32 - E[j])) & LB32);

        }


        if (mode == 'd') {

            s_input = s_input ^ sub_key[15 - i];

        }
        else {

            s_input = s_input ^ sub_key[i];

        }


        for (j = 0; j < 8; j++) {


            satir = (char)((s_input & (0x0000840000000000 >> 6 * j)) >> 42 - 6 * j);
            satir = (satir >> 4) | satir & 0x01;

            sutun = (char)((s_input & (0x0000780000000000 >> 6 * j)) >> 43 - 6 * j);

            s_output <<= 4;
            s_output |= (uint32_t)(S[j][16 * satir + sutun] & 0x0f);

        }

        f_function_res = 0;

        for (j = 0; j < 32; j++) {

            f_function_res <<= 1;
            f_function_res |= (s_output >> (32 - P[j])) & LB32;

        }

        temp = R;
        R = L ^ f_function_res;
        L = temp;

    }

    pre_output = (((uint64_t)R) << 32) | (uint64_t)L;


    for (i = 0; i < 64; i++) {

        inv_init_perm_res <<= 1;
        inv_init_perm_res |= (pre_output >> (64 - PI[i])) & LB64;

    }

    return inv_init_perm_res;

}

//gen full key
void generate_random_hex_value(char* hex_value, int length) {
    char hex_chars[] = "0123456789ABCDEF";
    int hex_chars_length = sizeof(hex_chars) - 1;  // ¬иключаючи нуль-терм≥натор

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        int index = rand() % hex_chars_length;
        hex_value[i] = hex_chars[index];
    }

    hex_value[length] = '\0';  // Ќуль-терм≥натор
}

//create 3 keys
void split_char_value(const char* value, uint64_t parts[3]) {
    // –озбиваЇмо значенн€ на три частини по 7 символ≥в
    sscanf(value, "%7llx%7llx%7llx", &parts[0], &parts[1], &parts[2]);
}

//timestamp
uint64_t generate_timestamp() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    uint64_t timestamp = timeinfo->tm_year;  // «береженн€ року
    timestamp = (timestamp << 4) | timeinfo->tm_mon;  // «береженн€ м≥с€ц€
    timestamp = (timestamp << 5) | timeinfo->tm_mday;  // «береженн€ дн€
    timestamp = (timestamp << 5) | timeinfo->tm_hour;  // «береженн€ годин
    timestamp = (timestamp << 6) | timeinfo->tm_min;  // «береженн€ хвилин
    timestamp = (timestamp << 6) | timeinfo->tm_sec;  // «береженн€ секунд

    return timestamp;
}

//gen iod
uint32_t generate_six_digit_number() {
    std::random_device rd;
    std::mt19937 gen(rd() + (unsigned)time(nullptr));
    std::uniform_int_distribution<uint64_t> uid(0, 1000000);

    uint32_t number = uid(gen);  // Generate a random number between 0 and 999999

    

    return number;
}



int main(int argc, const char* argv[]) {


    int i;
    //uint64_t message;
    //uint64_t key[3] = { 0x9474B8E8C73BCA7D, 0x9474B8E8C73BCA7C, 0x9474B8E8C73BC97D };
    uint64_t key[3];
    
    
    char hex_key[22];
    

   

    

    //printf("\n--------------------------Encrypt DES & 3-DES--------------------------\n");
    //printf("Input hex message:");
    //scanf("%llx", &message);
    ////printf("Input three hex keys:");
    ////scanf("%llx %llx %llx", &key[0], &key[1], &key[2]);
    ////printf("Input: %016llx\n", input);
    //result1 = des(message, key[0], 'e');
    //printf("DES Encrytion: %016llx\n", result1);
    //result2 = des(result1, key[1], 'd');
    ////printf ("Des: %016llx\n", result);
    //result3 = des(result2, key[2], 'e');
    //printf("3Des Encrytion: %016llx\n", result3);

    //printf("\n--------------------------Decrypt DES & 3-DES--------------------------\n");
    //result_de = des(result1, key[0], 'd');
    //printf("Des Decryption: %016llx\n", result_de);
    //result_de1 = des(result3, key[2], 'd');
    //result_de2 = des(result_de1, key[1], 'e');
    ////printf ("Des: %016llx\n", result);
    //result_de3 = des(result_de2, key[0], 'd');
    //printf("3Des Decryption: %016llx\n", result_de3);


    

    //protocol

    //generate id`s

    uint64_t idA = generate_six_digit_number();
    uint64_t idB = generate_six_digit_number();
    printf("Alice id :  %llx", idA);
    printf("\nBob id : %llx", idB);

    //generate keys
    
    generate_random_hex_value(hex_key, 21);

    split_char_value(hex_key, key);

    std::cout << "\nKey parts: \n";
    printf("\nPart 1: %llx\n", key[0]);
    printf("Part 2: %llx\n", key[1]);
    printf("Part 3: %llx\n", key[2]);

    //id sending
    std::cout << "Alice and bob are exchanged theirs id`s....\n\n";

    //generate Alice timestamp
    uint64_t t_A = generate_timestamp();
    printf("Alice generating her timestamp....\nAlice timestamp :   %llx", t_A);
   
    std::cout << "\n\nAlice encrypt her timestamp, Bob id and send to Bob...\n\n";

    uint64_t result1, result2, e_T;
    result1 = des(t_A, key[0], 'e');
    result2 = des(result1, key[1], 'd');
    //printf ("Des: %016llx\n", result);
    e_T = des(result2, key[2], 'e');

    uint64_t gidBob;
    printf("Input bob ID:");
    scanf("%llx", &gidBob);
    uint64_t result1_1, result2_1, e_idB;
    result1_1 = des(gidBob, key[0], 'e');
    result2_1 = des(result1_1, key[1], 'd');
    //printf ("Des: %016llx\n", result);
    e_idB = des(result2_1, key[2], 'e');

    printf("\n\nEncrypted timestmp and id: %016llx", e_T);
    printf(" %016llx", e_idB);


    std::cout << "\n\nBob decrypt msg and check truthfulness...\n\n";

    //decrypt Alice timestamp
    uint64_t result_de1, result_de2, d_tA;
    result_de1 = des(e_T, key[2], 'd');
    result_de2 = des(result_de1, key[1], 'e');
    //printf ("Des: %016llx\n", result);
    d_tA = des(result_de2, key[0], 'd');

    //decrypted recived id from alice
    uint64_t result_de1_1, result_de2_2, D_idB;
    result_de1_1 = des(e_idB, key[2], 'd');
    result_de2_2 = des(result_de1_1, key[1], 'e');
    //printf ("Des: %016llx\n", result);
    D_idB = des(result_de2_2, key[0], 'd');


    printf("Decrypted Alice timestamp and recived id: %016llx", d_tA);
    printf(" %016llx", D_idB);

    //check timestamp
    std::cout << "\n\nCheking recived timestamp...\n\n";
    uint64_t t_B = generate_timestamp();
    printf("Bob timestamp :   %llx", t_B);

    if ((t_B - t_A) < 20) {
        std::cout << "\nTimestamp good cheking recived ID....";
        if (D_idB == idB) {
            
            std::cout << "\n\nRecived ID is good. Continue authentification...";

            std::cout << "\n\nBob create new timestamp, Alice id and send it to Alice...";
            uint64_t t_B1 = generate_timestamp();
            printf("\n\nBob new timestamp :   %llx", t_B1);

            uint64_t result1_3, result2_3, e_TB2;
            result1_3 = des(t_B1, key[0], 'e');
            result2_3 = des(result1_3, key[1], 'd');
            //printf ("Des: %016llx\n", result);
            e_TB2 = des(result2_3, key[2], 'e');

            uint64_t gidA;
            printf("\n\nInput Alice ID:");
            scanf("%llx", &gidA);
            uint64_t result1_4, result2_4, e_idA2;
            result1_4 = des(gidA, key[0], 'e');
            result2_4 = des(result1_4, key[1], 'd');
            //printf ("Des: %016llx\n", result);
            e_idA2 = des(result2_4, key[2], 'e');

            printf("\n\nEncrypted timestmp and id: %016llx", e_TB2);
            printf(" %016llx", e_idA2);

            std::cout << "\n\nEncrypted message sended to Alice";

            std::cout << "\n\nAlice decrypt msg and check truthfulness...\n\n";

            //decrypt bob timestamp
            uint64_t result_de1_3, result_de2_3, d_tA3;
            result_de1_3 = des(e_TB2, key[2], 'd');
            result_de2_3 = des(result_de1_3, key[1], 'e');
            //printf ("Des: %016llx\n", result);
            d_tA3 = des(result_de2_3, key[0], 'd');

            //decrypted recived id from alice
            uint64_t result_de1_4, result_de2_4, D_idA3;
            result_de1_4 = des(e_idA2, key[2], 'd');
            result_de2_4 = des(result_de1_4, key[1], 'e');
            //printf ("Des: %016llx\n", result);
            D_idA3 = des(result_de2_4, key[0], 'd');

            printf("\n\nDecrypted Bob timestamp and recived id: %016llx", d_tA3);
            printf(" %016llx", D_idA3);

            //check timestamp
            std::cout << "\n\nCheking recived timestamp...\n\n";
            uint64_t last_t = generate_timestamp();
            printf("\n\nAlice timestamp :   %llx", last_t);
            if ((last_t - d_tA3) < 20) {

                std::cout << "\n\nTimestamp good cheking recived ID....";
                if (D_idA3 == idA) {
                    std::cout << "\n\nRecived ID is good. Authentification completed successfully!\n\n";
                    return 0;
                    
                }
                else {
                    std::cout << "\n\nRecived ID from Bob is incorrect. Authentification FAILED!";
                    return 0;
                }
            
            }
            else
            {
                std::cout << "\n\nAuthentification failed.Old Bob timestamp\n\n";
                return 0;
            }
        
        }
        else
        {
            std::cout << "\n\nRecived ID from Alice is incorrect. Authentification FAILED!";
            return 0;
        }
    
    }
    else
    {
        std::cout << "\n\nAuthentification failed.Old Alice timestamp\n\n";
        return 0;
    }

    return 0;

}