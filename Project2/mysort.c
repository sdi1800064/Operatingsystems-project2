#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void createChildren(int n, int level);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]); // Μετατροπή του ορίσματος σε ακέραιο

    if (n <= 0) {
        fprintf(stderr, "Number of children must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    createChildren(n, 0);

    printf("Parent process (PID: %d) received control from all children.\n", getpid());

    return 0;
}

void createGrandChildren(int n, int parentPID, int level) {
    for (int i = 0; i < n; ++i) {
        pid_t gc_pid = fork();

        if (gc_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (gc_pid == 0) {
            // Κώδικας για τα εγγόνια
            printf("GC%d (PID: %d, Parent PID: %d, Level: %d)\n", i, getpid(), getppid(), level);
            // Δεν θέλουμε παιδιά για τα εγγόνια
            exit(EXIT_SUCCESS);
        } else {
            // Περιμένει για την ολοκλήρωση του εγγονιού
            wait(NULL);
        }
    }
}

void createChildren(int n, int level) {
    for (int i = 0; i < n; ++i) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Κώδικας για τα παιδιά
            printf("C%d (PID: %d, Parent PID: %d, Level: %d)\n", i, getpid(), getppid(), level);
            // Κάλεσε τα εγγόνια μόνο αν είναι το πρώτο επίπεδο (τα παιδιά του γονέα)
            if (level == 0) {
                createGrandChildren(n - i, getpid(), level + 1);
            }
            exit(EXIT_SUCCESS);
        } else {
            // Περιμένει για την ολοκλήρωση του παιδιού
            wait(NULL);
        }
    }
}
