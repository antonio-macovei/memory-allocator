/* Copyright 2017 Macovei Antonio Dan */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1000
#define MAX_ARGS 5

int readCmd(int *argc, char *args[MAX_ARGS]);
void freeCmdMem(int argc, char *args[MAX_ARGS]);
void initialize(char *args[MAX_ARGS], unsigned char **arena, int *size);
void finalize(unsigned char *arena);
void dump(unsigned char *arena, int size);
void alloc(char *args[MAX_ARGS], unsigned char **arena, int size);
void freeExec(char *args[MAX_ARGS], unsigned char **arena, int size);
void fill(char *args[MAX_ARGS], unsigned char **arena);
void show(char *args[MAX_ARGS], unsigned char *arena, int size);
void showFree(unsigned char *arena, int size);
void showUsage(unsigned char *arena, int size);
void showAllocations(unsigned char *arena, int size);
/*void printArena(unsigned char *arena, int size);*/

int main() {
    int argc;
    char *args[MAX_ARGS];
    unsigned char *arena = NULL;  // pointer to the start of arena
    int size = 0;
    char *cmd;
    while (readCmd(&argc, args)) {
        cmd = args[0];
        if (strcmp(cmd, "INITIALIZE") == 0) {
            // &arena sends the address of pointer arena
            // &arena is used when the content is modified
            // size is modified in initialize in order to use its value here
            initialize(args, &arena, &size);
        } else if (strcmp(cmd, "FINALIZE") == 0) {
            // arena sends pointer to arena
            // arena is used when the content is not modified
            finalize(arena);
            // free command memory early and exit the program
            freeCmdMem(argc, args);
            return 0;
        } else if (strcmp(cmd, "DUMP") == 0) {
            dump(arena, size);
        } else if (strcmp(cmd, "ALLOC") == 0) {
            alloc(args, &arena, size);
        } else if (strcmp(cmd, "FREE") == 0) {
            freeExec(args, &arena, size);
        } else if (strcmp(cmd, "FILL") == 0) {
            fill(args, &arena);
        } else if (strcmp(cmd, "SHOW") == 0) {
            show(args, arena, size);
        }
        freeCmdMem(argc, args);
    }
    return 0;
}
/*
void printArena(unsigned char *arena, int size)
{
	int i;
	printf("\n######################### PRINT ARENA #########################\n\n");
	for(i = 0; i < size; i++) {
		if(!(i%16)) printf("\n");
		printf("%02d ", *(arena + i));
	}
	printf("\n\n");
	for(i = 0; i < size; i++) {
		if(!(i%10)) printf("\n");
		printf("%p ", (arena + i));
	}
	printf("\n\n######################### ARENA PRINTED #########################\n\n");
}
*/
void showFree(unsigned char *arena, int size) {
    int freeBlocks = 0, freeBytes = 0;
    int nextBlockStarts = *(int *)arena;
    int currentBlockStarts = 0;
    int currentBlockLen = sizeof(int);
    int dist = 0;
    // loop the arena while it didn't reach the end
    while (nextBlockStarts != 0) {
        dist = nextBlockStarts - (currentBlockStarts + currentBlockLen);
        currentBlockStarts = nextBlockStarts;
        nextBlockStarts = *(int *)(arena + nextBlockStarts);
        currentBlockLen = *(int *)(arena +
            currentBlockStarts + (2 * sizeof(int)));
        if (dist > 0) {
            freeBlocks++;
            freeBytes += dist;
        }
    }
    if (currentBlockStarts + currentBlockLen < size) {
        freeBlocks++;
        freeBytes += size - (currentBlockStarts + currentBlockLen);
    }
    printf("%d blocks (%d bytes) free\n", freeBlocks, freeBytes);
}
void showUsage(unsigned char *arena, int size) {
    int usedBlocks = 0, usedBytes = 0, freeBlocks = 0, freeBytes = 0;
    double eff = 0, fragm = 0;
    int reservedBytes = sizeof(int);
    int nextBlockStarts = *(int *)arena;
    int currentBlockStarts = 0;
    int currentBlockLen = sizeof(int);
    int dist = 0;
    // loop the arena while it didn't reach the end
    while (nextBlockStarts != 0) {
        dist = nextBlockStarts - (currentBlockStarts + currentBlockLen);
        currentBlockStarts = nextBlockStarts;
        nextBlockStarts = *(int *)(arena + nextBlockStarts);
        currentBlockLen = *(int *)(arena +
            currentBlockStarts + (2 * sizeof(int)));
        usedBlocks++;
        usedBytes += currentBlockLen - (3 * sizeof(int));
        reservedBytes += currentBlockLen;
        if (dist > 0) {
            freeBlocks++;
            freeBytes += dist;
        }
    }
    if (currentBlockStarts + currentBlockLen < size) {
        freeBlocks++;
        freeBytes += size - (currentBlockStarts + currentBlockLen);
    }
    eff = (double)usedBytes / reservedBytes * 100;
    fragm = (double)(freeBlocks - 1) / usedBlocks * 100;
    printf("%d blocks (%d bytes) used\n", usedBlocks, usedBytes);
    printf("%d%% efficiency\n", (int)eff);
    printf("%d%% fragmentation\n", (int)fragm);
}
void showAllocations(unsigned char *arena, int size) {
    int usedBlocks = 0, usedBytes = 0, freeBlocks = 0, freeBytes = 0;
    int nextBlockStarts = *(int *)arena;
    int currentBlockStarts = 0;
    int currentBlockLen = sizeof(int);
    int dist = 0;
    printf("OCCUPIED %d bytes\n", (int)sizeof(int));
    // loop the arena while it didn't reach the end
    while (nextBlockStarts != 0) {
        dist = nextBlockStarts - (currentBlockStarts + currentBlockLen);
        currentBlockStarts = nextBlockStarts;
        nextBlockStarts = *(int *)(arena + nextBlockStarts);
        currentBlockLen = *(int *)(arena +
            currentBlockStarts + (2 * sizeof(int)));
        usedBlocks++;
        usedBytes += currentBlockLen - (3 * sizeof(int));
        if (dist > 0) {
            printf("FREE %d bytes\n", dist);
        }
        printf("OCCUPIED %d bytes\n", currentBlockLen);
    }
    if (currentBlockStarts + currentBlockLen < size) {
        printf("FREE %d bytes\n", size -
            (currentBlockStarts + currentBlockLen));
    }
}
void show(char *args[MAX_ARGS], unsigned char *arena, int size) {
    // check which parameter was given to SHOW and call a function
    char *param = args[1];
    if (strcmp(param, "FREE") == 0) {
        showFree(arena, size);
    } else if (strcmp(param, "USAGE") == 0) {
        showUsage(arena, size);
    } else if (strcmp(param, "ALLOCATIONS") == 0) {
        showAllocations(arena, size);
    }
}
void fill(char *args[MAX_ARGS], unsigned char **arena) {
    int startIndex = atoi(args[1]);
    int size = atoi(args[2]);
    int value = atoi(args[3]);
    int i;
    // loop the given section and set the value
    for (i = startIndex; i < startIndex + size; i++) {
        *(*arena + i) = value;
    }
}
void freeExec(char *args[MAX_ARGS], unsigned char **arena, int size) {
    int startIndex = atoi(args[1]);
    int prevBlockStarts, nextBlockStarts;
    nextBlockStarts = *(int *)(*arena + startIndex - (3 * sizeof(int)));
    prevBlockStarts = *(int *)(*arena + startIndex - (2 * sizeof(int)));
    // set the new index for both the previous and the next block
    *(int *)(*arena + nextBlockStarts + (1 * sizeof(int))) = prevBlockStarts;
    *(int *)(*arena + prevBlockStarts) = nextBlockStarts;
}
void dump(unsigned char *arena, int size) {
    int i, j = 0;
    for (i = 0; i < size; i++, j++) {
        // print a new line only if index is not 0 and index divides by 16
        if (i % 16 == 0) {
            if (i != 0) {
                printf("\n");
            }
            printf("%08X\t", i);
            j = 0;
        }
        printf("%02X ", arena[i]);
        // j counts how many bytes were printed on the current line
        // and after the 7th one, a space is printed
        if (j == 7) {
            printf(" ");
        }
    }
    printf("\n%08X\n", i);
}
void alloc(char *args[MAX_ARGS], unsigned char **arena, int size) {
    int userAllocSize = atoi(args[1]);
    // allocSize is the user size + management size
    int allocSize = userAllocSize + (3 * sizeof(int));
    int spaceBetween = 0;
    // *arena is the pointer to the start of arena
    // arena is pointer to pointer
    // Cases (3):
    // 1. if the block is first to be added (there are no blocks)
    // 2. if the block is to be added in the middle
    // 3. if the block is last to be added
    if (!(*(int *)*arena)) {
        // check if there is enough room for alloc
        if (allocSize > size) {
            printf("0");
        } else {
            // case 1
            // init first index
            *(int *)(*arena + (0 * sizeof(int))) = sizeof(int);
            int firstBlockStarts = *(int *)*(arena + (0 * sizeof(int)));
            // next block starts
            *(int *)(*arena + firstBlockStarts + (0 * sizeof(int))) = 0;
            // previous block starts
            *(int *)(*arena + firstBlockStarts + (1 * sizeof(int))) = 0;
            // block length
            *(int *)(*arena + firstBlockStarts + (2 * sizeof(int))) = allocSize;

            printf("%d\n", (int)(firstBlockStarts + (3 * sizeof(int))));
        }
    } else {
        // case 2
        int nextBlockStarts = *(int *)*arena;
        int currentBlockStarts = 0;
        int currentBlockLen = sizeof(int);
        int dist = 0;

        // loop the arena while it didn't reach the end and a gap was not found
        dist = nextBlockStarts - (currentBlockStarts + currentBlockLen);
        while (nextBlockStarts != 0 && (dist < allocSize)) {
            currentBlockStarts = nextBlockStarts;
            nextBlockStarts = *(int *)(*arena + nextBlockStarts);
            currentBlockLen = *(int *)(*arena +
                currentBlockStarts + (2 * sizeof(int)));
            dist = nextBlockStarts - (currentBlockStarts + currentBlockLen);
        }
        if (dist >= allocSize) {
            spaceBetween = 1;

            // set start index OF the next
            *(int *)(*arena + currentBlockStarts +
                currentBlockLen + (0 * sizeof(int))) = nextBlockStarts;
            // set start index OF the prev
            *(int *)(*arena + currentBlockStarts +
                currentBlockLen + (1 * sizeof(int))) = currentBlockStarts;
            // set block len
            *(int *)(*arena + currentBlockStarts +
                currentBlockLen + (2 * sizeof(int))) = allocSize;
            // set start index IN the prev block
            *(int *)(*arena + currentBlockStarts +
                (0 * sizeof(int))) = currentBlockStarts + currentBlockLen;
            // set start index IN the next block
            *(int *)(*arena + nextBlockStarts +
                (1 * sizeof(int))) = currentBlockStarts + currentBlockLen;
            if (currentBlockStarts == 0) {
                currentBlockStarts = 4;
            }
            printf("%d\n", (int)(currentBlockStarts + (3 * sizeof(int))));
        }

        if (!spaceBetween) {
            // case 3
            int nextBlockStarts = *(int *)*arena;
            int currentBlockStarts = 0;
            int currentBlockLen = 0;

            while (nextBlockStarts != 0) {
                currentBlockStarts = nextBlockStarts;
                nextBlockStarts = *(int *)(*arena + nextBlockStarts);
            }
            currentBlockLen = *(int *)(*arena +
                currentBlockStarts + (2 * sizeof(int)));
            if (currentBlockStarts + currentBlockLen + allocSize > size) {
                printf("0\n");
            } else {
                // set start index IN the prev block
                *(int *)(*arena + currentBlockStarts +
                    (0 * sizeof(int))) = currentBlockStarts + currentBlockLen;

                // set start index OF the next
                *(int *)(*arena + currentBlockStarts +
                    currentBlockLen + (0 * sizeof(int))) = 0;
                // set start index OF the prev
                *(int *)(*arena + currentBlockStarts +
                    currentBlockLen + (1 * sizeof(int))) = currentBlockStarts;
                // set block len
                *(int *)(*arena + currentBlockStarts +
                    currentBlockLen + (2 * sizeof(int))) = allocSize;
                printf("%d\n", (int)(currentBlockStarts +
                    currentBlockLen + (3 * sizeof(int))));
            }
        }
    }
}
void finalize(unsigned char *arena) {
    // free the memory allocated at the beggining of the program
    free(arena);
}
void initialize(char *args[MAX_ARGS], unsigned char **arena, int *size) {
    // *arena is the pointer to the start of arena
    // arena is pointer to pointer (double pointer)
    *size = atoi(args[1]);
    *arena = (unsigned char *) calloc(*size, sizeof(unsigned char));
}
// read and parse a command
int readCmd(int *argc, char *args[MAX_ARGS]) {
    char line[MAX_BUFFER + 1], *token;
    const char *delim = " ";  // delimiter for params
    int len, count = 0;
    if (!fgets(line, MAX_BUFFER, stdin)) {
        return 0;  // fail
    }
    // remove extra \n
    len = strlen(line);
    if (line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    // split line in args (tokens)
    token = strtok(line, delim);
    while (token != NULL) {
        // alloc memory for current token
        len = strlen(token) + 1;  // +1 because we need '\0' at the end
        args[count] = (char *) malloc(len * sizeof(char));
        strcpy(args[count], token);
        // get the next token
        token = strtok(NULL, delim);
        count++;
    }
    *argc = count;
    return 1;  // success
}
// free memory used to store args
void freeCmdMem(int argc, char *args[MAX_ARGS]) {
    int i;
    for (i = 0; i < argc; i++) {
        free(args[i]);
    }
}
