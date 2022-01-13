#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define LEN(array) (sizeof(array)/sizeof(array[0]))

void
Draw(char playerName[], const int squareRoot, const char xScale[], int xScaleLen, const char yScale[], int yScaleLen,
     char fieldValues[]) {


    const int frameSize = 2 + squareRoot * squareRoot + squareRoot;
    char frame[frameSize][frameSize];


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
                        if (x != 0 && y != 0) {
                            frame[x][y] = fieldValues[(x - 2 - ((x - 2) / (squareRoot + 1))) * squareRoot * squareRoot +
                                                      (y - 2 - ((y - 2) / (squareRoot + 1)))];
                        } else {
                            frame[x][y] = ' ';
                        }

                    }
                }
            }
        }
    }

    // draw scale
    for (int i = 1; i < frameSize; i++) {
        if (i % (squareRoot + 1) == 1) {
            continue;
        }
        frame[0][i] = xScale[i - 2 - ((i - 2) / (squareRoot + 1))];
        frame[i][0] = yScale[i - 2 - ((i - 2) / (squareRoot + 1))];
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

int FindIndex(char a[], int size, char value) {
    int index = 0;
    while (index < size && a[index] != value) ++index;
    return (index == size ? -1 : index);
}

void setFieldValue(char field[], int fieldLen, char x, char y, char val) {
    printf("acolumn: %c, arow: %c, avalue: %c \n\n", x, y, val);
    field[fieldLen * x + y] = val;
}

int main() {
    // length of values must be a square number
    const char values[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                           'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    const int valuesLen = 36; // LEN(values);
    if (sizeof(values) == 0) {
        printf("[ERROR]: values cannot be empty");
        return 1;
    }
    if (valuesLen > 36) {
        printf("[ERROR]: more than 36 values, sudokus with more than 36 values would require a major rework,"
               " so they are currently not supported");
    }
    if (floor(sqrt(LEN(values))) != ceil(sqrt(LEN(values)))) {
        printf("[ERROR]: length of values must be a square number");
        return 1;
    }

    //generate scales
    const char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                               'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                               's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char xScale[valuesLen];
    char yScale[valuesLen];
    for (int i = 0; i < valuesLen; i++) {
        //fill xScale
        if (i < LEN(alphabet)) {
            xScale[i] = toupper(alphabet[i]);
        } else {
            xScale[i] = (char) (i - LEN(alphabet) + '0');
        }

        //fill yScale
        if (i < 9) {
            yScale[i] = (char) ((i + 1) + '0');
        } else {
            if(i == 9) {
                yScale[i] = '0';
            } else {
                yScale[i] = toupper(alphabet[i - 10]);
            }
        }

    }

    const int squareRoot = (int) sqrt(valuesLen);
    char fieldValues[squareRoot * squareRoot * squareRoot * squareRoot];
    // fill fieldValues with stars
    for (int a = 0; a < squareRoot * squareRoot; a++) {
        for (int b = 0; b < squareRoot * squareRoot; b++) {
            fieldValues[a * squareRoot * squareRoot + b] = '*';
        }
    }

    // get player name
    printf("Enter your name: ");
    char name[15];
    scanf("%15s", name);

    int gameFinished = 0;
    while (gameFinished == 0) {

        Draw(name,
             squareRoot,
             xScale, LEN(xScale),
             yScale, LEN(yScale),
             fieldValues);

        //clear user input buffer
        char c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        //get user input
        char inputX;
        char inputY;
        char inputVal;
        int validInput = 0;
        while (validInput == 0) {
            printf("Enter your next move in the format: column row value\n");
            scanf("%1c %1c %1c", &inputX, &inputY, &inputVal);
            if (FindIndex(xScale, LEN(xScale), inputX) == -1) {
                printf("column value '%c' is invalid\n", inputX);
            } else {
                if (FindIndex(yScale, LEN(yScale), inputY) == -1) {
                    printf("row value '%c' is invalid\n", inputY);
                } else {
                    if (FindIndex(values, valuesLen, inputVal) == -1) {
                        printf("value value '%c' is invalid\n", inputVal);
                    } else {
                        validInput = 1;
                    }
                }
            }

        }
        printf("column: %c, row: %c, value: %c \n\n", inputX, inputY, inputVal);
        setFieldValue(fieldValues,
                      LEN(fieldValues),
                      FindIndex(xScale, LEN(xScale), inputX),
                      FindIndex(yScale, LEN(yScale), inputY),
                      inputVal);
    }
    return 0;
}


