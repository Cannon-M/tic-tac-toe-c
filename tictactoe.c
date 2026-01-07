// Tic-Tac-Toe Game (Week 10: Basic Operation)
// Author: Mathieu Cannon
// Date: Week 10, 2025
// Description: Two-player Tic-Tac-Toe with customizable grid size.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char **createBoard(int size) {
    char **board = malloc(size * sizeof(char *));
    if (!board) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        board[i] = malloc(size * sizeof(char));
        if (!board[i]) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }

    return board;
}

void freeBoard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

void initializeBoard(char **board, int size);
void printBoard(char **board, int size);
int checkWin(char **board, int size, char player);
int checkDraw(char **board, int size);
void playerMove(char **board, int size, char player);
int aiMove(char **board, int size, char ai, char human);
int canWinNext(char **board, int size, char player, int *r, int *c);

// Main
int main() {
    char **board;
    int size;
    char currentPlayer = 'X';
    int gamemode = 0;

    printf("=== TIC-TAC-TOE GAME ===\n");

    do {
        printf("Gamemode:\n 1. Player vs Player\n 2. Player vs AI\n Enter Gamemode: ");
        scanf("%d", &gamemode);

        printf("Enter board size (3 to 15): ");
        scanf("%d", &size);

    } while (size < 3 || size > 15);

    board = createBoard(size);
    initializeBoard(board, size);

    if (gamemode == 1) {  
        while (1) {
            printBoard(board, size);
            printf("Player %c's turn.\n", currentPlayer);
            playerMove(board, size, currentPlayer);

            if (checkWin(board, size, currentPlayer)) {
                printBoard(board, size);
                printf("Player %c wins!\n", currentPlayer);
                break;
            }
            if (checkDraw(board, size)) {
                printBoard(board, size);
                printf("It's a draw!\n");
                break;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

    } else {

        char human = 'X';
        char ai = 'O';

        while (1) {
            printBoard(board, size);
            printf("Your turn.\n");
            playerMove(board, size, human);

            if (checkWin(board, size, human)) {
                printBoard(board, size);
                printf("You win!\n");
                break;
            }
            if (checkDraw(board, size)) {
                printBoard(board, size);
                printf("It's a draw!\n");
                break;
            }

            printf("AI is thinking...\n");
            aiMove(board, size, ai, human);

            if (checkWin(board, size, ai)) {
                printBoard(board, size);
                printf("AI wins!\n");
                break;
            }
            if (checkDraw(board, size)) {
                printBoard(board, size);
                printf("It's a draw!\n");
                break;
            }
        }
    }

    freeBoard(board, size);
    return 0;
}

void initializeBoard(char **board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = ' ';
}

void printBoard(char **board, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");

        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

void playerMove(char **board, int size, char player) {
    int row, col;
    while (1) {
        printf("Enter row and column (1-%d): ", size);
        scanf("%d %d", &row, &col);

        if (row < 1 || row > size || col < 1 || col > size) {
            printf("Invalid position. Try again.\n");
        } else if (board[row - 1][col - 1] != ' ') {
            printf("Cell already taken. Try again.\n");
        } else {
            board[row - 1][col - 1] = player;
            break;
        }
    }
}

int checkWin(char **board, int size, char player) {
    int win;

    // Rows
    for (int i = 0; i < size; i++) {
        win = 1;
        for (int j = 0; j < size; j++)
            if (board[i][j] != player) win = 0;

        if (win) return 1;
    }

    // Columns
    for (int j = 0; j < size; j++) {
        win = 1;
        for (int i = 0; i < size; i++)
            if (board[i][j] != player) win = 0;

        if (win) return 1;
    }

    // Main diagonal
    win = 1;
    for (int i = 0; i < size; i++)
        if (board[i][i] != player) win = 0;
    if (win) return 1;

    // Anti-diagonal
    win = 1;
    for (int i = 0; i < size; i++)
        if (board[i][size - i - 1] != player) win = 0;
    if (win) return 1;

    return 0;
}

int checkDraw(char **board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ') return 0;

    return 1;
}

int canWinNext(char **board, int size, char player, int *r, int *c) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;

                if (checkWin(board, size, player)) {
                    board[i][j] = ' ';
                    *r = i;
                    *c = j;
                    return 1;
                }

                board[i][j] = ' ';
            }
        }
    }
    return 0;
}

int aiMove(char **board, int size, char ai, char human) {
    int r, c;

    // 1. Try to win
    if (canWinNext(board, size, ai, &r, &c)) {
        board[r][c] = ai;
        return 1;
    }

    // 2. Block human
    if (canWinNext(board, size, human, &r, &c)) {
        board[r][c] = ai;
        return 1;
    }

    // 3. Random move
    srand(time(NULL));
    while (1) {
        r = rand() % size;
        c = rand() % size;

        if (board[r][c] == ' ') {
            board[r][c] = ai;
            return 1;
        }
    }
}