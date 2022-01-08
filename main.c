#include <stdio.h>
#include <math.h>

#define LEN(array) (sizeof(array)/sizeof(array[0]))

void Draw(char playerName[], const char value_space[], const int value_space_len) {
    const int squareRoot = (int) sqrt(value_space_len);
    char fieldValues[squareRoot * squareRoot][squareRoot * squareRoot];
    // fill fieldValues with crap
    for (int a = 0; a < LEN(fieldValues); a++) {
        for (int b = 0; b < LEN(fieldValues); b++) {
            fieldValues[a][b] = '*';
        }
    }

    const int frameSize = 2 + squareRoot * squareRoot + squareRoot;
    char frame[frameSize][frameSize];
    const char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                               'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                               's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    // draw lines and fieldValues
    for (int x = 0; x < frameSize; x += 1) {
        for (int y = 0; y < frameSize; y += 1) {
            if (x % (squareRoot + 1) == 1 && y % (squareRoot + 1) == 1) {
                frame[x][y] = ' ';
            } else {
                if (x % (squareRoot + 1) == 1 && y != 0) {
                    frame[x][y] = '-';
                } else {
                    if (y % (squareRoot + 1) == 1 && x != 0) {
                        frame[x][y] = '|';
                    } else {
                        // insert fieldValues
                        if(x != 0 && y != 0) {
                            frame[x][y] = fieldValues[x - 2 - ((x - 2) / (squareRoot + 1))][y - 2 - ((y - 2) / (squareRoot + 1))];
                        }
                        else {
                            frame[x][y] = ' ';
                        }

                    }
                }
            }
        }
    }

    // draw scale
    int counter = 0;
    for (int i = 1; i < frameSize; i++) {
        if (i % 4 == 1) {
            continue;
        }
        frame[0][i] = alphabet[counter];
        frame[i][0] = counter + 1 + '0';

        counter++;
    }



    // print frame
    printf("%s\n", playerName);
    for (int i = 0; i < frameSize; i++) {
        for (int j = 0; j < frameSize; j++) {
            printf("%c", frame[i][j]);
        }
        printf(" \n");
    }
}

int main() {
    const char values[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; // length must be a square number

    if (sizeof(values) == 0) {
        printf("[ERROR]: values cannot be empty");
        return 1;
    }
    if (floor(sqrt(LEN(values))) != ceil(sqrt(LEN(values)))) {
        printf("[ERROR]: length of values must be a square number");
        return 1;
    }

    printf("Enter your name: ");
    char name[15];
    scanf("%15s", name);
    Draw(name, values, LEN(values));
    return 0;
}


