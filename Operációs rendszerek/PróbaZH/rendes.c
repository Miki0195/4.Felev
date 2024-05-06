#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#define NUM_AREAS 7
#define FIND_ME SIGUSR1
#define YOU_CAN_RETURN_HOME SIGUSR2

// Define a structure for message queue
struct msg_buffer {
    long msg_type;
    int count;
};

// Global variables for message types
int msg_type_tapsi = 1;
int msg_type_fules = 2;

int should_exit = 0;

union semunM {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Signal handler for SIGUSR1 and SIGUSR2
void signal_handler(int signum) {
    int msg_type;
    if (getpid() == 1) {
        msg_type = msg_type_tapsi;
    } else {
        msg_type = msg_type_fules;
    }

    if (signum == FIND_ME) {
        printf("Received 'Find_me' signal.\n");
        // Generate a new random number between 70 and 100 for bunny count
        int count = rand() % 31 + 70;

        // Send the new count result to the parent process via message queue
        key_t key = ftok("progfile", 65);
        int msgid = msgget(key, 0666 | IPC_CREAT);
        struct msg_buffer message;
        message.msg_type = msg_type; // Use the global variable as the message type
        message.count = count;
        msgsnd(msgid, &message, sizeof(message), 0);
    } else if (signum == YOU_CAN_RETURN_HOME) {
        printf("Received 'You_can_return_home' signal.\n");
        should_exit = 1;
    }
}

// Bunny counting function
int count_bunnies() {
    // Generate a random number between 50 and 100 for bunny count
    return rand() % 51 + 50;
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Create message queue
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    // Create shared memory
    int shmid = shmget(key, 2 * sizeof(int), 0666 | IPC_CREAT);
    int *shm_array = (int *)shmat(shmid, NULL, 0);

    // Create semaphore
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    union semunM sem_union;
    sem_union.val = 1;
    if (semctl(semid, 0, SETVAL, sem_union) == -1) {
        perror("Failed to set semaphore value");
        return 1;
    }

    // Define areas
    char* areas[NUM_AREAS] = {"Friendtree", "Horsey", "Snakestream", "Cabbagy", "Szula", "Millstead", "Easter"};

    // Create pipes
    int tapsi_pipe[2];
    int fules_pipe[2];
    pipe(tapsi_pipe);
    pipe(fules_pipe);

    // Generate two different random indices
    int tapsi_index, fules_index;
    do {
        tapsi_index = rand() % NUM_AREAS;
        fules_index = rand() % NUM_AREAS;
    } while (tapsi_index == fules_index);

    // Write indices to pipes
    write(tapsi_pipe[1], &tapsi_index, sizeof(int));
    write(fules_pipe[1], &fules_index, sizeof(int));

    // Fork Tapsi and Füles
    pid_t tapsi_pid = fork();
    if (tapsi_pid == 0) {
        // Tapsi process
        srand(time(NULL) ^ (getpid()<<16));
        signal(FIND_ME, signal_handler);
        signal(YOU_CAN_RETURN_HOME, signal_handler);

        // Read area index from pipe
        int area_index;
        read(tapsi_pipe[0], &area_index, sizeof(int));

        printf("Tapsi is counting bunnies in %s.\n", areas[area_index]);
        int count = count_bunnies();

        // Send the count result to the parent process via message queue
        struct msg_buffer message;
        message.msg_type = 1; // Use the PID as the message type
        message.count = count;
        if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
            perror("msgsnd failed in child process");
            exit(1);
        }
        msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0);

