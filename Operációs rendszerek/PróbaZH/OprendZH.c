#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/sem.h>

#define SEM_KEY 7890
#define BUFFER_SIZE 256
#define MSG_KEY 1234
#define SHM_KEY 5678


typedef struct {
    char title[BUFFER_SIZE];
    char student_name[BUFFER_SIZE];
    int submission_year;
    char supervisor_name[BUFFER_SIZE];
} Thesis;

// Define a structure for message queue because i don't have <mqueue.h> on MacOS (it's not supported)
struct msg_buffer {
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

// Define a structure for shared memory
typedef struct {
    int decision;
} SharedMemory;

// Define a semaphore union
union MySemun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Student has logged in.\n");
    } else if (signum == SIGUSR2) {
        printf("Supervisor has logged in.\n");
    }
}

void student_process(int fd[]) {
    close(fd[0]); 

    kill(getppid(), SIGUSR1);

    Thesis thesis = {
        .title = "Záródoga",
        .student_name = "Buchsbaum Miklós",
        .submission_year = 2022,
        .supervisor_name = "Tatai Áron"
    };

    write(fd[1], &thesis, sizeof(Thesis));
    close(fd[1]); 

    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer msg;
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Question from supervisor: %s\n", msg.msg_text);

    sleep(2); // Simulate time taken to contemplate

    // Create shared memory
    int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    SharedMemory *shm = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (shm->decision == 1) {
        printf("The supervisor accepted the topic.\n");
    } else {
        printf("The supervisor declined the topic.\n");
    }

    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Create semaphore
    int semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    struct sembuf sb = {0, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    sb.sem_op = 1;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    exit(0);
}

void supervisor_process(int fd[]) {
    srand(time(NULL));
    close(fd[1]); 

    kill(getppid(), SIGUSR2);

    Thesis thesis;
    read(fd[0], &thesis, sizeof(Thesis));
    printf("--------------\n");
    printf("Thesis title: %s\n", thesis.title);
    printf("Student name: %s\n", thesis.student_name);
    printf("Submission year: %d\n", thesis.submission_year);
    printf("Supervisor name: %s\n", thesis.supervisor_name);
    printf("--------------\n");
    close(fd[0]); 

    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "What technology would you like to use to implement your task?");
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    
    sleep(2); // Simulate time taken to contemplate

    // Create shared memory
    int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    SharedMemory *shm = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int random_number = rand() % 100;
    shm->decision = (random_number < 20) ? 0 : 1; // 20% chance to decline

    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Create semaphore
    int semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    union MySemun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    struct sembuf sb = {0, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    sb.sem_op = 1;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    exit(0);
}

int main() {
    // Pipes for communication between Neptun and the student
    int neptun_student_pipe[2];
    if (pipe(neptun_student_pipe) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Pipes for communication between Neptun and the supervisor
    int neptun_supervisor_pipe[2];
    if (pipe(neptun_supervisor_pipe) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    pid_t student_pid = fork();

    if (student_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (student_pid == 0) { // Student process
        student_process(neptun_student_pipe);
        exit(EXIT_SUCCESS);
    }

    // Wait for signal from student
    pause();

    pid_t supervisor_pid = fork();

    if (supervisor_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (supervisor_pid == 0) { // Supervisor process
        supervisor_process(neptun_supervisor_pipe);
        exit(EXIT_SUCCESS);
    }

    // Wait for signal from supervisor
    pause();

    Thesis thesis;
    read(neptun_student_pipe[0], &thesis, sizeof(Thesis));

    write(neptun_supervisor_pipe[1], &thesis, sizeof(Thesis));

    wait(NULL);
    printf("Student process (child) finished.\n");
    wait(NULL);
    printf("Supervisor process (grandchild) finished.\n");
    printf("Neptun process (parent) finished.\n");

    return 0;
}