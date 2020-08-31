//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "Graphics2D.h"



#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>

#include <cmath>

Graphics2D engine = Graphics2D(540, 540, "Digit Recognition", false);
double tiles[28][28];
double tileWidth = (double)2 / 28;

using namespace std;

int nodes[4] = { 784, 16, 16, 10 }; //the number of nodes for each layer - nodes[0] = input, nodes[3] = output
vector<vector<vector<double>>> weights; //dimensions represent matrix number, matrix row, matrix column
vector<vector<double>> bias; //dimensions represent layer, node number
vector<vector<double>> contents; //contents of all nodes (between 0 and 1 inclusive)
vector<vector<double>> zValues; //contents of all nodes, however has not been input into the sigmoid function THERE ARE NO Z VALUES FOR THE FIRST LAYER!!!

double expx(double x) {
    double result = 1;
    double currentVal = 1;
    for (int i = 1; i <= 100; i++) {
        currentVal *= (double)x / i;
        result += currentVal;
    }
    return result;
}

double sigmoid(double x) { //returns a value between 0 and 1 given any real x
    return (double)1 / (1 + expx(-x));
}

double sigmoidDerivative(double x) {
    double ex = expx(-x);
    double denominator = ex + 1;
    denominator *= denominator;

    return (double)ex / denominator;
}

void createNewNetwork() {
    //srand(time(0));

    vector<vector<double>> nextMat;
    vector<double> nextRow;
    int weightSize = 0;
    for (int i = 0; i < 3; i++) { //initialises weights with random values between -1 and 1 inclusive
        for (int rows = 0; rows < nodes[i + 1]; rows++) {
            for (int columns = 0; columns < nodes[i]; columns++) {
                nextRow.push_back(((double)-123 / RAND_MAX) - 1);
                weightSize += 1;
            }
            nextMat.push_back(nextRow);
            nextRow.clear();
        }

        weights.push_back(nextMat);
        //weightGrads.push_back(nextMat); //ONLY USED FOR TRAINING
        nextMat.clear();
    }

    for (int i = 1; i < 4; i++) { //initialises biases with zero values
        for (int x = 0; x < nodes[i]; x++) {
            nextRow.push_back(0);
        }
        bias.push_back(nextRow);
        //biasGrads.push_back(nextRow); //ONLY USED FOR TRAINING
        nextRow.clear();
    }




    //READS VALUES FROM FILE
    ifstream file;
    file.open("variables.txt");

    if (!file) {
        cout << "Could not open file!" << endl;
        exit(1);
    }

    double current;
    cout << "Neural Network data found!" << endl;
    int a = 0, b = 0, c = 0; //indexes of values
    while (file >> current) {
        if (c != -1) { //current is a weight
            if (c >= weights[a][b].size()) {
                c = 0;
                b += 1;
            }
            if (b >= weights[a].size()) {
                b = 0;
                a += 1;
            }
            if (a >= weights.size()) {
                c = -1;
                a = 0;
                b = 1;
                bias[0][0] = current;
            }
            else {
                weights[a][b][c] = current;
                c += 1;
            }
        }
        else { //current is a bias
            if (b >= bias[a].size()) {
                b = 0;
                a += 1;
            }
            bias[a][b] = current;
            b += 1;
        }

    }
    file.close();
    //END OF FILE READING







    for (int i = 0; i < 4; i++) { //initialises contents with zero values
        for (int x = 0; x < nodes[i]; x++) {
            nextRow.push_back(0);
        }
        if (i != 0) {
            zValues.push_back(nextRow);
        }
        contents.push_back(nextRow);
        nextRow.clear();
    }
}

void generateOutputValues() {
    double currentVal;
    for (int i = 0; i < 3; i++) { //for each node multiply the relevant weight matrix by the given node contents
        //the folloiwing multiplies weights[i] with contents[i]
        for (int a = 0; a < nodes[i + 1]; a++) {
            currentVal = 0;
            for (int b = 0; b < nodes[i]; b++) {
                currentVal += weights[i][a][b] * contents[i][b];
            }
            zValues[i][a] = currentVal + bias[i][a];
            contents[i + 1][a] = sigmoid(currentVal + bias[i][a]);
        }
    }
}

int getOutput() { //returns the number which the neural network "sees"
    double maxVal = -1;
    int result = -1;
    for (int i = 0; i < nodes[3]; i++) {
        if (contents[3][i] > maxVal) {
            maxVal = contents[3][i];
            result = i;
        }
    }
    return result;
}





















int input() {
	if (engine.keyPress(GLFW_KEY_ESCAPE)) {
		engine.closeWindow();
	}
	if (engine.keyPress('C')) {
		return 2;
	}
	if (engine.mouseButtonPress(GLFW_MOUSE_BUTTON_1)) {
		return 1;
	}
	if (engine.mouseButtonPress(GLFW_MOUSE_BUTTON_2)) {
		return 0;
	}
	return -1;
}

void updateTiles(int colour) {
	double xPos, yPos;
	engine.getMousePos(&xPos, &yPos);
	int xInd = (14 * (xPos + 1));
	int yInd = 28 - (14 * (yPos + 1));
    for (int i = yInd - 1; i <= yInd + 1; i++) {
        if (i >= 0 && i < 28) {
            for (int x = xInd - 1; x <= xInd + 1; x++) {
                if (x >= 0 && x < 28) {
                    tiles[i][x] = colour;
                }
            }
        }
    }
}

void clearTiles() {
	for (int i = 0; i < 28; i++) {
		for (int x = 0; x < 28; x++) {
			tiles[i][x] = 0;
		}
	}
}


void renderTiles() {
	for (int y = 0; y < 28; y++) { //starts in top left corner
		for (int x = 0; x < 28; x++) {
			engine.setFillColour(tiles[y][x], tiles[y][x], tiles[y][x], 1);
			engine.rect((x * tileWidth) - 1, 1 - ((y + 1) * tileWidth), tileWidth, tileWidth);
		}
	}
}

int readDigit() { //reads currently drawn digit
    for (int i = 0; i < 28; i++) {
        for (int x = 0; x < 28; x++) {
            contents[0][(i * 28) + x] = tiles[i][x];
        }
    }
    generateOutputValues();
    return getOutput();
}

//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)
int main() {
    createNewNetwork();

	int currentColour;
	while (engine.shouldClose() == false) {
		currentColour = input();
		if (currentColour == 2) {
			clearTiles();
		}
		else if (currentColour != -1) {
			updateTiles(currentColour);
		}

		renderTiles();

        
        cout << readDigit() << endl;


		engine.clear();
	}
}