        // Send the count result to the parent process via shared memory
        struct sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1; // P operation
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);
        shm_array[0] = count;
        sem_op.sem_op = 1; // V operation
        semop(semid, &sem_op, 1);

        pause(); // Wait for signal from parent
        while (!should_exit) {
            pause(); // Wait for signal from parent
        }
        exit(0);
    }

    pid_t fules_pid = fork();
    if (fules_pid == 0) {
        // Füles process
        srand(time(NULL) ^ (getpid()<<16));
        signal(FIND_ME, signal_handler);
        signal(YOU_CAN_RETURN_HOME, signal_handler);

        // Read area index from pipe
        int area_index;
        read(fules_pipe[0], &area_index, sizeof(int));

        printf("Füles is counting bunnies in %s.\n", areas[area_index]);
        int count = count_bunnies();

        // Send the count result to the parent process via message queue
        struct msg_buffer message;
        message.msg_type = 2; // Use the PID as the message type
        message.count = count;
        if (msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0) == -1) {
            perror("msgsnd failed in child process");
            exit(1);
        }
        msgsnd(msgid, &message, sizeof(message) - sizeof(long), 0);

        // Send the count result to the parent process via shared memory
        struct sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1; // P operation
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);
        shm_array[1] = count;
        sem_op.sem_op = 1; // V operation
        semop(semid, &sem_op, 1);


        pause(); // Wait for signal from parent

        while (!should_exit) {
            pause(); // Wait for signal from parent
        }
        exit(0);
    }

    // Parent process (Chief Bunny)
    printf("Chief Bunny is waiting for bunny counters to prepare...\n");
    sleep(1);
    // Read the results from shared memory and calculate the total count
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1; // P operation
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
    int total_count_tapsiM = shm_array[0];
    int total_count_fulesM = shm_array[1];
    sem_op.sem_op = 1; // V operation
    semop(semid, &sem_op, 1);

    // Read the results from message queue and calculate the total count
    struct msg_buffer message;
    int total_count_tapsi = 0, total_count_fules = 0;
    msgrcv(msgid, &message, sizeof(message), 1, 0); // Read message from Tapsi
    total_count_tapsi = message.count;
    msgrcv(msgid, &message, sizeof(message), 2, 0); // Read message from Füles
    total_count_fules = message.count;

    printf("Total bunny count from Tapsi: %d\n", total_count_tapsi);
    printf("Total bunny count from Füles: %d\n", total_count_fules);

    // Determine if there has been a decrease or increase in bunny population
    int previous_count_tapsi = (rand() % 51) + 50;
    int previous_count_fules = (rand() % 51) + 50;
    printf("Previous bunny count for Tapsi: %d\n", previous_count_tapsi);
    printf("Previous bunny count for Füles: %d\n", previous_count_fules);

    if (total_count_tapsi < previous_count_tapsi) {
        printf("There has been a decrease in bunny population in Tapsi's area.\n");
        printf("Sending 'Find_me' signal to Tapsi.\n");
        kill(tapsi_pid, FIND_ME);

        sleep(1);
        // Read the new count from Tapsi and print it
        msgrcv(msgid, &message, sizeof(message), 1, 0); // Read message from Tapsi
        printf("New bunny count from Tapsi: %d\n", message.count);
        printf("Sending 'You_can_return_home' signal to Tapsi.\n");
        kill(tapsi_pid, YOU_CAN_RETURN_HOME);
    } else {
        printf("There has been an increase or no change in bunny population in Tapsi's area.\n");
        printf("Sending 'You_can_return_home' signal to Tapsi.\n");
        kill(tapsi_pid, YOU_CAN_RETURN_HOME);
        waitpid(tapsi_pid, NULL, 0);
    }

    if (total_count_fules < previous_count_fules) {
        printf("There has been a decrease in bunny population in Füles's area.\n");
        printf("Sending 'Find_me' signal to Füles.\n");
        kill(fules_pid, FIND_ME);

        sleep(1);
        // Read the new count from Füles and print it
        msgrcv(msgid, &message, sizeof(message), 2, 0); // Read message from Füles
        printf("New bunny count from Füles: %d\n", message.count);
        printf("Sending 'You_can_return_home' signal to Füles.\n");
        kill(fules_pid, YOU_CAN_RETURN_HOME);
    } else {
        printf("There has been an increase or no change in bunny population in Füles's area.\n");
        printf("Sending 'You_can_return_home' signal to Füles.\n");
        kill(fules_pid, YOU_CAN_RETURN_HOME);
        waitpid(tapsi_pid, NULL, 0);
    }

    // Wait for both children to finish
    wait(NULL);
    wait(NULL);

    // Clean up shared memory and semaphore
    shmdt(shm_array);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, sem_union);

    // Clean up message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}