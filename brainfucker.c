#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    // Get commandline parameters.
    if (argc != 3) {
        fputs("usage: brainfucker <cell-count> <program>\n", stderr);
        exit(1);
    }
    size_t cells_len;
    if (sscanf(argv[1], "%zi", &cells_len) != 1) {
        fprintf(stderr, "cell-count parameter (%s) must be an integer\n", argv[1]);
    }
    char *program = argv[2];

    // Interpret per https://esolangs.org/wiki/Brainfuck#Language_overview .
    uint64_t instructions_executed = 0;
    uint8_t *cells = (uint8_t *)calloc(1, cells_len);
    for (int ip = 0, dp = 0; ip < strlen(program); instructions_executed++) {
        switch (program[ip]) {

        case '>':
            dp++;
            if (dp >= cells_len) {
                fprintf(stderr, "malformed program or bad parameters: dp >= cells_len\n");
                exit(1);
            }
            ip++;
            break;

        case '<':
            dp--;
            if (dp < 0) {
                fprintf(stderr, "malformed program or bad parameters: dp < 0\n");
                exit(1);
            }
            ip++;
            break;

        case '+':
            cells[dp]++;
            ip++;
            break;

        case '-':
            cells[dp]--;
            ip++;
            break;

        case '.':
            putchar(cells[dp]);
            ip++;
            break;

        case ',':
            cells[dp] = getchar();
            ip++;
            break;

        case '[':
            ip++;
            if (cells[dp] == 0) {
                for (int nesting = 1; nesting != 0 && ip < strlen(program); ip++) {
                    if (program[ip] == '[') {
                        nesting++;
                    } else if (program[ip] == ']') {
                        nesting--;
                    }
                }
                if (ip >= strlen(program)) {
                    fprintf(stderr, "malformed program: ip >= strlen(program)\n");
                }
            }
            break;

        case ']':
            if (cells[dp] != 0) {
                ip--;
                if (ip < 0) {
                    fprintf(stderr, "malformed program: ip < 0\n");
                }

                for (int nesting = 1; nesting != 0 && ip >= 0; ip--) {
                    if (program[ip] == ']') {
                        nesting++;
                    } else if (program[ip] == '[') {
                        nesting--;
                    }
                }
                if (ip < 0) {
                    fprintf(stderr, "malformed program: ip < 0\n");
                }
            }
            ip++;
            break;
        }
    }

    fprintf(stderr, "instructions executed: %" PRId64 "\n", instructions_executed);

    return 0;
}
