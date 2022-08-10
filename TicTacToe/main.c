#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief This program is a simple Tic Tac Toe game that can be played against another person
 * or the computer. A record of wins/ties will be kept in the record.txt file that will be diplayed
 * at the end of the game
 * 
 * @author Chris Pellerito
 */


// Blueprints
void turn(char*, int);
char is_winner(char*);
void end_game(int);
void print_board(char*);

int main() {
    printf("Welcome to TicTacToe!\n");
    printf("***Instructions***\nThe squares on the board are labeled 1 - 9 as such:\n");
    printf("1|2|3\n4|5|6\n7|8|9\n");
    printf("When it is your turn to make a move, you will be prompted for the number you wish to choose.\n");
    printf("This game can either be played against another person or the computer\n");

    // Get the number of players
    int players;
    printf("How many players are playing? 1 or 2: ");
    while (1) {
        scanf("%d", &players);
        if (players == 1 || players == 2)
            break;
        else
            printf("\nThere must be 1 or 2 players, try again: ");
    }
    // 0 for computer 2 for player 2
    int player = (players == 1)? 0 : 2;

    // Initialize board and random generator
    char *board = calloc(9, sizeof(char));
    srand(time(NULL));

    for (int i = 0; i < 6; i++) {
        // Player 1 is always a user so this can be hard coded
        turn(board, 1);
        // check if there is a winner
        char winner = is_winner(board);
        if (winner != '\0')
            end_game(1);

        // If i is 5 the game is over after player 1s turn
        if (i == 5)
            break;

        // Player 2s turn
        turn(board, player);
        // Check if there is a winner and if it is the computer or player 2
        winner = is_winner(board);
        if (winner != '\0' && player == 0)
            end_game(3);
        else if (winner != '\0' && player == 2)
            end_game(2);
    }

    // End the game
    end_game(3);

    return 0;
}


/**
 * @brief This function asks for the users move, validates that it is a valid move and 
 * then makes the move. If it is the computers turn it makes the move for the computer.
 * 
 * @param board pointer to the board
 * @param player the player whose turn it is, 1 for player 1, 2 for player 2, and 0 for the computer
 */
void turn(char *board, int player) {
    int move;
    // If the player is a user ask for input
    if (player > 0) {
        printf("Player %d, your turn\n", player);
        // Validate the users input
        while (1) {
            printf("make your move: ");
            scanf("%d", &move);

            // Between 1 and 9 and does not already contain an 'X' or 'O'
            if (move > 0 && move < 10) {
                if (board[move-1] != 'X' && board[move-1] != 'O')
                    break;
            }

            printf("ERROR: invalid move, try again\n");
        }
    } else {
        // Auto-pick
        move = (rand() % 9) + 1;

        while (board[move-1] != '\0')
            move = (rand() % 9) + 1;

        printf("Comp move: %d\n", move);
    }

    // Make the validated move
    char symbol = (player == 1)? 'X' : 'O';
    board[move-1] = symbol;

    print_board(board);
}


/**
 * @brief This function checks if there is a winner
 * 
 * @param board pointer to the board
 * @return char 'X' or 'O' depending on the winner and null('\0') if no winner
 */
char is_winner(char *board) { 
    int index = 0;
    // Check for horizontal wins
    while (index <= 6) { 
        if (board[index] == board[index + 1] && board[index] == board[index + 2])
            return board[index];
        index += 3;
    }

    // Check for vertical wins
    index = 0;
    while (index <= 3) { 
        if (board[index] == board[index + 3] && board[index] == board[index + 6])
            return board[index];
        index++;
    }

    // Check for diagonal wins
    if (board[0] == board[4] && board[0] == board[8])
        return board[0];
    if (board[2] == board[4] && board[2] == board[6])
        return board[2];
    
    // If no winner return null character
    return '\0';
}


/**
 * @brief This function is called when the game is over. It keeps track of how many times
 * someone has won and the number of ties.
 * 
 * @param winner 1 for player 1, 2 for player 2, 3 for computer and 4 for tie
 */
void end_game(int winner) {
    FILE *fpointer = fopen("record.txt", "r");
    // If the file doesn't exist, create it and fill with base data
    if (fpointer == NULL) {
        fclose(fpointer);
        fpointer = fopen("record.txt", "w");
        fprintf(fpointer, "Player1: 0\nPlayer2: 0\nComputer: 0\nTies: 0");
        fclose(fpointer);
    }
    // Read the values and store them as ints
    char line[11];
    fgets(line, 11, fpointer);
    int player1 = atoi(&line[9]);

    fgets(line, 11, fpointer);
    int player2 = atoi(&line[9]);

    fgets(line, 11, fpointer);
    int computer = atoi(&line[9]);

    fgets(line, 11, fpointer);
    int ties = atoi(&line[9]);

    fclose(fpointer);

    switch(winner) {
        case 1:
            player1++;
            printf("Congrats Player 1! You win!\n");
            break;
        case 2:
            player2++;
            printf("Congrats Player 2! You win!\n");
            break;
        case 3:
            computer++;
            printf("The Computer wins! Better luck next time!\n");
            break;
        case 0:
            ties++;
            printf("Congrats! You tied like every Tic Tac Toe game should!");
            break;
    }

    // Print the record to command line
    printf("***RECORD***\n");
    printf("Player1: %d\nPlayer2: %d\nComputer: %d\nTies: %d", player1, player2, computer, ties);

    // Then save the record in the record.txt file
    fpointer = fopen("record.txt", "w");
    fprintf(fpointer, "Player1: %d\nPlayer2: %d\nComputer: %d\nTies: %d", player1, player2, computer, ties);
    fclose(fpointer);

    // Exit program
    exit(0);
}


// Prints the board
void print_board(char *board){
    for (int i = 0; i < 9; i++) {
        // If one of the middle indexes, print the lines
        if (i == 1 || i == 4 || i == 7) {
            if (board[i] != '\0')
                printf(" | %c | ", board[i]);
            else
                printf(" |   | ");
        } else {
            // If not one of the middle indexes just print the character
            if (board[i] != '\0')
                printf("%c", board[i]);
            else
                printf(" ");
        }

        // Print newline
        if (i == 2 || i == 5 || i == 8)
            printf("\n");
    }
}