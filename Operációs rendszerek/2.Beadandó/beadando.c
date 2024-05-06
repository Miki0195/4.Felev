#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define MAX_VERS 100
#define MAX_VERS_LENGTH 1000
#define NUMBER_OF_CHILDREN 4

int pipefd[2];
char poems[MAX_VERS][MAX_VERS_LENGTH];
int poem_count;

void signal_handler(int signum) {
    int index1 = rand() % poem_count;
    int index2;
    do {
        index2 = rand() % poem_count;
    } while(index2 == index1);
    write(pipefd[1], &index1, sizeof(index1));
    write(pipefd[1], poems[index1], MAX_VERS_LENGTH);
    write(pipefd[1], &index2, sizeof(index2));
    write(pipefd[1], poems[index2], MAX_VERS_LENGTH);
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
    if (strlen(poem) < 2 || strlen(poem) > MAX_VERS_LENGTH - 1)
    {
        printf("Error: The poem is too long or too short!\n");
        return;
    }
    int i;
    for (i = 0; i < strlen(poem); i++) {
        if (poem[i] == ' ') {
            i++;
        }
    }
    if (i == strlen(poem)) {
        printf("Error: The poem cannot consist only of blank spaces!\n");
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

    FILE *fileReRead = fopen(filename, "r");
    if (fileReRead == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return;
    }

    poem_count = 0;
    while (fgets(poems[poem_count], sizeof(poems[poem_count]), file) != NULL) {
        poem_count++;
    }

    fclose(fileReRead);
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    if (!fileExists(filename)) {
        createFileOption(filename);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Failed to open the file for reading!\n");
        return -1;
    }
    
    poem_count = 0;
    while (fgets(poems[poem_count], sizeof(poems[poem_count]), file) != NULL) {
        poem_count++;
    }

    fclose(file);

    int choice, index;
    char poem[MAX_VERS_LENGTH];
    int sync_pipe[2];

    struct msgbuf {
        long mtype;
        char mtext[MAX_VERS_LENGTH];
    };
    do {
        printf("\n------------------\n");
        printf("1. Add new poem\n");
        printf("2. List the poems\n");
        printf("3. Modify the poem\n");
        printf("4. Delete\n");
        printf("5. Sprinkle!\n");
        printf("0. Quit\n");
        printf("------------------\n");
        printf("Choose: ");
        //scanf("%d", &choice);
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }
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
                if(poem_count < 2){
                    printf("There are not enough poems to sprinkle!\n");
                    break;
                }

                srand(time(NULL));
                int son = rand() % NUMBER_OF_CHILDREN + 1;
                printf("Son %d is selected for sprinkling.\n\n", son);

                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                
                pid_t pid = fork();
                
                if (pid < 0) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (pid == 0) { // Child process
                    kill(getppid(), SIGUSR1); // Send signal to parent

                    int index1, index2;
                    char rhyme1[MAX_VERS_LENGTH], rhyme2[MAX_VERS_LENGTH];
                    read(pipefd[0], &index1, sizeof(index1));
                    read(pipefd[0], rhyme1, MAX_VERS_LENGTH);
                    read(pipefd[0], &index2, sizeof(index2));
                    read(pipefd[0], rhyme2, MAX_VERS_LENGTH);
                    //printf("\n");
                    printf("Received poems:\n");
                    printf("1. %s\n", rhyme1);
                    printf("2. %s\n", rhyme2);

                    int selectedIndex = rand() % 2 == 0 ? index1 : index2;
                    char *selectedPoem = selectedIndex == index1 ? rhyme1 : rhyme2;
                    printf("Selected poem: %s\n", selectedPoem);

                    key_t key = ftok("queue", 65);
                    int msgid = msgget(key, 0666 | IPC_CREAT);
                    struct msgbuf message;
                    message.mtype = 1;
                    sprintf(message.mtext, "%d. %s", selectedIndex, selectedPoem);
                    msgsnd(msgid, &message, sizeof(message.mtext), 0);

                    printf("May I sprinkle?\n");
                    exit(EXIT_SUCCESS);
                } else { // Parent process
                    signal(SIGUSR1, signal_handler); // Set up signal handler

                    pause(); // Wait for child to send signal

                    key_t key = ftok("queue", 65);
                    int msgid = msgget(key, 0666 | IPC_CREAT);
                    char selectedPoem[MAX_VERS_LENGTH];
                    struct msgbuf message;
                    msgrcv(msgid, &message, sizeof(message.mtext), 0, 0);

                    printf("------------------\n");
                    printf("Received selected poem: %s\n", message.mtext);

                    int selectedIndex;
                    sscanf(message.mtext, "%d", &selectedIndex);
                    //printf("Received selected index: %d\n", selectedIndex+1);
                    deletePoem(filename, selectedIndex+1);

                    printf("\nSon %d has returned home.", son);
                }
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
