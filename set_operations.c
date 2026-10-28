#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LENOFDIGITS 16
#define MAX_SET_SIZE 17
#define UNIVERSUM "0123456789ABCDEF"
#define TEST_ITERATIONS 100000

// ==================== —“–” “”–€ ƒ¿ÕÕ€’ ====================
typedef struct ST {
    char digit;
    struct ST* next;
} ST;

// ==================== ¬—œŒÃŒ√¿“≈À‹Õ€≈ ‘”Õ ÷»» ====================
int charToIndex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'A' + 10;
}

char indexToChar(int index) {
    if (index < 10) return '0' + index;
    return 'A' + index - 10;
}

int isValidHexDigit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

void removeDuplicates(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] == str[j]) {
                for (int k = j; k < len; k++) {
                    str[k] = str[k + 1];
                }
                len--;
                j--;
            }
        }
    }
}

// ==================== Ã¿——»¬€ ====================
void arrayUnion(const char* A, const char* B, char* result) {
    strcpy(result, A);
    for (int i = 0; B[i]; i++) {
        if (!strchr(result, B[i])) {
            char temp[2] = {B[i], '\0'};
            strcat(result, temp);
        }
    }
}

void arrayDifference(const char* A, const char* B, char* result) {
    int index = 0;
    for (int i = 0; A[i]; i++) {
        if (!strchr(B, A[i])) {
            result[index++] = A[i];
        }
    }
    result[index] = '\0';
}

void arrayOperation(const char* A, const char* B, const char* C, const char* D, char* E) {
    char CD_union[MAX_SET_SIZE] = {0};
    char AB_union[MAX_SET_SIZE] = {0};

    arrayUnion(C, D, CD_union);
    arrayUnion(A, B, AB_union);
    arrayDifference(AB_union, CD_union, E);
}

// ==================== —œ»— » ====================
ST* createNode(char digit) {
    ST* node = (ST*)malloc(sizeof(ST));
    node->digit = digit;
    node->next = NULL;
    return node;
}

void freeList(ST* list) {
    while (list) {
        ST* temp = list;
        list = list->next;
        free(temp);
    }
}

ST* arrayToList(const char* array) {
    if (!array || !*array) return NULL;

    ST* head = createNode(array[0]);
    ST* current = head;

    for (int i = 1; array[i]; i++) {
        current->next = createNode(array[i]);
        current = current->next;
    }

    return head;
}

int isInList(ST* list, char digit) {
    ST* current = list;
    while (current) {
        if (current->digit == digit) return 1;
        current = current->next;
    }
    return 0;
}

