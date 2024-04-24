/*Defines all the functions in for the Functions class, both the ones required by the assignments
 * and several helper functions (make_matrix, add_matrix, subtract_matrix, partition, place)
 * unfortunately strassens algorithm is not working correctly, more information on line 100
*/
#include "Functions.h"

//meant to be used with getline as the input string, but any string should work
//returns a 1d double array representing a size x size matrix from the data string
double* Functions::make_matrix(int size, std::string data) {
	int size2 = size * size;
	int m = 0;
	double* matrix = new double[size2];
	std::string value = "";

	//for the size of data, push each non [space] character into value
	//if character = [space], make value into double and put into matrix
	for (int i = 0; i <= data.size(); i++) {

		if (data[i] != ' ' && data[i] != '\n' && data[i] != '\0') {//push characters into value
			value.push_back(data[i]);
		}
		else {//make into double and put into matrix[m], iterate m, make value empty
			matrix[m] = std::stod(value);
			m++;
			//std::cout << value << '\n';
			value = "";
		}
	}

	//if data did not make a full size x size matrix for some reason, padd rest with -1s
	if (m < size2) {
		while (m < size2) {
			matrix[m] = -1;
			m++;
		}
	}

	return matrix;
}

//print the input matrix to the console in column-major format
void Functions::print_in_2d(int size, double* matrix) {
	//for each row
	for (int row = 0; row < size; row++) {
		//get and print the row-th item for that column
		for (int col = 0; col < size; col++) {
			double d = matrix[(row * size) + col];
			std::cout << d << ' ';
		}
		std::cout << '\n';
	}
}

//simple iterative matrix multiplication
//verified correct
double* Functions::iterative_mult(int size, double* a, double* b) {
	int size2 = size * size;
	double sum = 0;
	double* result = new double[size2];

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			for (int k = 0; k < size; k++) {
				sum += a[(row * size) + k] * b[(k * size) + col];
			}
			result[(row * size) + col] = sum;
			sum = 0;
		}
	}

	return result;
}

//recursive matrix multiply
//giving this NxN matricies where N is not a power of 2 may cause problems
double* Functions::recursive_mult(int size, double* a, double* b) {

	int size2 = size * size;
	int half = size / 2;
	double* result = new double[size2];

	if (size == 1) {
		result[0] = a[0] * b[0];
	}
	else {
		//top left		0,0
		place(size, 0, 0, result, (add_matrix(half, 0, 0, recursive_mult(half, partition(half, 0, 0, a), partition(half, 0, 0, b)), recursive_mult(half, partition(half, 0, half, a), partition(half, half, 0, b)))));
		//top right		0,size/2
		place(size, 0, half, result, (add_matrix(half, 0, 0, recursive_mult(half, partition(half, 0, 0, a), partition(half, 0, half, b)), recursive_mult(half, partition(half, 0, half, a), partition(half, half, half, b)))));
		//bottom left	size/2,0
		place(size, half, 0, result, (add_matrix(half, 0, 0, recursive_mult(half, partition(half, half, 0, a), partition(half, 0, 0, b)), recursive_mult(half, partition(half, half, half, a), partition(half, half, 0, b)))));
		//bottom right	size/2,size/2
		place(size, half, half, result, (add_matrix(half, 0, 0, recursive_mult(half, partition(half, half, 0, a), partition(half, 0, half, b)), recursive_mult(half, partition(half, half, half, a), partition(half, half, half, b)))));
	}
	
	return result;
}

