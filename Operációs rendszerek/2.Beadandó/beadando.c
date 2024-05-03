#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h> // For boolean type

#define MAX_VERS_LENGTH 100
#define PIPE_READ_END 0
#define PIPE_WRITE_END 1
#define MAX_POEM_COUNT 100
char used_verses[MAX_POEM_COUNT][MAX_VERS_LENGTH] = {""}; // Initialize all elements to empty strings
int used_verse_count = 0;

// Check if a verse has been used
bool isVerseUsed(const char *verse) {
    for (int i = 0; i < used_verse_count; ++i) {
        if (strcmp(used_verses[i], verse) == 0) {
            return true;
        }
    }
    return false;
}

// Mark a verse as used
void markVerseAsUsed(const char *verse) {
    strcpy(used_verses[used_verse_count++], verse);
}

//Checks if the file exists
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

//Adds a new poem to the file
void addPoemToFile(const char *filename, const char *poem) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    fprintf(file, "%s", poem);
    fclose(file);
    printf("The poem was successfully added.\n");
}

//Lists out the poems in the file
void listPoems(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    char poem[MAX_VERS_LENGTH];
    int count = 1;
    printf("Easter poems:\n");
    while (fgets(poem, sizeof(poem), file) != NULL) {
        printf("%d. %s", count, poem);
        count++;
    }
    fclose(file);
}

//Modifies the poem
void modifyPoem(const char *filename, int index, const char *newPoem) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    char poems[MAX_VERS_LENGTH][MAX_VERS_LENGTH];
    int count = 0;
    while (fgets(poems[count], sizeof(poems[count]), file) != NULL) {
        count++;
    }
    fclose(file);

    if (index < 1 || index > count) {
        printf("Error: The given index was not found!\n");
        return;
    }

    strcpy(poems[index - 1], newPoem);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to open the file for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s", poems[i]);
    }
    fclose(file);
    printf("The poem was successfully modified!\n");
}

//Deletes the poem at the given index
void deletePoem(const char *filename, int index) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    char poems[MAX_VERS_LENGTH][MAX_VERS_LENGTH];
    int count = 0;
    while (fgets(poems[count], sizeof(poems[count]), file) != NULL) {
        count++;
    }
    fclose(file);

    if (index < 1 || index > count) {
        printf("Error: The given index was not found!\n");
        return;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to open the file for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        if (i != index - 1) {
            fprintf(file, "%s", poems[i]);
        }
    }
    fclose(file);
    printf("The poem was successfully deleted!\n");
}

//If the given .txt in the parameter does not exist you have the option to create it.
void createFileOption(const char *filename) {
    char response;
    printf("The given file does not exist. Would you like to create it? (y/n): ");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error: Failed to create the file!\n");
            exit(1);
        }
        fclose(file);
        printf("The file has been successfully created!\n");
    } else {
        printf("Quitting...\n");
        exit(0);
    }
}

int selectRandomPoemIndex(const char *filename, int previous_index) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return -1;
    }
    char poem[MAX_VERS_LENGTH];
    int count = 0;
    int valid_indices[MAX_VERS_LENGTH];
    while (fgets(poem, sizeof(poem), file) != NULL) {
        if (count != previous_index) {
            valid_indices[count] = 1;
        }
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("Error: There are no poems in the file!\n");
        return -1;
    }

    srand(time(NULL));
    int random_index;
    do {
        random_index = rand() % count;
    } while (valid_indices[random_index] != 1);

    return random_index;
}

