#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings



int file_length(char *filename)
{
    struct stat info;
    if (stat(filename, &info) < 0 ) 
        return -1;
    else
        return info.st_size;
}


// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.
int tryguess(char *hash, char *guess)
{
    // Hash the guess using MD5
    int guessLen = strlen(guess);
    char *guessedHash = md5(guess,guessLen);
    // Compare the two hashes
    
    int result = 0;
    
    
    
    if(strcmp(hash, guessedHash) == 0)
        result = 1;
        
    free(guessedHash);
    
    return result;
}

// Read in the dictionary file and return the array of strings
// and store the length of the array in size.
// This function is responsible for opening the dictionary file,
// reading from it, building the data structure, and closing the
// file.
char **read_dictionary(char *filename, int *size)
{
    int fileLength = file_length(filename);
    char *contents = malloc(fileLength);
    
    FILE *f = fopen(filename,"r");
    if(!f)
    {
        printf("Can't open %s for reading\n", filename);
        exit(1);
    }
    
    fread(contents, 1, fileLength, f);
    fclose(f);
    

    //Remove whitespaces
    int numPasswords = 0;
    for(int i=0; i < fileLength; i++)
    {
        if(contents[i] == '\n')
        {
            contents[i] = '\0';
            numPasswords++;
        }
    }
    
    
    char **stringArray = malloc(numPasswords * sizeof(char *));
    
    //Input strings into the stringArray.
    stringArray[0] = contents; //???
    
    

    int j = 1; //other counter for indexing stringArray
   
    for(int i=0; i < fileLength-1; i++)
    {
        if(contents[i] == '\0')
        {
            stringArray[j] = &contents[i+1];
            j++;
        }
    }
    
    //free(contents); DO NOT FREE CONTENTS. LOL
    *size = numPasswords;
    return stringArray;
}





int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }
    
    char *hashFile = argv[1];
    char *dictFile = argv[2];

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(dictFile, &dlen);

    // Open the hash file for reading.
    FILE *f = fopen(hashFile, "r");
    if(!f)
    {
        printf("Cannot open %s for reading\n", hashFile);
        return 1;
    }
    
    
    //Read hashes into an array (Using same read_dictionary function just because it works)
    int hLen;
    char **hashes = read_dictionary(hashFile, &hLen );
    
    // For each hash, try every entry in the dictionary.
    // Print the matching dictionary entry.
    // Need two nested loops.
    for( int i = 0; i < hLen; i++)
    {
        
        for( int j = 0; j < dlen; j++)
        {
            
            
            int result = tryguess(hashes[i], dict[j]);
            if(result == 1)
            {
                printf("%s matches with %s \n", dict[j], hashes[i]);   
                break;
                
            }
        }
    }
    
    

    
}
