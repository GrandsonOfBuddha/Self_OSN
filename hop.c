#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hop.h"

static char prev_dir[1024] = "";

void execute_hop(char *args[]) {
    char cwd[1024];
    char *home_dir = getenv("HOME");

    if (args[1] == NULL) {
        if (chdir(home_dir) != 0) {
            perror("ERROR");
        }
    } else {
        for (int i = 1; args[i] != NULL; i++) {
            char *path = args[i];

            if (strcmp(path, "~") == 0) {
                path = home_dir;
            } else if (strcmp(path, "-") == 0) {
                if (strlen(prev_dir) == 0) {
                    fprintf(stderr, "ERROR: No previous directory\n");
                    return;
                }
                path = prev_dir;
            } else if (strcmp(path, ".") == 0) {
                continue;
            } else if (strcmp(path, "..") == 0) {
                path = "..";
            } else if (path[0] == '~') {
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s%s", home_dir, path + 1);
                path = full_path;
            }

            if (chdir(path) != 0) {
                perror("ERROR");
                return;
            }

            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                strcpy(prev_dir, cwd);
                printf("%s\n", cwd);
            } else {
                perror("getcwd() error");
                return;
            }
        }
    }
}
