# include <math.h>
# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

struct WordCount {
    char word[50];
    int count;
};

int main ( int argc, char *argv[] ){
    // MPI variables
    int i;
    int id;
    int ierr;
    int p;
    double wtime;

    // Variables for word counting
    FILE *completeFile;

    // Initialize MPI.
    //ierr = MPI_Init ( &argc, &argv );

    // Get the number of processes.
    //ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );

    // Determine this processes's rank.

    //ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );

    if ( id == 0 )
    {
        // timestamp ( );
        printf ( "\n" );
        printf ( "PRIME_MPI\n" );
        printf ( "  C/MPI version\n" );
        printf ( "\n" );
        printf ( "  An MPI example program to count the number of primes.\n" );
        printf ( "  The number of processes is %d\n", p );
        printf ( "\n" );
        printf ( "         N        Pi          Time\n" );
        printf ( "\n" );
    }

    // Reads the decrypted file.

    // Open the file to save the decrypted file
    completeFile = fopen("/home/andres/Escritorio/PSO-Proyecto2-RoboticTEC/Files/DecryptedFile.txt", "rb");
    if (completeFile == NULL) {
        perror("Error opening the file");
    }

    // Separates the file in three leght-equal files.

    // Get file size
    fseek(completeFile, 0, SEEK_END);
    long fileSize = ftell(completeFile);
    rewind(completeFile);

    // Allocate memory for full file content
    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Memory allocation failed");
        fclose(completeFile);
        return 1;
    }

    fread(fileContent, 1, fileSize, completeFile);
    fileContent[fileSize] = '\0';
    fclose(completeFile);

    // Count total words
    int totalWords = 0, inWord = 0;
    for (char *ptr = fileContent; *ptr; ptr++) {
        if (isspace(*ptr)) {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            totalWords++;
        }
    }

    int wordsPerPart = totalWords / 3;

    // Initializes countPart array to have a size of wordsPerPart
    struct WordCount countPart[wordsPerPart];
    struct WordCount totalCount[totalWords];

    // Allocate output buffers
    char *part1 = (char *)malloc(fileSize + 1);
    char *part2 = (char *)malloc(fileSize + 1);
    char *part3 = (char *)malloc(fileSize + 1);
    if (!part1 || !part2 || !part3) {
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

    while (*src) {
        if (isspace(*src)) {
            if (inWord) {
                wordCount++;
                inWord = 0;
                if (wordCount == wordsPerPart) currentPart++;
                if (wordCount == 2 * wordsPerPart) currentPart++;
            }
        } else {
            if (!inWord) inWord = 1;
        }

        if (currentPart == 1) *dst1++ = *src;
        else if (currentPart == 2) *dst2++ = *src;
        else *dst3++ = *src;

        src++;
    }

    *dst1 = '\0';
    *dst2 = '\0';
    *dst3 = '\0';

    // Write to output files
    FILE *part1File = fopen("../Files/DecryptedFilePart1.txt", "wb");
    FILE *part2File = fopen("../Files/DecryptedFilePart2.txt", "wb");
    FILE *part3File = fopen("../Files/DecryptedFilePart3.txt", "wb");

    if (part1File) fwrite(part1, 1, strlen(part1), part1File);
    if (part2File) fwrite(part2, 1, strlen(part2), part2File);
    if (part3File) fwrite(part3, 1, strlen(part3), part3File);

    // Clean up
    if (part1File) fclose(part1File);
    if (part2File) fclose(part2File);
    if (part3File) fclose(part3File);

    free(fileContent);

    // Counts the times a word appear on the file according to which client is working (id)

    id = 0; // Assuming id is set to 0 for this example, replace with actual MPI rank
    char word[256];
    wordCount = 0;
    char buffer[50];

    switch (id) {
        case 0:
            // Reads each word of part1, if doesnt exists on countPart, adds it with count 1, if does, increments its count
            part1File = fopen("../Files/DecryptedFilePart1.txt", "r");
            if (part1File == NULL) {
                perror("Error opening part 1 file");
            }
            
            while (fscanf(part1File, "%49s", buffer) == 1) {
                // Convert to lowercase
                for (int i = 0; buffer[i]; i++) {
                    buffer[i] = tolower(buffer[i]);
                }

                // Remove punctuation from the end (optional)
                while (strlen(buffer) > 0 && !isalpha(buffer[strlen(buffer)-1])) {
                    buffer[strlen(buffer)-1] = '\0';
                }

                // Search for the word in the array
                int found = 0;
                for (int i = 0; i < wordCount; i++) {
                    if (strcmp(countPart[i].word, buffer) == 0) {
                        countPart[i].count++;
                        found = 1;
                        break;
                    }
                }

                // If not found, add new word
                if (!found && wordCount < wordsPerPart) {
                    strcpy(countPart[wordCount].word, buffer);
                    countPart[wordCount].count = 1;
                    wordCount++;
                }
            }
            
            fclose(part1File);

            break;
        
        case 1:
            // Reads each word of part2, if doesnt exists on countPart, adds it with count 1, if does, increments its count
            part2File = fopen("../Files/DecryptedFilePart2.txt", "rb");
            if (part2File == NULL) {
                perror("Error opening part 2 file");
            }

            // Read each word and update countPart
            while (fscanf(part2File, "%255s", word) == 1) {
                // Update countPart
                if (countPart->count == 0) {
                    strcpy(countPart->word, word);
                    countPart->count = 1;
                } else if (strcmp(countPart->word, word) == 0) {
                    countPart->count++;
                }
            }
            fclose(part2File);
            break;

        case 2:
            // Reads each word of part3, if doesnt exists on countPart, adds it with count 1, if does, increments its count
            part3File = fopen("../Files/DecryptedFilePart3.txt", "rb");
            if (part3File == NULL) {
                perror("Error opening part 3 file");
            }

            // Read each word and update countPart
            while (fscanf(part3File, "%255s", word) == 1) {
                // Update countPart
                if (countPart->count == 0) {
                    strcpy(countPart->word, word);
                    countPart->count = 1;
                } else if (strcmp(countPart->word, word) == 0) {
                    countPart->count++;
                }
            }

            fclose(part3File);
            break;
        }

        // Prints the content of countPart
        for (int i = 0; i < wordCount; i++) {
            printf("Process %d: Word '%s' appears %d times.\n", id, countPart[i].word, countPart[i].count);
        }

    // Uses MPI_Reduce to merge all dictionaries created

    /*
    n = n_lo;

    while ( n <= n_hi )
    {
        if ( id == 0 )
        {
        wtime = MPI_Wtime ( );
        }
        ierr = MPI_Bcast ( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );

        primes_part = prime_number ( n, id, p );

        ierr = MPI_Reduce ( &primes_part, &primes, 1, MPI_INT, MPI_SUM, 0, 
        MPI_COMM_WORLD );

        if ( id == 0 )
        {
        wtime = MPI_Wtime ( ) - wtime;
        printf ( "  %8d  %8d  %14f\n", n, primes, wtime );
        }
        n = n * n_factor;
    }
    */

    /*
    Terminate MPI.
    */
    // ierr = MPI_Finalize ( );
    /*
    Terminate.
    */
    if ( id == 0 ) 
    {
        printf ( "\n");         
        printf ( "PRIME_MPI - Master process:\n");         
        printf ( "  Normal end of execution.\n"); 
        printf ( "\n" );
        // timestamp ( );        
    }

    // Free allocated memory
    free(part1);
    free(part2);
    free(part3);

    return 0;
    }

    /*

    int prime_number ( int n, int id, int p )

    /*
    Purpose:

        PRIME_NUMBER returns the number of primes between 1 and N.

    Discussion:

        In order to divide the work up evenly among P processors, processor
        ID starts at 2+ID and skips by P.

        A naive algorithm is used.

        Mathematica can return the number of primes less than or equal to N
        by the command PrimePi[N].

                    N  PRIME_NUMBER

                    1           0
                10           4
                100          25
                1,000         168
            10,000       1,229
            100,000       9,592
            1,000,000      78,498
        10,000,000     664,579
        100,000,000   5,761,455
        1,000,000,000  50,847,534

    Licensing:

        This code is distributed under the GNU LGPL license. 

    Modified:

        21 May 2009

    Author:

        John Burkardt

    Parameters:

        Input, int N, the maximum number to check.

        Input, int ID, the ID of this process,
        between 0 and P-1.

        Input, int P, the number of processes.

        Output, int PRIME_NUMBER, the number of prime numbers up to N.
    */
    /*
    {
    int i;
    int j;
    int prime;
    int total;

    total = 0;

    for ( i = 2 + id; i <= n; i = i + p )
    {
        prime = 1;
        for ( j = 2; j < i; j++ )
        {
        if ( ( i % j ) == 0 )
        {
            prime = 0;
            break;
        }
        }
        total = total + prime;
    }
    return total;
    }*/
    /******************************************************************************/

    //void timestamp ( void )

    /******************************************************************************/
    /*
    Purpose:

        TIMESTAMP prints the current YMDHMS date as a time stamp.

    Example:

        31 May 2001 09:45:54 AM

    Licensing:

        This code is distributed under the GNU LGPL license. 

    Modified:

        24 September 2003

    Author:

        John Burkardt

    Parameters:

        None
    */
    /*{
    # define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    size_t len;
    time_t now;

    now = time ( NULL );
    tm = localtime ( &now );

    len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

    printf ( "%s\n", time_buffer );

    return;
    # undef TIME_SIZE
    }*/