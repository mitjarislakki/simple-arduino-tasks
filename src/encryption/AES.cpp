#include <Arduino.h>
#include <Crypto.h>
#include <AES.h>

AES128 aes; // Create an AES object

const byte key[] = "NetworkingCourse";

// 16-byte (128-bit) block to encrypt/decrypt
byte encryptBlock[16];
// Bools to check run behavior in loop
bool encrypting = true;
bool waitingForData = false;

// Function declarations
void readAndDecrypt(void);
void readAndEncrypt(void);
void printEncryptBlock(void);
void printDecryptBlock(void);


void setup() {
    aes.setKey(key, sizeof(key));
    Serial.begin(9600); // Initialize serial communication
    while(!Serial); // Wait for Serial connection
}
void loop() {
    // Print instruction
    if(!waitingForData){
        encrypting ? Serial.println("Input a message to encrypt:") :
        Serial.println("Input a message to decrypt:");
        waitingForData = true;
        }
    // Handle input
    if (Serial.available()) { // If there's data available to read from serial
        if(encrypting){
        readAndEncrypt();
        encrypting = false; // Decrypting next input
        }
        else{
            readAndDecrypt();
            encrypting = true; // Encrypting next input
        }
        waitingForData = false; // Flag for printing next instruction
    }
}

void printEncryptBlock(){
    for(auto b : encryptBlock){
        // Prepend leading zero if necessary
        if(b < 0x10) Serial.print('0');
        Serial.print(b, HEX);
    }
}

void readAndEncrypt(){
    String inputString = Serial.readStringUntil('\n'); // Read the serial input until newline character
    Serial.print("Encrypted: ");
    unsigned int i = 0;
    for(;i<inputString.length();i++){
        int blockPointer = i % 16;
        // Encrypt and print block if full
        if(blockPointer == 0 && i > 0){
            aes.encryptBlock(encryptBlock, encryptBlock);
            printEncryptBlock();
        }
        encryptBlock[blockPointer] = inputString[i];
    }
    // Pad, encrypt and print last block
    i = i % 16;
    if(i != 0){
        while(i < 16){
            encryptBlock[i++] = '\0';
        }
    }
    aes.encryptBlock(encryptBlock, encryptBlock);
    printEncryptBlock();
    Serial.print('\n');
    }

void readAndDecrypt(){
    String inputString = Serial.readStringUntil('\n'); // Read the serial input until newline character
    Serial.print("Decrypted: ");
    unsigned int i = 0;
    // Convert Hex string to byte array
    for(;i<inputString.length(); i+=2){
        // Fill 16-byte array, decrypt and print contents when full
        int blockPointer = (i/2) % 16;
        if(blockPointer == 0 && i > 0){
        aes.decryptBlock(encryptBlock, encryptBlock);
        printDecryptBlock();
        }
        // Convert 2-char substring into byte
        char byteString[3];
        byteString[0] = inputString[i];
        byteString[1] = inputString[i+1];
        byteString[2] = '\0';
        encryptBlock[blockPointer] = (byte)strtol(byteString, NULL, 16);
    }
    // Pad last block
    i = (i/2) % 16;
    if(i != 0){
        while(i < 16){
            encryptBlock[i++] = '\0';
    }}
    // Decrypt in-place and print the last block
    aes.decryptBlock(encryptBlock, encryptBlock);
    printDecryptBlock();
    Serial.print('\n');
}

void printDecryptBlock(){
    for(auto b : encryptBlock){
        char c = (char)b;
        if(c == '\0') break; // End of string reached
        Serial.print(c);
    }
}