//strassens multiplication algorithm
//does not produce a correct matrix
//strangely, the values in a direct line from the bottom left to the top right are correct
//I believe the issue is in how the top-left (line 126) and bottom-right (line 132) lines are functioning
//since those do not interact with the correct cells (bottom left to top right) and the more they interact with a cell the more incorrect it ends up in the final matrix
double* Functions::strassen_mult(int size, double* a, double* b) {

	int size2 = size * size;
	int half = size / 2;
	double* result = new double[size2];

	//base case
	if (size == 1) {
		result[0] = a[0] * b[0];
		return result;
	}

	//build and store p1-7 for use later on
	double* p1 = strassen_mult(half, partition(half, 0, 0, a), subtract_matrix(half, 0, 0, partition(half, 0, half, b), partition(half, half, half, b)));
	double* p2 = strassen_mult(half, add_matrix(half, 0, 0, partition(half, 0, 0, a), partition(half, 0, half, a)), partition(half, half, half, b));
	double* p3 = strassen_mult(half, add_matrix(half, 0, 0, partition(half, half, 0, a), partition(half, half, half, a)), partition(half, 0, 0, b));
	double* p4 = strassen_mult(half, partition(half, half, half, a), subtract_matrix(half, 0, 0, partition(half, half, 0, b), partition(half, 0, 0, b)));
	double* p5 = strassen_mult(half, add_matrix(half, 0, 0, partition(half, 0, 0, a), partition(half, half, half, a)), add_matrix(half, 0, 0, partition(half, 0, 0, b), partition(half, half, half, b)));
	double* p6 = strassen_mult(half, subtract_matrix(half, 0, 0, partition(half, 0, half, a), partition(half, half, half, a)), add_matrix(half, 0, 0, partition(half, half, 0, b), partition(half, half, half, b)));
	double* p7 = strassen_mult(half, subtract_matrix(half, 0, 0, partition(half, 0, 0, a), partition(half, half, 0, a)), add_matrix(half, 0, 0, partition(half, 0, 0, b), partition(half, 0, half, b)));

	//top left		0,0
	place(size, 0, 0, result, subtract_matrix(half, 0, 0, add_matrix(half, 0, 0, p5, p4), add_matrix(half, 0, 0, p2, p6)));
	//top right		0,size/2
	place(size, 0, half, result, add_matrix(half, 0, 0, p1, p2));
	//bottom left	size/2,0
	place(size, half, 0, result, add_matrix(half, 0, 0, p3, p4));
	//bottom right	size/2,size/2
	place(size, half, half, result, subtract_matrix(half, 0, 0, add_matrix(half, 0, 0, p5, p1), subtract_matrix(half, 0, 0, p3, p7)));

	return result;
}

//add two identical matricies together and return the result
double* Functions::add_matrix(int size, int row, int col, double* a, double* b) {
	int size2 = size * size;
	double* result = new double[size2];
	for (int i = 0; i < size2; i++) {
		result[i] = a[i] + b[i];
	}

	return result;
}

//returns a - b
//should only be used with matricies of identical size
double* Functions::subtract_matrix(int size, int row, int col, double* a, double* b) {
	int size2 = size * size;
	double* result = new double[size2];
	for (int i = 0; i < size2; i++) {
		result[i] = a[i] - b[i];
	}

	return result;
}

//returns a sub-matrix 1/4th the total size (size value / 2) of the original from the given coordinants
//_row = y and _col = x are offsets used to position the new sub matrix
double* Functions::partition(int size, int _row, int _col, double* matrix){
	int size2 = size * size;
	double* newMatrix = new double[size2];

	//for each row
	for (int row = 0; row < size; row++) {
		//get and print the row-th item for that column
		for (int col = 0; col < size; col++) {
			newMatrix[(row * size) + col] = matrix[((row + _row) * size*2) + (col + _col)];
		}
	}
	return newMatrix;
}

//place child matrix into parent matrix at given coords
//assumes childs matrix size value is always half of the parents
void Functions::place(int size, int _row, int _col, double* parent, double* child) {
	//for each row
	for (int row = 0; row < size/2; row++) {
		//get and print the row-th item for that column
		for (int col = 0; col < size/2; col++) {
			parent[((row + _row) * size) + (col + _col)] = child[(row * size/2) + col];
		}
	}
}