ST* listUnion(ST* A, ST* B) {
    ST* result = NULL;
    ST* tail = NULL;

    // ƒÓ·‡‚ÎˇÂÏ ˝ÎÂÏÂÌÚ˚ ËÁ A
    ST* current = A;
    while (current) {
        if (!isInList(result, current->digit)) {
            ST* newNode = createNode(current->digit);
            if (!result) {
                result = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        current = current->next;
    }

    // ƒÓ·‡‚ÎˇÂÏ ˝ÎÂÏÂÌÚ˚ ËÁ B
    current = B;
    while (current) {
        if (!isInList(result, current->digit)) {
            ST* newNode = createNode(current->digit);
            tail->next = newNode;
            tail = newNode;
        }
        current = current->next;
    }

    return result;
}

ST* listDifference(ST* A, ST* B) {
    ST* result = NULL;
    ST* tail = NULL;

    ST* current = A;
    while (current) {
        if (!isInList(B, current->digit)) {
            ST* newNode = createNode(current->digit);
            if (!result) {
                result = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        current = current->next;
    }

    return result;
}

ST* listOperation(ST* A, ST* B, ST* C, ST* D) {
    ST* CD_union = listUnion(C, D);
    ST* AB_union = listUnion(A, B);
    ST* result = listDifference(AB_union, CD_union);

    freeList(CD_union);
    freeList(AB_union);

    return result;
}

void listToArray(ST* list, char* array) {
    int index = 0;
    ST* current = list;
    while (current && index < MAX_SET_SIZE - 1) {
        array[index++] = current->digit;
        current = current->next;
    }
    array[index] = '\0';
}

// ==================== ¡»“Œ¬€≈ Ã¿——»¬€ ====================
void arrayToBitArray(const char* array, int* bitArray) {
    for (int i = 0; i < LENOFDIGITS; i++) bitArray[i] = 0;
    for (int i = 0; array[i]; i++) {
        int index = charToIndex(array[i]);
        if (index >= 0 && index < LENOFDIGITS) {
            bitArray[index] = 1;
        }
    }
}

void bitArrayToArray(const int* bitArray, char* array) {
    int index = 0;
    for (int i = 0; i < LENOFDIGITS; i++) {
        if (bitArray[i]) {
            array[index++] = indexToChar(i);
        }
    }
    array[index] = '\0';
}

void bitArrayUnion(const int* A, const int* B, int* result) {
    for (int i = 0; i < LENOFDIGITS; i++) {
        result[i] = A[i] || B[i];
    }
}

void bitArrayDifference(const int* A, const int* B, int* result) {
    for (int i = 0; i < LENOFDIGITS; i++) {
        result[i] = A[i] && !B[i];
    }
}

void bitArrayOperation(const int* A, const int* B, const int* C, const int* D, int* E) {
    int CD_union[LENOFDIGITS] = {0};
    int AB_union[LENOFDIGITS] = {0};

    bitArrayUnion(C, D, CD_union);
    bitArrayUnion(A, B, AB_union);
    bitArrayDifference(AB_union, CD_union, E);
}

// ==================== Ã¿ÿ»ÕÕ€≈ —ÀŒ¬¿ ====================
unsigned short arrayToMachineWord(const char* array) {
    unsigned short word = 0;
    for (int i = 0; array[i]; i++) {
        int index = charToIndex(array[i]);
        if (index >= 0 && index < LENOFDIGITS) {
            word |= (1 << index);
        }
    }
    return word;
}

void machineWordToArray(unsigned short word, char* array) {
    int index = 0;
    for (int i = 0; i < LENOFDIGITS; i++) {
        if (word & (1 << i)) {
            array[index++] = indexToChar(i);
        }
    }
    array[index] = '\0';
}

unsigned short machineWordOperation(unsigned short A, unsigned short B,
                                  unsigned short C, unsigned short D) {
    return (A | B) & ~(C | D);
}

// ==================== √≈Õ≈–¿÷»ﬂ “≈—“Œ¬ ====================
void generateRandomSet(int size, char* set) {
    char universe[] = UNIVERSUM;
    int used[LENOFDIGITS] = {0};
    int count = 0;

    while (count < size && count < LENOFDIGITS) {
        int index = rand() % LENOFDIGITS;
        if (!used[index]) {
            set[count++] = universe[index];
            used[index] = 1;
        }
    }
    set[count] = '\0';
}

// ==================== »«Ã≈–≈Õ»≈ ¬ “¿ “¿’ ====================
clock_t measureArrayTimeTicks(const char* A, const char* B, const char* C, const char* D, char* E) {
    clock_t start = clock();

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        arrayOperation(A, B, C, D, E);
    }

    clock_t end = clock();
    return end - start;
}

clock_t measureListTimeTicks(ST* A, ST* B, ST* C, ST* D, char* E) {
    clock_t start = clock();

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        ST* result = listOperation(A, B, C, D);
        listToArray(result, E);
        freeList(result);
    }

    clock_t end = clock();
    return end - start;
}

clock_t measureBitArrayTimeTicks(const int* A, const int* B, const int* C, const int* D, int* E) {
    clock_t start = clock();

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        bitArrayOperation(A, B, C, D, E);
    }

    clock_t end = clock();
    return end - start;
}

clock_t measureMachineWordTimeTicks(unsigned short A, unsigned short B,
                                  unsigned short C, unsigned short D, unsigned short* E) {
    clock_t start = clock();

    for (int i = 0; i < TEST_ITERATIONS; i++) {
        *E = machineWordOperation(A, B, C, D);
    }

    clock_t end = clock();
    return end - start;
}

// ==================== ¬€¬Œƒ –≈«”À‹“¿“Œ¬ ====================
void printResults(const char* method, const char* E, clock_t ticks) {
    printf("%s: E = {", method);
    for (int i = 0; E[i]; i++) {
        printf("%c", E[i]);
        if (E[i + 1]) printf(", ");
    }
    printf("} | Ticks: %ld for %d iterations | Per operation: %.2f ticks\n",
           ticks, TEST_ITERATIONS, (double)ticks / TEST_ITERATIONS);
}

// ==================== Œ—ÕŒ¬Õ¿ﬂ œ–Œ√–¿ÃÃ¿ ====================
int main() {
    srand(time(NULL));

    printf("=== SET OPERATIONS ON HEXADECIMAL DIGITS ===\n");
    printf("Test iterations: %d\n\n", TEST_ITERATIONS);

    // ==================== ¬¬Œƒ ƒ¿ÕÕ€’ Œ“ œŒÀ‹«Œ¬¿“≈Àﬂ ====================
    printf("=== MANUAL INPUT ===\n");
    char A_manual[MAX_SET_SIZE], B_manual[MAX_SET_SIZE];
    char C_manual[MAX_SET_SIZE], D_manual[MAX_SET_SIZE];

    printf("Enter set A (hex digits 0-9, A-F): ");
    scanf("%s", A_manual);
    printf("Enter set B (hex digits 0-9, A-F): ");
    scanf("%s", B_manual);
    printf("Enter set C (hex digits 0-9, A-F): ");
    scanf("%s", C_manual);
    printf("Enter set D (hex digits 0-9, A-F): ");
    scanf("%s", D_manual);

    // ”‰‡ÎˇÂÏ ‰Û·ÎËÍ‡Ú˚ Ë ÔÓ‚ÂˇÂÏ ÍÓÂÍÚÌÓÒÚ¸
    removeDuplicates(A_manual);
    removeDuplicates(B_manual);
    removeDuplicates(C_manual);
    removeDuplicates(D_manual);

    printf("\nA = {%s}, B = {%s}, C = {%s}, D = {%s}\n\n",
           A_manual, B_manual, C_manual, D_manual);

    // “ÂÒÚËÓ‚‡ÌËÂ Ì‡ ‚‚Â‰ÂÌÌ˚ı ‰‡ÌÌ˚ı
    char E_manual_array[MAX_SET_SIZE] = {0};
    clock_t ticks_array_manual = measureArrayTimeTicks(A_manual, B_manual, C_manual, D_manual, E_manual_array);
    printResults("Arrays", E_manual_array, ticks_array_manual);

    ST* A_manual_list = arrayToList(A_manual);
    ST* B_manual_list = arrayToList(B_manual);
    ST* C_manual_list = arrayToList(C_manual);
    ST* D_manual_list = arrayToList(D_manual);
    char E_manual_list[MAX_SET_SIZE] = {0};
    clock_t ticks_list_manual = measureListTimeTicks(A_manual_list, B_manual_list, C_manual_list, D_manual_list, E_manual_list);
    printResults("Lists  ", E_manual_list, ticks_list_manual);

    int A_manual_bit[LENOFDIGITS], B_manual_bit[LENOFDIGITS];
    int C_manual_bit[LENOFDIGITS], D_manual_bit[LENOFDIGITS];
    int E_manual_bit[LENOFDIGITS] = {0};
    arrayToBitArray(A_manual, A_manual_bit);
    arrayToBitArray(B_manual, B_manual_bit);
    arrayToBitArray(C_manual, C_manual_bit);
    arrayToBitArray(D_manual, D_manual_bit);
    clock_t ticks_bit_manual = measureBitArrayTimeTicks(A_manual_bit, B_manual_bit, C_manual_bit, D_manual_bit, E_manual_bit);
    char E_manual_bit_str[MAX_SET_SIZE] = {0};
    bitArrayToArray(E_manual_bit, E_manual_bit_str);
    printResults("BitArr ", E_manual_bit_str, ticks_bit_manual);

    unsigned short A_manual_word = arrayToMachineWord(A_manual);
    unsigned short B_manual_word = arrayToMachineWord(B_manual);
    unsigned short C_manual_word = arrayToMachineWord(C_manual);
    unsigned short D_manual_word = arrayToMachineWord(D_manual);
    unsigned short E_manual_word;
    clock_t ticks_word_manual = measureMachineWordTimeTicks(A_manual_word, B_manual_word, C_manual_word, D_manual_word, &E_manual_word);
    char E_manual_word_str[MAX_SET_SIZE] = {0};
    machineWordToArray(E_manual_word, E_manual_word_str);
    printResults("Machine", E_manual_word_str, ticks_word_manual);

    // Œ˜ËÒÚÍ‡ Ô‡ÏˇÚË
    freeList(A_manual_list);
    freeList(B_manual_list);
    freeList(C_manual_list);
    freeList(D_manual_list);

    printf("\n");

    // ==================== ‘» —»–Œ¬¿ÕÕ€≈ “≈—“Œ¬€≈ ƒ¿ÕÕ€≈ ====================
    char A_fixed_test[] = "123";
    char B_fixed_test[] = "456";
    char C_fixed_test[] = "234";
    char D_fixed_test[] = "789";

    printf("=== FIXED TEST DATA ===\n");
    printf("A = {%s}, B = {%s}, C = {%s}, D = {%s}\n\n",
           A_fixed_test, B_fixed_test, C_fixed_test, D_fixed_test);

    // Ã‡ÒÒË‚˚
    char E_array_test[MAX_SET_SIZE] = {0};
    clock_t ticks_array_test = measureArrayTimeTicks(A_fixed_test, B_fixed_test, C_fixed_test, D_fixed_test, E_array_test);
    printResults("Arrays", E_array_test, ticks_array_test);

    // —ÔËÒÍË
    ST* A_list_test = arrayToList(A_fixed_test);
    ST* B_list_test = arrayToList(B_fixed_test);
    ST* C_list_test = arrayToList(C_fixed_test);
    ST* D_list_test = arrayToList(D_fixed_test);
    char E_list_test[MAX_SET_SIZE] = {0};
    clock_t ticks_list_test = measureListTimeTicks(A_list_test, B_list_test, C_list_test, D_list_test, E_list_test);
    printResults("Lists  ", E_list_test, ticks_list_test);

    // ¡ËÚÓ‚˚Â Ï‡ÒÒË‚˚
    int A_bit_test[LENOFDIGITS], B_bit_test[LENOFDIGITS];
    int C_bit_test[LENOFDIGITS], D_bit_test[LENOFDIGITS];
    int E_bit_test[LENOFDIGITS] = {0};
    arrayToBitArray(A_fixed_test, A_bit_test);
    arrayToBitArray(B_fixed_test, B_bit_test);
    arrayToBitArray(C_fixed_test, C_bit_test);
    arrayToBitArray(D_fixed_test, D_bit_test);
    clock_t ticks_bit_test = measureBitArrayTimeTicks(A_bit_test, B_bit_test, C_bit_test, D_bit_test, E_bit_test);
    char E_bit_str_test[MAX_SET_SIZE] = {0};
    bitArrayToArray(E_bit_test, E_bit_str_test);
    printResults("BitArr ", E_bit_str_test, ticks_bit_test);

    // Ã‡¯ËÌÌ˚Â ÒÎÓ‚‡
    unsigned short A_word_test = arrayToMachineWord(A_fixed_test);
    unsigned short B_word_test = arrayToMachineWord(B_fixed_test);
    unsigned short C_word_test = arrayToMachineWord(C_fixed_test);
    unsigned short D_word_test = arrayToMachineWord(D_fixed_test);
    unsigned short E_word_test;
    clock_t ticks_word_test = measureMachineWordTimeTicks(A_word_test, B_word_test, C_word_test, D_word_test, &E_word_test);
    char E_word_str_test[MAX_SET_SIZE] = {0};
    machineWordToArray(E_word_test, E_word_str_test);
    printResults("Machine", E_word_str_test, ticks_word_test);

    // Œ˜ËÒÚÍ‡ Ô‡ÏˇÚË
    freeList(A_list_test);
    freeList(B_list_test);
    freeList(C_list_test);
    freeList(D_list_test);

    printf("\n");

    // ==================== —À”◊¿…Õ€≈ “≈—“Œ¬€≈ ƒ¿ÕÕ€≈ ====================
    printf("=== RANDOM TEST DATA ===\n");

    // √ÂÌÂ‡ˆËˇ ÒÎÛ˜‡ÈÌ˚ı ‰‡ÌÌ˚ı
    char A_rand[MAX_SET_SIZE], B_rand[MAX_SET_SIZE];
    char C_rand[MAX_SET_SIZE], D_rand[MAX_SET_SIZE];

    generateRandomSet(8, A_rand);
    generateRandomSet(8, B_rand);
    generateRandomSet(8, C_rand);
    generateRandomSet(8, D_rand);

    printf("A = {%s}, B = {%s}, C = {%s}, D = {%s}\n\n",
           A_rand, B_rand, C_rand, D_rand);

    // “ÂÒÚËÓ‚‡ÌËÂ Ì‡ ÒÎÛ˜‡ÈÌ˚ı ‰‡ÌÌ˚ı
    char E_rand_array[MAX_SET_SIZE] = {0};
    clock_t ticks_array_rand = measureArrayTimeTicks(A_rand, B_rand, C_rand, D_rand, E_rand_array);
    printResults("Arrays", E_rand_array, ticks_array_rand);

    ST* A_rand_list = arrayToList(A_rand);
    ST* B_rand_list = arrayToList(B_rand);
    ST* C_rand_list = arrayToList(C_rand);
    ST* D_rand_list = arrayToList(D_rand);
    char E_rand_list[MAX_SET_SIZE] = {0};
    clock_t ticks_list_rand = measureListTimeTicks(A_rand_list, B_rand_list, C_rand_list, D_rand_list, E_rand_list);
    printResults("Lists  ", E_rand_list, ticks_list_rand);

    int A_rand_bit[LENOFDIGITS], B_rand_bit[LENOFDIGITS];
    int C_rand_bit[LENOFDIGITS], D_rand_bit[LENOFDIGITS];
    int E_rand_bit[LENOFDIGITS] = {0};
    arrayToBitArray(A_rand, A_rand_bit);
    arrayToBitArray(B_rand, B_rand_bit);
    arrayToBitArray(C_rand, C_rand_bit);
    arrayToBitArray(D_rand, D_rand_bit);
    clock_t ticks_bit_rand = measureBitArrayTimeTicks(A_rand_bit, B_rand_bit, C_rand_bit, D_rand_bit, E_rand_bit);
    char E_rand_bit_str[MAX_SET_SIZE] = {0};
    bitArrayToArray(E_rand_bit, E_rand_bit_str);
    printResults("BitArr ", E_rand_bit_str, ticks_bit_rand);

    unsigned short A_rand_word = arrayToMachineWord(A_rand);
    unsigned short B_rand_word = arrayToMachineWord(B_rand);
    unsigned short C_rand_word = arrayToMachineWord(C_rand);
    unsigned short D_rand_word = arrayToMachineWord(D_rand);
    unsigned short E_rand_word;
    clock_t ticks_word_rand = measureMachineWordTimeTicks(A_rand_word, B_rand_word, C_rand_word, D_rand_word, &E_rand_word);
    char E_rand_word_str[MAX_SET_SIZE] = {0};
    machineWordToArray(E_rand_word, E_rand_word_str);
    printResults("Machine", E_rand_word_str, ticks_word_rand);

    // Œ˜ËÒÚÍ‡ Ô‡ÏˇÚË
    freeList(A_rand_list);
    freeList(B_rand_list);
    freeList(C_rand_list);
    freeList(D_rand_list);

    return 0;
}
