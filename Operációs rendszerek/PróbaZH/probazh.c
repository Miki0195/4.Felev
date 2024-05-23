#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 256
#define MSG_KEY 1234

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

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Student has logged in.\n");
    } else if (signum == SIGUSR2) {
        printf("Supervisor has logged in.\n");
    }
}

void student_process(int fd[]) {
    close(fd[0]); // Close reading end of the pipe

    // Send signal to Neptun that student has logged in
    kill(getppid(), SIGUSR1);

    // Prepare the thesis
    Thesis thesis = {
        .title = "Záródoga",
        .student_name = "Buchsbaum Miklós",
        .submission_year = 2022,
        .supervisor_name = "Tatai Áron"
    };

    // Write the thesis to the pipe
    write(fd[1], &thesis, sizeof(Thesis));
    close(fd[1]); // Close the writing end of the pipe

    // Get the message queue
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Read the question from the message queue
    struct msg_buffer msg;
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Question from supervisor: %s\n", msg.msg_text);

    // Contemplate the answer
    sleep(5); // Simulate time taken to contemplate

    // Send answer to supervisor
    /*strcpy(msg.msg_text, "I would like to use C programming language.");
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }*/
}

void supervisor_process(int fd[]) {
    close(fd[1]); // Close writing end of the pipe

    // Send signal to Neptun that supervisor has logged in
    kill(getppid(), SIGUSR2);

    // Read the thesis from the pipe
    Thesis thesis;
    read(fd[0], &thesis, sizeof(Thesis));
    printf("--------------\n");
    printf("Thesis title: %s\n", thesis.title);
    printf("Student name: %s\n", thesis.student_name);
    printf("Submission year: %d\n", thesis.submission_year);
    printf("Supervisor name: %s\n", thesis.supervisor_name);
    printf("--------------\n");
    close(fd[0]); // Close the reading end of the pipe

    // Get the message queue
    int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Send question to student
    struct msg_buffer msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "What technology would you like to use to implement your task?");
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    // Wait for answer from student
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Supervisor:%s\n", msg.msg_text);
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

    // Set up signal handler
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

    // Parent process reads the thesis from the student process
    Thesis thesis;
    read(neptun_student_pipe[0], &thesis, sizeof(Thesis));

    // Parent process sends the thesis to the supervisor process
    write(neptun_supervisor_pipe[1], &thesis, sizeof(Thesis));

    // Wait for student and supervisor processes to finish
    wait(NULL);
    printf("Student process (child) finished.\n");
    wait(NULL);
    printf("Supervisor process (grandchild) finished.\n");
    printf("Neptun process (parent) finished.\n");

    return 0;
}