// Sends a signal to mama and receives two verses from her
// Sends a signal to mama and receives two verses from her
void communicateWithMama(const char *filename, char *verse1, char *verse2, int previous_index) {
    int random_index1 = selectRandomPoemIndex(filename, previous_index);
    if (random_index1 == -1) {
        printf("Error: Communication with Mama failed!\n");
        return;
    }

    int random_index2;
    do {
        random_index2 = selectRandomPoemIndex(filename, random_index1);
    } while (random_index2 == -1 || random_index2 == random_index1);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    char poem[MAX_VERS_LENGTH];
    int count = 0;
    while (fgets(poem, sizeof(poem), file) != NULL) {
        if (count == random_index1) {
            strcpy(verse1, poem);
        }
        if (count == random_index2) {
            strcpy(verse2, poem);
        }
        count++;
    }
    fclose(file);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    if (!fileExists(filename)) {
        createFileOption(filename);
    }

    int choice, index;
    char poem[MAX_VERS_LENGTH];
    char verse1[MAX_VERS_LENGTH];
    char verse2[MAX_VERS_LENGTH];
    int previous_index = -1;
    int pipefd[2]; // Pipe file descriptors

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    do {
        printf("\n------------------\n");
        printf("1. Add new poem\n");
        printf("2. List the poems\n");
        printf("3. Modify the poem\n");
        printf("4. Delete\n");
        printf("5. Locsolás\n");
        printf("0. Quit\n");
        printf("------------------\n");
        printf("Choose: ");
        scanf("%d", &choice);
        printf("------------------\n");
        getchar(); // Flush input buffer

        switch (choice) {
            case 1:
                printf("Type in the new poem: ");
                fgets(poem, sizeof(poem), stdin);
                addPoemToFile(filename, poem);
                break;
            case 2:
                listPoems(filename);
                break;
            case 3:
                printf("Give the index of the poem to be modified: ");
                scanf("%d", &index);
                getchar(); // Flush input buffer
                printf("Type in the modification: ");
                fgets(poem, sizeof(poem), stdin);
                modifyPoem(filename, index, poem);
                break;
            case 4:
                printf("Give the index of the poem to be deleted: ");
                scanf("%d", &index);
                getchar(); // Flush input buffer
                deletePoem(filename, index);
                break;
            case 5: {
                pid_t pid = fork();
                if (pid == 0) { // Child process for locsolás
                    communicateWithMama(filename, verse1, verse2, previous_index);
                    printf("Received verses from Mama:\n1. %s\n2. %s\n", verse1, verse2);
                    printf("Selecting a verse...\n");
                    srand(time(NULL));
                    int selected_verse = rand() % 2;
                    printf("Selected verse index: %d\n", selected_verse);
                    // Write selected verse index to pipe
                    write(pipefd[PIPE_WRITE_END], &selected_verse, sizeof(selected_verse));
                    // Write selected verse to pipe
                    write(pipefd[PIPE_WRITE_END], (selected_verse == 0) ? verse1 : verse2, MAX_VERS_LENGTH);
                    exit(0); // Child process exits after locsolás
                } else if (pid > 0) { // Parent process continues menu
                    wait(NULL); // Wait for child process to finish
                    int selected_verse_index;
                    char selected_verse_text[MAX_VERS_LENGTH];
                    // Read selected verse index from pipe
                    read(pipefd[PIPE_READ_END], &selected_verse_index, sizeof(selected_verse_index));
                    // Read selected verse from pipe
                    read(pipefd[PIPE_READ_END], selected_verse_text, MAX_VERS_LENGTH);
                    printf("Selected verse index: %d\n", selected_verse_index);
                    printf("Selected verse: %s\n", selected_verse_text);
                    if (!isVerseUsed(selected_verse_text)) {
                        printf("\n\nSzabad-e locsolni!\n");
                        printf("Locsolás...\n");
                        printf("Returning home...\n");
                        previous_index = selected_verse_index;
                        markVerseAsUsed(selected_verse_text); // Mark selected verse as used
                    } else {
                        printf("The selected verse has already been used. Choose another verse!\n");
                    }
                } else {
                    printf("Error: Fork failed!\n");
                }
                break;
            }



            case 0:
                printf("Quiting...\n");
                break;
            default:
                printf("Wrong number. Please choose once more!\n");
        }
    } while (choice != 0);

    return 0;
}