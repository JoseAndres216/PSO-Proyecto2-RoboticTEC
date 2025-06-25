#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordCount
{
    char word[50];
    int count;
};

int main(int argc, char *argv[])
{

    // Variables for word counting
    FILE *completeFile;

    // Reads the decrypted file.

    // Open the file to save the decrypted file
    completeFile = fopen("ClusterFiles/DecryptedFile.txt", "rb");
    if (completeFile == NULL)
    {
        perror("Error opening the file");
    }

    // Separates the file in three leght-equal files.

    // Get file size
    fseek(completeFile, 0, SEEK_END);
    long fileSize = ftell(completeFile);
    rewind(completeFile);

    // Allocate memory for full file content
    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL)
    {
        perror("Memory allocation failed");
        fclose(completeFile);
        return 1;
    }

    fread(fileContent, 1, fileSize, completeFile);
    fileContent[fileSize] = '\0';
    fclose(completeFile);

    // Count total words
    int totalWords = 0, inWord = 0;
    for (char *ptr = fileContent; *ptr; ptr++)
    {
        if (isspace(*ptr))
        {
            inWord = 0;
        }
        else if (!inWord)
        {
            inWord = 1;
            totalWords++;
        }
    }

    int wordsPerPart = totalWords / 3;

    // Initializes countPart array to have a size of wordsPerPart
    struct WordCount *partCount1 = NULL;
    struct WordCount *partCount2 = NULL;
    struct WordCount *partCount3 = NULL;
    struct WordCount *totalCount = NULL;

    // Allocate memory for partCount and totalCount
    partCount1 = (struct WordCount *)malloc(wordsPerPart * sizeof(struct WordCount));
    partCount2 = (struct WordCount *)malloc(wordsPerPart * sizeof(struct WordCount));
    partCount3 = (struct WordCount *)malloc(wordsPerPart * sizeof(struct WordCount));
    totalCount = (struct WordCount *)malloc(totalWords * sizeof(struct WordCount));

    // Check if memory allocation was successful
    if (partCount1 == NULL || partCount2 == NULL || partCount3 == NULL || totalCount == NULL)
    {
        perror("Error allocating memory for WordCount arrays");
        free(fileContent);
        if (partCount1)
            free(partCount1);
        if (partCount2)
            free(partCount2);
        if (partCount3)
            free(partCount3);
        if (totalCount)
            free(totalCount);
        return 1;
    }

    // Initialize the count arrays
    memset(partCount1, 0, wordsPerPart * sizeof(struct WordCount));
    memset(partCount2, 0, wordsPerPart * sizeof(struct WordCount));
    memset(partCount3, 0, wordsPerPart * sizeof(struct WordCount));
    memset(totalCount, 0, totalWords * sizeof(struct WordCount));

    // Allocate output buffers
    char *part1 = (char *)malloc(fileSize + 1);
    char *part2 = (char *)malloc(fileSize + 1);
    char *part3 = (char *)malloc(fileSize + 1);
    if (!part1 || !part2 || !part3)
    {
        perror("Memory allocation for parts failed");
        free(fileContent);
        return 1;
    }

    // Split content by words
    int wordCount = 0;
    char *src = fileContent;
    char *dst1 = part1, *dst2 = part2, *dst3 = part3;
    int currentPart = 1;
    inWord = 0;

    while (*src)
    {
        if (isspace(*src))
        {
            if (inWord)
            {
                wordCount++;
                inWord = 0;
                if (wordCount == wordsPerPart)
                    currentPart++;
                if (wordCount == 2 * wordsPerPart)
                    currentPart++;
            }
        }
        else
        {
            if (!inWord)
                inWord = 1;
        }

        if (currentPart == 1)
            *dst1++ = *src;
        else if (currentPart == 2)
            *dst2++ = *src;
        else
            *dst3++ = *src;

        src++;
    }

    *dst1 = '\0';
    *dst2 = '\0';
    *dst3 = '\0';

    // Write to output files
    FILE *part1File = fopen("ClusterFiles/DecryptedFilePart1.txt", "wb");
    FILE *part2File = fopen("ClusterFiles/DecryptedFilePart2.txt", "wb");
    FILE *part3File = fopen("ClusterFiles/DecryptedFilePart3.txt", "wb");

    if (part1File)
        fwrite(part1, 1, strlen(part1), part1File);
    if (part2File)
        fwrite(part2, 1, strlen(part2), part2File);
    if (part3File)
        fwrite(part3, 1, strlen(part3), part3File);

    // Clean up
    if (part1File)
        fclose(part1File);
    if (part2File)
        fclose(part2File);
    if (part3File)
        fclose(part3File);

    free(fileContent);

    // Counts the times a word appear on the file according to which client is working (id)

    char wordBuffer[50] = {0};
    int charIndex = 0;

    inWord = 0;

    // For part 1

    int wordsRegisteredInArray1 = 0;

    FILE *part1DecryptedFile = fopen("ClusterFiles/DecryptedFilePart1.txt", "rb");
    if (part1File == NULL)
    {
        perror("Error opening part 1 file");
        free(part1);
        free(part2);
        free(part3);
        free(partCount1);
        free(partCount2);
        free(partCount3);
        free(totalCount);
        return 1;
    }

    while (fscanf(part1DecryptedFile, "%49s", wordBuffer) == 1)
    {
        // Deletes special characters from the word
        for (int i = 0; wordBuffer[i]; i++)
        {
            if (!isalpha((unsigned char)wordBuffer[i]) && wordBuffer[i] != '\'')
            {
                for (int j = i; wordBuffer[j]; j++)
                {
                    wordBuffer[j] = wordBuffer[j + 1];
                }
                i--;
            }
        }

        // Convert word to lowercase
        for (int i = 0; wordBuffer[i]; i++)
        {
            wordBuffer[i] = tolower((unsigned char)wordBuffer[i]);
        }

        // printf("%s\n", wordBuffer);

        for (int i = 0; i < wordsPerPart; i++)
        {
            if (strcmp(partCount1[i].word, wordBuffer) == 0)
            {
                partCount1[i].count++;
                break;
            }
            else
            {
                if (partCount1[i].count != 0)
                    continue;
                else
                {
                    strcpy(partCount1[i].word, wordBuffer);
                    partCount1[i].count = 1;
                    wordsRegisteredInArray1++;
                    break;
                }
            }
        }
    }

    fclose(part1DecryptedFile);

    for (int i = 0; i < wordsRegisteredInArray1; i++)
    {
        // printf("%s,%d\n", partCount1[i].word, partCount1[i].count);
    }

    // For part 2

    int wordsRegisteredInArray2 = 0;

    FILE *part2DecryptedFile = fopen("ClusterFiles/DecryptedFilePart2.txt", "rb");
    if (part1File == NULL)
    {
        perror("Error opening part 2 file");
        free(part1);
        free(part2);
        free(part3);
        free(partCount1);
        free(partCount2);
        free(partCount3);
        free(totalCount);
        return 1;
    }

    while (fscanf(part2DecryptedFile, "%49s", wordBuffer) == 1)
    {
        // Deletes special characters from the word
        for (int i = 0; wordBuffer[i]; i++)
        {
            if (!isalpha((unsigned char)wordBuffer[i]) && wordBuffer[i] != '\'')
            {
                for (int j = i; wordBuffer[j]; j++)
                {
                    wordBuffer[j] = wordBuffer[j + 1];
                }
                i--;
            }
        }

        // Convert word to lowercase
        for (int i = 0; wordBuffer[i]; i++)
        {
            wordBuffer[i] = tolower((unsigned char)wordBuffer[i]);
        }

        // printf("%s\n", wordBuffer);

        for (int i = 0; i < wordsPerPart; i++)
        {
            if (strcmp(partCount2[i].word, wordBuffer) == 0)
            {
                partCount2[i].count++;
                break;
            }
            else
            {
                if (partCount2[i].count != 0)
                    continue;
                else
                {
                    strcpy(partCount2[i].word, wordBuffer);
                    partCount2[i].count = 1;
                    wordsRegisteredInArray2++;
                    break;
                }
            }
        }
    }

    fclose(part2DecryptedFile);

    for (int i = 0; i < wordsRegisteredInArray2; i++)
    {
        // printf("%s,%d\n", partCount2[i].word, partCount2[i].count);
    }

    // For part 3

    int wordsRegisteredInArray3 = 0;

    FILE *part3DecryptedFile = fopen("ClusterFiles/DecryptedFilePart3.txt", "rb");
    if (part1File == NULL)
    {
        perror("Error opening part 3 file");
        free(part1);
        free(part2);
        free(part3);
        free(partCount1);
        free(partCount2);
        free(partCount3);
        free(totalCount);
        return 1;
    }

    while (fscanf(part3DecryptedFile, "%49s", wordBuffer) == 1)
    {
        // Deletes special characters from the word
        for (int i = 0; wordBuffer[i]; i++)
        {
            if (!isalpha((unsigned char)wordBuffer[i]) && wordBuffer[i] != '\'')
            {
                for (int j = i; wordBuffer[j]; j++)
                {
                    wordBuffer[j] = wordBuffer[j + 1];
                }
                i--;
            }
        }

        // Convert word to lowercase
        for (int i = 0; wordBuffer[i]; i++)
        {
            wordBuffer[i] = tolower((unsigned char)wordBuffer[i]);
        }

        // printf("%s\n", wordBuffer);

        for (int i = 0; i < wordsPerPart; i++)
        {
            if (strcmp(partCount3[i].word, wordBuffer) == 0)
            {
                partCount3[i].count++;
                break;
            }
            else
            {
                if (partCount3[i].count != 0)
                    continue;
                else
                {
                    strcpy(partCount3[i].word, wordBuffer);
                    partCount3[i].count = 1;
                    wordsRegisteredInArray3++;
                    break;
                }
            }
        }
    }

    fclose(part3DecryptedFile);

    for (int i = 0; i < wordsRegisteredInArray3; i++)
    {
        // printf("%s,%d\n", partCount3[i].word, partCount3[i].count);
    }

    // Merges all dictionaries created

    int wordsRegisteredInTotal = 0;

    // For part 1
    for (int i = 0; i < wordsRegisteredInArray1; i++)
    {
        for (int j = 0; j < totalWords; j++)
        {
            if (strcmp(totalCount[j].word, partCount1[i].word) == 0)
            {
                totalCount[j].count = totalCount[j].count + partCount1[i].count;
                break;
            }
            else
            {
                if (totalCount[j].count != 0)
                    continue;
                else
                {

                    strcpy(totalCount[j].word, partCount1[i].word);
                    totalCount[j].count = partCount1[i].count;
                    wordsRegisteredInTotal++;
                    break;
                }
            }
        }

        // For part 2
        for (int i = 0; i < wordsRegisteredInArray2; i++)
        {
            for (int j = 0; j < totalWords; j++)
            {
                if (strcmp(totalCount[j].word, partCount2[i].word) == 0)
                {
                    totalCount[j].count = totalCount[j].count + partCount2[i].count;
                    break;
                }
                else
                {

                    if (totalCount[j].count != 0)
                        continue;
                    else
                    {

                        strcpy(totalCount[j].word, partCount2[i].word);
                        totalCount[j].count = partCount2[i].count;
                        wordsRegisteredInTotal++;
                        break;
                    }
                }
            }
        }

        // For part 3
        for (int i = 0; i < wordsRegisteredInArray3; i++)
        {
            for (int j = 0; j < totalWords; j++)
            {
                if (strcmp(totalCount[j].word, partCount3[i].word) == 0)
                {
                    totalCount[j].count = totalCount[j].count + partCount3[i].count;
                    break;
                }
                else
                {

                    if (totalCount[j].count != 0)
                        continue;
                    else
                    {

                        strcpy(totalCount[j].word, partCount3[i].word);
                        totalCount[j].count = partCount3[i].count;
                        wordsRegisteredInTotal++;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < wordsRegisteredInTotal; i++)
        {
            // printf("%s,%d\n", totalCount[i].word, totalCount[i].count);
        }

        // Saves totalCount to a file
        FILE *resultFile = fopen("ClusterFiles/ClusterResult.txt", "wb");
        if (resultFile == NULL)
        {
            perror("Error opening total count file");
            free(part1);
            free(part2);
            free(part3);
            free(partCount1);
            free(partCount2);
            free(partCount3);
            free(totalCount);
            return 1;
        }

        for (int i = 0; i < wordsRegisteredInTotal; i++)
        {
            if (totalCount[i].word[0] != '\0')
            {
                fprintf(resultFile, "%s,%d\n", totalCount[i].word, totalCount[i].count);
            }
        }

        fclose(resultFile);

        for (int i = 0; i < 3; i++)
        {
            // printf("%s,%d\n", totalCount[i].word, totalCount[i].count);
        }

        // Sorts the totalCount array by count in descending order
        for (int i = 0; i < wordsRegisteredInTotal - 1; i++)
        {
            for (int j = i + 1; j < wordsRegisteredInTotal; j++)
            {
                if (totalCount[i].count < totalCount[j].count)
                {
                    struct WordCount temp = totalCount[i];
                    totalCount[i] = totalCount[j];
                    totalCount[j] = temp;
                }
            }
        }

        // Prints the 10 most frequent words
        printf("The 10 most frequent words are:\n");
        for (int i = 0; i < 10 && i < wordsRegisteredInTotal; i++)
        {
            //printf("%s: %d\n", totalCount[i].word, totalCount[i].count);
        }

        // Writes the most frequent word to a file
        FILE *mostFrequentWordFile = fopen("ClusterFiles/MostFrequentWord.txt", "wb");
        if (mostFrequentWordFile == NULL)
        {
            perror("Error opening most frequent word file");
            free(part1);
            free(part2);
            free(part3);
            free(partCount1);
            free(partCount2);
            free(partCount3);
            free(totalCount);
            return 1;
        }

        if (wordsRegisteredInTotal > 0)
        {
            fprintf(mostFrequentWordFile, "%s,%d", totalCount[0].word, totalCount[0].count);
        }

        // Free allocated memory
        free(part1);
        free(part2);
        free(part3);

        return 0;
    }
}