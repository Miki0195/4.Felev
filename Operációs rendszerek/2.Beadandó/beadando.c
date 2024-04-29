#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VERS_LENGTH 100

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
        printf("Quiting...\n");
        exit(0);
    }
}

// Selects a random poem for watering
int selectRandomPoemIndex(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return -1;
    }
    char poem[MAX_VERS_LENGTH];
    int count = 0;
    while (fgets(poem, sizeof(poem), file) != NULL) {
        count++;
    }
    fclose(file);
    if (count == 0) {
        printf("Error: There are no poems in the file!\n");
        return -1;
    }
    srand(time(NULL));
    return rand() % count;
}

// Sends a signal to mama and receives two verses from her
void communicateWithMama(const char *filename, char *verse1, char *verse2) {
    int random_index = selectRandomPoemIndex(filename);
    if (random_index == -1) {
        printf("Error: Communication with Mama failed!\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    char poem[MAX_VERS_LENGTH];
    int count = 0;
    while (fgets(poem, sizeof(poem), file) != NULL) {
        if (count == random_index) {
            strcpy(verse1, poem);
            break;
        }
        count++;
    }
    fclose(file);

    random_index = selectRandomPoemIndex(filename);
    if (random_index == -1) {
        printf("Error: Communication with Mama failed!\n");
        return;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }
    count = 0;
    while (fgets(poem, sizeof(poem), file) != NULL) {
        if (count == random_index) {
            strcpy(verse2, poem);
            break;
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

    do {
        printf("\n------------------\n");
        printf("1. Add new poem\n");
        printf("2. List the poems\n");
        printf("3. Modify the poem\n");
        printf("4. Delete\n");
        printf("5. Watering\n");
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
            case 5:
                communicateWithMama(filename, verse1, verse2);
                printf("Received verses from Mama:\n1. %s2. %s\n", verse1, verse2);
                printf("Selecting a verse...\n");
                srand(time(NULL));
                int selected_verse = rand() % 2;
                printf("Selected verse: %d. %s", selected_verse + 1, (selected_verse == 0) ? verse1 : verse2);
                printf("\n\nSzabad-e locsolni!\n");
                printf("Locsolás...\n");
                printf("Returning home...\n");
                break;
            case 0:
                printf("Quiting...\n");
                break;
            default:
                printf("Wrong number. Please choose once more!\n");
        }
    } while (choice != 0);

    return 0;
}
