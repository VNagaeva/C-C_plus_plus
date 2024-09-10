#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct enigma_t {
    int size_rotor;
    int num_rotors;
    char *reflector;
    char **rotors;
} enigma_t;

extern enigma_t *constructor(int size_rotor, int num_rotors) {
    enigma_t *enigma = (enigma_t*)malloc(sizeof(enigma_t));

    enigma->size_rotor = size_rotor;
    enigma->num_rotors = num_rotors;

    enigma->reflector = (char*)malloc(sizeof(char)*size_rotor);
    enigma->rotors = (char**)malloc(sizeof(char*)*num_rotors);

    for (int i = 0; i < num_rotors; ++i) {
        enigma->rotors[i] = (char*)malloc(sizeof(char)*size_rotor);
    }

    return enigma;
}

extern void set_reflector(enigma_t *enigma, char *reflector) {
    for (int i = 0; i < enigma->size_rotor; ++i) {
        enigma->reflector[i] = reflector[i];
    }
}

extern void set_rotor(enigma_t *enigma, int num, char *rotor) {
    for (int i = 0; i < enigma->size_rotor; ++i) {
        enigma->rotors[num][i] = rotor[i];
    }
}

static int get_alpha_code(char letter) {
    return letter - 'A';
}

static int get_index_alpha(enigma_t *enigma, char letter, int num_rotor) {
    for (int i = 0; i < enigma->size_rotor; ++i) {
        if (letter == enigma->rotors[num_rotor][i]) {
            return i;
        }
    }
}

extern char *crypt(enigma_t *enigma, const char *phrase, char *key, char *alphabet) {
    int R0 = get_alpha_code(key[0]);
    int R1 = get_alpha_code(key[1]);
    int R2 = get_alpha_code(key[2]);

    int length = strlen(phrase);
    char *cypher = (char*)malloc(sizeof(char)*length);

    for (int i = 0; i < length; ++i) {
        char code = alphabet[(enigma->size_rotor + (get_index_alpha(enigma, phrase[i], 2) + R2)) % enigma->size_rotor];
        code = alphabet[(enigma->size_rotor + (get_index_alpha(enigma, code, 1) + R1)) % enigma->size_rotor];
        code = alphabet[(enigma->size_rotor + (get_index_alpha(enigma, code, 0) + R0)) % enigma->size_rotor];

        code = enigma->reflector[get_alpha_code(code)];
        code = enigma->rotors[0][(enigma->size_rotor + (get_alpha_code(code) - R0)) % enigma->size_rotor];
        code = enigma->rotors[1][(enigma->size_rotor + (get_alpha_code(code) - R1)) % enigma->size_rotor];
        code = enigma->rotors[2][(enigma->size_rotor + (get_alpha_code(code) - R2)) % enigma->size_rotor];

        cypher[i] = code;

        R2 = (R2 + 1) % enigma->size_rotor;
        if (R2 == get_alpha_code(key[2])) {
            R1 = (R1 + 1) % enigma->size_rotor;
        }
        if (R1 == get_alpha_code(key[1])) {
            R0 = (R0 + 1) % enigma->size_rotor;
        }

    }
    cypher[length] = '\0';
    return cypher;
}

extern void destructor(enigma_t *enigma) {
    for (int i = 0; i < enigma->num_rotors; ++i) {
        free(enigma->rotors[i]) ;
    }
    free(enigma->rotors);
    free(enigma->reflector);
    free(enigma);
}

int main() {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char reflector[] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
    char rotors[3][27] = {
        "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
        "AJDKSIRUXBLHWTMCQGZNPYFVOE",
        "BDFHJLCPRTXVZNYEIWGAKMUSQO"};
    char key[] = "ABC";

    enigma_t *enigma = constructor(26, 3);
    set_reflector(enigma, reflector);
    for (int i = 0; i < 3; ++i) {
        set_rotor(enigma, i, rotors[i]);
    }

    char msg[] = "HOMEHOMEHOME";
    char *code = crypt(enigma, msg, key, alphabet);
    char *phrase = crypt(enigma, code, key, alphabet);
    printf("%s\n", code);
    printf("%s\n", phrase);

    cracker(code);

    destructor(enigma);
    
    return 0;
}
