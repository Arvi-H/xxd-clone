#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
    if (argc > 2) {
        printf("Usage: %s [-b|-bits]\n", argv[0]);
        exit(BAD_NUMBER_ARGS);
    }

    if (argc == 2 &&
        (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
        *bits = TRUE;
    } else {
        *bits = FALSE;
    }

    return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
    for (int i = 0; i < 16; i++) {
        // Nicecly format hex output (every byte - 2 hex characters)
        if ((i % 2) == 0) printf(" ");

        // Print 1 byte - fill with 0s if <
        if (i < size) {
            printf("%02x", data[i]);
        } else {
        // Formatting
            printf("  ");
        }
    }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // Check to see if it's a printable character
        if (data[i] >= 32 && data[i] <= 126) {
            printf("%c", data[i]);
        } else {
            printf(".");
        }
    }
}

void setBitString(char *bits, unsigned char x) {
    for (int i = 7; i >= 0; i--) {
        if ((x % 2) == 1) {
            bits[i] = '1';
        } else {
            bits[i] = '0';
        }
        x /= 2;    
    }
}

void printDataAsBits (unsigned char *data, size_t size) {   
    // Store binary result in an array of size 8
    char bits[9];
    bits[8] = 0;

    for (size_t i = 0; i < 6; i++) { 
        // Nicecly format binary output 
        printf(" ");

        if (i < size) {
            setBitString(bits, data[i]);
            printf("%s", bits);
        } else {
            printf("        ");
        }
    }
}

void readAndPrintInputAsHex(FILE *input) {
    // Store input data 16 bytes at a time
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);

    // Initial line number
    unsigned int offset = 0;

    while (numBytesRead != 0) { 
        // Print line numbers on the left
        printf("%08x:", offset);
        offset += numBytesRead;
        
        // Print the bytes in hex
        printDataAsHex(data, numBytesRead);
        printf("  ");
        
        // Print the data regularly
        printDataAsChars(data, numBytesRead);
        printf("\n");

        // Continue until there is nothing to read
        numBytesRead = fread(data, 1, 16, input);
    }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {

    // Store input data 6 bytes at a time
    unsigned char data[6];
    int numBytesRead = fread(data, 1, 6, input);
    unsigned int offset = 0;

    while (numBytesRead != 0) { 
        printf("%08x:", offset);
        offset += numBytesRead;

        printDataAsBits(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        
        printf("\n");
        numBytesRead = fread(data, 1, 6, input);
    }
}

int main(int argc, char **argv) {
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);

    if (bits == FALSE) {
        readAndPrintInputAsHex(input);
    } else {
        readAndPrintInputAsBits(input);
    }
    return 0;
}
