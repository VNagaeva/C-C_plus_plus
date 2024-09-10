#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h> 
#include <stdarg.h>
#include <unistd.h> 

#define BUF_SIZE 3000

int count_signals_1 = 0;
int count_signals_2 = 0;
int end_read = 0;
int first_break = 0;
int second_break = 0;

char file[2][BUF_SIZE];
pid_t pid;

void InfoFile(int flag) {
    if (flag == 1 && first_break == 0) {
        first_break = 1;
        printf("\nFirst file: \n");
        char command[BUF_SIZE];
        sprintf(command, "wc -lwm %s", file[0]);
        if (system(command) == -1) {
            printf("System error\n");
            exit(1);
        }
    }
    if (flag == 2 && second_break == 0) {
        second_break = 1;
        printf("\nSecond file: \n");
        char command[BUF_SIZE];
        sprintf(command, "wc -lwm %s", file[1]);
        if (system(command) == -1) {
            printf("System error\n");
            exit(1);
        }
    }
}

void SigintHandler() {
    if (end_read == 0) {
        count_signals_2++;
        count_signals_1 = count_signals_2;
        return;
    }

    count_signals_1++;
    if (count_signals_1 == 1) { InfoFile(1); } 
    else if (count_signals_1 == 2) { InfoFile(2); }
}

int isFileValid(char *filename) {
    scanf("%s", filename);
    if (open(filename, O_RDONLY)  != -1) {
        return 1;
    }
    return 0;
}

int main() {
	struct sigaction sa;
	sa.sa_handler = SigintHandler;
    sigprocmask(0, 0, &sa.sa_mask); 
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);

    do {    
        printf("Enter the first file name: \n");
    } while (isFileValid(file[0]) == 0);
    do {
        printf("Enter the second file name: \n");
    } while (isFileValid(file[1]) == 0);

    end_read = 1;
    if (count_signals_2 >= 1) { InfoFile(1); } 
    if (count_signals_2 >= 2) { InfoFile(2); }

    int fd[2];
    //sleep(5);
    if (pipe(fd) == -1) {
        printf("Error in pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
    	printf("Error in fork");
    	exit(1);
    }
    //sleep(5);
    if (pid == 0) {
    	// child
        close(fd[0]);
        dup2(fd[1], 1);

        char command[BUF_SIZE];
        sprintf(command, "cat %s %s", file[0], file[1]);
        if (system(command) == -1) {
            printf("System error\n");
            exit(1);
        }

        close(fd[1]);
    } else {
    	//parent
        wait(NULL);
        close(fd[1]);

        dup2(fd[0], 0);
        printf("\nMerged file:\n");
        execlp("wc", "-lwm", NULL);
        
        close(fd[0]);
    }
}
