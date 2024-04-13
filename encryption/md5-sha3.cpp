#include <MD5.h> // MD5 implementation
#include <Crypto.h> // Crypto library
#include <SHA3.h> // SHA3 implementation, part of Crypto
#include <stdlib.h> // memory allocation

// Flag for input prompt
bool waitingForData;

// SHA3-256 specific
#define HASH_SIZE 32
unsigned char hash[HASH_SIZE];
SHA3_256 sha3_256;

// Function declarations
void hashMD5(char*);
void hashSHA3(char*, int);


void setup()
{
  waitingForData = false;
  // Initialize serial
  Serial.begin(9600);
  while(!Serial); // Wait for serial connection
}

void loop()
{
  // Print instruction if not waiting for input
  if(!waitingForData){
    Serial.print("Input a message to hash: ");
    waitingForData = true;
  }
  // Handle serial monitor input
  while(Serial.available()){
    String input = Serial.readStringUntil('\n'); // Read input as Arduino string
    Serial.println(input);
    int strSize = input.length();
    char* str = (char*) malloc( (strSize+1) * sizeof(char) ); // Allocate memory for the C-string
    input.toCharArray(str, strSize+1); // Copy input String to C character-array
    str[strSize] = '\0'; // Null terminator

    // Print the MD5-digest
    hashMD5(str);
    // Print the SHA3-256 digest
    hashSHA3(str, strSize);

    // Free the memory allocated to the input C-string
    free(str);
    Serial.println("----------------------------------------"); // Separation in loop
    // Set flag to print next instruction
    waitingForData = false;
  }
}

void hashMD5(char* str){
  // Create the hash from the input C-string
  unsigned char* hash = MD5::make_hash(str); 
  // Generate the hex-encoded (base-16) digest for our hash
  char *md5str = MD5::make_digest(hash, 16);
  // Free the hash string
  free(hash);
  // Print the digest
  Serial.print("MD5 digest: ");
  Serial.println(md5str);
  // Free the md5 string memory
  free(md5str);
}

void hashSHA3(char* str, int strSize){
  sha3_256.reset();
  sha3_256.update(str, strSize);
  sha3_256.finalize(hash, HASH_SIZE);
  Serial.print("SHA3 digest: ");
  for (int i = 0; i < HASH_SIZE; i++)
    Serial.print(hash[i], HEX);
  Serial.print('\n');
}