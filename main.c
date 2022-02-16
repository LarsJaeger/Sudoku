#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "stack/stack.h"

#define LEN(array) (sizeof(array)/sizeof(array[0]))

typedef struct {
    int x;
    int y;
    char value;
} PreviousValue;

void UndoLastFieldValue(char field[], int fieldLen, myStack_t *moves);

void FillFieldsRandomly(char fieldValues[], int squareRoot, myStack_t *moves, const char values[]);

void Draw(char playerName[], const int squareRoot, const char xScale[], int xScaleLen, const char yScale[],
          int yScaleLen, char fieldValues[]);

int FindIndex(const char a[], int size, char value);

int SetFieldValue(char field[], int fieldLen, int x, int y, char val, myStack_t *moves);

int PlayerSetFieldValue(char field[], char startValues[], int fieldLen, int x, int y, char val, myStack_t *moves);

int RekursivFill(char field[], const char values[], int line, int row, int squareRoot, myStack_t *moves);

void HideRandomly(char field[], int fieldLen);

void CopyArray(char i_array[], int i_array_size, char o_array[], int o_array_size);

int main() {
    srand(time(NULL));

    // length of values must be a square number
    // values must not contain '*'
    const char values[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                           'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


    printf("Enter field size (must be a square number and lower than 36): ");
    int valuesLen; // LEN(values);
    scanf("%i", &valuesLen);
    if (valuesLen == 0) {
        printf("[ERROR]: field size cannot be 0");
        return 1;
    }
    if (valuesLen > 36) {
        printf("[ERROR]: you cannot choose more a size bigger than 36, sudokus with more than 36 values would require a major rework,"
               " so they are currently not supported");
    }
    if (floor(sqrt(valuesLen)) != ceil(sqrt(valuesLen))) {
        printf("[ERROR]: field size must be a square number");
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
            if (i == 9) {
                yScale[i] = '0';
            } else {
                yScale[i] = toupper(alphabet[i - 10]);
            }
        }

    }

    //create stack for past moves
    myStack_t *moves;
    moves = StackNew(sizeof(PreviousValue), valuesLen * valuesLen * valuesLen * valuesLen * valuesLen);

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

    //generate start values
    FillFieldsRandomly(fieldValues, squareRoot, moves, values);
    HideRandomly(fieldValues, squareRoot * squareRoot * squareRoot * squareRoot);
    char startValues[LEN(fieldValues)];
    CopyArray(fieldValues, LEN(fieldValues), startValues, LEN(startValues));

    int gameFinished = 0;
    while (gameFinished == 0) {


        Draw(name,
             squareRoot,
             xScale, LEN(xScale),
             yScale, LEN(yScale),
             fieldValues);

        //get user input
        char inputX;
        char inputY;
        char inputVal;
        int validInput = 0;
        while (validInput == 0) {
            printf("Enter 'UNDO' to revoke your last move, or your next move in the format: column row value\n");
            //clear user input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}

            scanf("%1c %1c %1c", &inputX, &inputY, &inputVal);

            if (inputX == 'U' && inputY == 'N' && inputVal == 'D') {
                char undo;
                scanf("%1c", &undo);
                if (undo == 'O') {
                    printf("Your last move will be revoked.\n");
                    validInput = 1;
                    UndoLastFieldValue(fieldValues, squareRoot * squareRoot, moves);
                }
            } else {
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
        }
        PlayerSetFieldValue(fieldValues,
                      startValues,
                      squareRoot * squareRoot,
                      FindIndex(xScale, LEN(xScale), inputX),
                      FindIndex(yScale, LEN(yScale), inputY),
                      inputVal, moves);
        const int fieldLen = LEN(fieldValues);
        gameFinished = 1;
        for (int i = 0; i < fieldLen; i++) {
            if (fieldValues[i] == '*') {
                gameFinished = 0;
                break;
            }
        }
    }
    return 0;
}


void FillFieldsRandomly(char fieldValues[], int squareRoot, myStack_t *moves, const char values[]) {
    // fill diagonal fields
    for (int diag_quadrant = 0; diag_quadrant < squareRoot; diag_quadrant++) {
        const int xMinSubfield = diag_quadrant * squareRoot;
        const int xMaxSubfield = (diag_quadrant + 1) * squareRoot;
        const int yMinSubfield = diag_quadrant * squareRoot;
        const int yMaxSubfield = (diag_quadrant + 1) * squareRoot;
        for (int yTest = yMinSubfield; yTest < yMaxSubfield; yTest++) {
            for (int xTest = xMinSubfield; xTest < xMaxSubfield; xTest++) {
                const int start_val = rand() % (squareRoot * squareRoot);
                for (int i = start_val; i < start_val + (squareRoot * squareRoot); i++) {
                    if (SetFieldValue(fieldValues, squareRoot * squareRoot, xTest, yTest,
                                      values[i % (squareRoot * squareRoot)], moves) == 1) {
                        break;
                    }
                }
            }
        }
    }
    RekursivFill(fieldValues, values, 0, 0, squareRoot, moves);
}

void UndoLastFieldValue(char *field, int fieldLen, myStack_t *moves) {
    PreviousValue previousValue;
    Pop(moves, &previousValue);
    int testX = previousValue.x;
    int testY = previousValue.y;
    char testValue = previousValue.value;
    field[fieldLen * previousValue.y + previousValue.x] = previousValue.value;
}

void Draw(char *playerName, const int squareRoot, const char *xScale, int xScaleLen, const char *yScale, int yScaleLen,
          char *fieldValues) {

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
    printf("\n%s\n", playerName);
    for (int i = 0; i < frameSize; i++) {
        for (int j = 0; j < frameSize; j++) {
            printf("%c", frame[i][j]);
        }
        printf(" \n");
    }
}

int FindIndex(const char *a, int size, char value) {
    int index = 0;
    while (index < size && a[index] != value) ++index;
    return (index == size ? -1 : index);
}

int SetFieldValue(char *field, int fieldLen, int x, int y, char val, myStack_t *moves) {
    for (int yTest = 0; yTest < fieldLen; yTest++) {
        if (field[fieldLen * yTest + x] == val) {
            return 0;
        }
    }
    for (int xTest = 0; xTest < fieldLen; xTest++) {
        if (field[fieldLen * y + xTest] == val) {
            return 0;
        }
    }
    const int squareRoot = (int) sqrt(fieldLen);
    const int xMinSubfield = (x / squareRoot) * squareRoot;
    const int xMaxSubfield = ((x / squareRoot) + 1) * squareRoot;
    const int yMinSubfield = (y / squareRoot) * squareRoot;
    const int yMaxSubfield = ((y / squareRoot) + 1) * squareRoot;
    // check
    for (int yTest = yMinSubfield; yTest < yMaxSubfield; yTest++) {
        const int xIndex = fieldLen * yTest;
        for (int xTest = xMinSubfield; xTest < xMaxSubfield; xTest++) {
            if (field[xIndex + xTest] == val) {
                return 0;
            }
        }
    }

    PreviousValue previousValue;
    previousValue.x = x;
    previousValue.y = y;
    previousValue.value = field[fieldLen * y + x];

    field[fieldLen * y + x] = val;
    Push(moves, &previousValue);
    return 1;
}

int RekursivFill(char *field, const char *values, int line, int row, int squareRoot, myStack_t *moves) {
    if (row >= (squareRoot * squareRoot) && line >= (squareRoot * squareRoot))
        return 1;


    if (row >= (squareRoot * squareRoot)) {
        if (line < (squareRoot * squareRoot) - 1) {
            line++;
            row = 0;
        }
    }

    if (line >= (squareRoot * squareRoot) - squareRoot && row >= (squareRoot * squareRoot) - squareRoot) {
        line++;
        row = 0;
        if (line >= (squareRoot * squareRoot)) {
            return 1;
        }

    }

    if (squareRoot > line < (squareRoot * squareRoot) - squareRoot && row == (int) (line / squareRoot) * squareRoot)
        row = row + squareRoot;

    if (line < squareRoot && row < squareRoot)
        row = squareRoot;


    const int start_val = rand() % (squareRoot * squareRoot);
    for (int testZahl = start_val; testZahl < start_val + (squareRoot * squareRoot); testZahl++) {
        if (SetFieldValue(field, squareRoot * squareRoot, row, line, values[testZahl % (squareRoot * squareRoot)],
                          moves) == 1) {
            if (RekursivFill(field, values, line, row + 1, squareRoot, moves) == 1) {
                return 1;
            }
            UndoLastFieldValue(field, squareRoot * squareRoot, moves);

        }
    }
    return 0;
}

void HideRandomly(char field[], int fieldLen) {
    for (int i = 0; i < fieldLen - 17; i++) {
        int fieldSet = 0;
        while (fieldSet == 0) {
            int index = rand() % fieldLen;
            if (field[index] != '*') {
                field[index] = '*';
                fieldSet = 1;
            }
        }
    }
}

void CopyArray(char i_array[], int i_array_size, char o_array[], int o_array_size){
    if(i_array_size > o_array_size) {
        printf("[ERROR]: o_array is too small for i_array");
        return;
    }
    for (int i = 0; i < i_array_size; i++) {
        o_array[i] = i_array[i];
    }
}

int PlayerSetFieldValue(char field[], char startValues[], int fieldLen, int x, int y, char val, myStack_t *moves) {
    if(startValues[fieldLen * y + x] != '*') {
        printf("This field cannot be set as it is part of the start values!");
        return 0;
    } else {
        SetFieldValue(field, fieldLen, x, y, val, moves);
    }
}