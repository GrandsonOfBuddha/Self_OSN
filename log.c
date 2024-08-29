#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define LOG_FILE "command_log.txt"
#define MAX_LOG_SIZE 15

void log_command(const char *command) {
    // Open the log file
    FILE *file = fopen(LOG_FILE, "r+");
    if (file == NULL) {
        file = fopen(LOG_FILE, "w+");
        if (file == NULL) {
            perror("Error opening log file");
            return;
        }
    }

    char current_log[MAX_LOG_SIZE][1024];
    int count = 0;

    // Load existing log entries into memory
    while (fgets(current_log[count], sizeof(current_log[count]), file) != NULL) {
        current_log[count][strcspn(current_log[count], "\n")] = '\0'; // Remove newline
        if (strcmp(current_log[count], command) == 0) {
            fclose(file);
            return; // Do not log duplicate commands
        }
        count++;
    }

    // Append the new command if it's not the same as the last command
    if (count == 0 || strcmp(current_log[count - 1], command) != 0) {
        if (count < MAX_LOG_SIZE) {
            strcpy(current_log[count], command);
            count++;
        } else {
            // Shift log entries if the log is full
            for (int i = 1; i < MAX_LOG_SIZE; i++) {
                strcpy(current_log[i - 1], current_log[i]);
            }
            strcpy(current_log[MAX_LOG_SIZE - 1], command);
        }
    }

    // Rewrite the log file
    freopen(LOG_FILE, "w", file);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", current_log[i]);
    }
    fclose(file);
}

void display_log() {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) {
        printf("No log available.\n");
        return;
    }

    char line[1024];
    int index = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        printf("%d: %s\n", index++, line);
    }
    fclose(file);
}

void purge_log() {
    // Open the log file in write mode to clear its contents
    FILE *file = fopen(LOG_FILE, "w");
    if (file != NULL) {
        fclose(file);
    }
}

void execute_log_command(int index) {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) {
        printf("No log available.\n");
        return;
    }

    char line[1024];
    int current_index = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_index == index) {
            line[strcspn(line, "\n")] = '\0'; // Remove newline
            fclose(file);

            printf("Executing: %s\n", line);

            // Execute the command
            if (system(line) == -1) {
                perror("Error executing command");
            } else {
                // If it's not the most recent command, log it
                if (current_index != 1) {
                    log_command(line);
                }
            }
            return;
        }
        current_index++;
    }
    printf("Invalid log index.\n");
    fclose(file);
}
