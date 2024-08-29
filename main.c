#include <stdio.h>
#include <stdlib.h>
#include "prompt.h"
#include "input.h"
#include "command.h"
#include "signal.h"

int main() {
    setup_signal_handler();  // Set up the signal handler at the start of the program
    char input[1024];

    while (1) {
        display_prompt();   // Display the custom shell prompt
        if (get_input(input, sizeof(input)) == 0) {
            continue;
        }

        // Process the input (handles ';' and '&' separated commands)
        process_input(input);
    }

    return 0;
}
