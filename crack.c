#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    char *hashText = md5(plaintext, strlen(plaintext));

    // Open the hash file
    FILE *hfn = fopen(hashFilename, "r");
    if (!hfn)
    {
        printf("Hash file not found\n");
        exit(1);
    }

    // Loop through the hash file, one line at a time.
    // Attempt to match the hash from the file to the
    // hash of the plaintext.
    while (!feof(hfn))
    {
        char line[1000];
        fgets(line, 1000, hfn);

        //trims newline char
        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        /*
        i utilized AI as a learning tool in creating this next block of code.
        my initial if block consisted of:

        char *tempHash;
        strncpy(tempHash, line, HASH_LEN);
        if (strcmp(line, hashText))
        {
            free(hashText);
            fclose(hfn);
            return tempHash;
        }

        In another iteration of my code which is lost now I used strcpy on temphash without malloc'ing it which also resulted in a warning.
        I would be more than happy to go over this short section to prove my understanding if necessary. 
        */

        //checks if the hashfile line matches the hashed plaintext
        if (strcmp(line, hashText) == 0)
        {
            //allocates a 33 character space of the char * type (which isn't necessary but makes the code more readable in my opinion)
            //in memory for the matched line with null terminator. This is necessary 
            //because the pointer would otherwise be de-allocated after the function completes. 
            char *tempHash = (char *)malloc(HASH_LEN);

            //another piece of code that isn't strictly necessary since malloc errors are especially rare in modern systems
            //but is good practice in the off chance there is an error
            if (tempHash == NULL)
            {
                printf("Memory allocation error\n");
                exit(1);
            }

            //copy the line into the allocated memory, which can be safely returned now that it has been malloc'd
            strcpy(tempHash, line);

            free(hashText);
            fclose(hfn);
            return tempHash;
        }

    }

    // If not, return NULL.
    free(hashText);
    fclose(hfn);
    return NULL;
}

int main(int argc, char *argv[])
{
    int matchCount = 0;

    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Open the dictionary file "argv[2]" for reading.
    FILE *dict = fopen(argv[2], "r");
    if (!dict)
    {
        printf("Dictionary file could not be found\n");
        exit(1);
    }

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hashfile.
    while (!feof(dict))
    {
        //allocates and gets the first line of the dictionary file
        char dictLine[1000];
        fgets(dictLine, 1000, dict);

        //trims line
        char *nl = strchr(dictLine, '\n');
        if (nl) *nl = '\0';

        char *found = tryWord(dictLine, argv[1]);

        // If we got a match, display the hash and the word.
        //runs tryword using the word in the dictionary file, comparing against the argument 1 file.
        if (tryWord(dictLine, argv[1]))
        {
            matchCount++;
            printf("%s %s\n", found, dictLine);
        }

    }
    
    // Close the dictionary file.
    fclose(dict);

    // Display the number of hashes that were cracked.
    printf("%d hashes cracked\n", matchCount);
    
    //printf("%s\n", tryWord("hllo", "hashes00.txt"));
}

