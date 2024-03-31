#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pty.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define MARGIN_SIZE 2
#define MAX_MARGINS 80
#define MARGIN_CHAR ' '

void add_margin_and_print(const char *line) {
    char margin[MARGIN_SIZE + 1];  // NUL終端を含むマージン文字列
    memset(margin, ' ', MARGIN_SIZE);  // スペースで埋める
    margin[MARGIN_SIZE] = '\0';  // NUL終端

    printf("%s%s\n", margin, line);  // マージンを追加して出力
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int master_fd, slave_fd;
    pid_t pid;
    char buf[BUFFER_SIZE];
    fd_set fd_in;
    int rc;
    char margin[MARGIN_SIZE + 1]; // NUL終端用の1を足す
    memset(margin, MARGIN_CHAR, MARGIN_SIZE);
    margin[MARGIN_SIZE] = '\0'; 

    // ptyを開く
    if (openpty(&master_fd, &slave_fd, NULL, NULL, NULL) == -1) {
        perror("openpty");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(master_fd);
        setsid();
        ioctl(slave_fd, TIOCSCTTY, NULL);

        dup2(slave_fd, STDIN_FILENO);
        dup2(slave_fd, STDOUT_FILENO);
        dup2(slave_fd, STDERR_FILENO);
        
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } 

    // 子プロセスの成功した場合の親プロセスのコード
    // closeやwaitpidは子プロセスによらず実行する
    close(slave_fd);

    while(1) {
        FD_ZERO(&fd_in);
        FD_SET(master_fd, &fd_in);

        rc = select(master_fd + 1, &fd_in, NULL, NULL, NULL);
        if(rc == -1) {
            // EINTR以外はselectエラー
            if(errno != EINTR) {
                perror("select");
                break;
            }
            continue;
        }

        if (FD_ISSET(master_fd, &fd_in)) {
            ssize_t num_read = read(master_fd, buf, sizeof(buf) - 1);
            if (num_read <= 0) // 子プロセスが終了またはエラー
                break;
            
            buf[num_read] = '\0'; // null terminate the buffer

            char *line = strtok(buf, "\n");
            while (line != NULL) {
                add_margin_and_print(line);
                line = strtok(NULL, "\n");
            }
        }
    }

    close(master_fd);
    waitpid(pid, NULL, 0); // 子プロセスの終了を待つ
    return EXIT_SUCCESS;
}
