#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>
#include <iomanip>

using namespace std;

struct matrixMultiplicationProps{
	int id = 1;
	int numRows;
	int numColumns;
	int* rowNumbers = NULL;
	int* columnNumbers = NULL;
	int* Result = NULL;

	int blockSizeForThread;
	int startRowOffset;
	int startColumnOffset = 0;
	int placementOfResult;
	int numRowsEvaluate; // Number of rows to evaluate per thread.
};

void* rank3TensorMultPThread(void* args)
{	
	
	matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;
	int id = orginalData->id;
	orginalData->startRowOffset = id*orginalData->blockSizeForThread*orginalData->numRows;

	id++;
	orginalData->id = id;

	if (id%orginalData->numRows == 1 && id != 1)
	{
		orginalData->startColumnOffset += orginalData->numRows*orginalData->numRows;
	}
	
	int offset = orginalData->startRowOffset;
	int res = 0;
	int count = offset;
	int resPlacement = offset;
   	for (int i = orginalData->startColumnOffset; i <= orginalData->startColumnOffset + orginalData->numColumns*orginalData->numColumns - 1; i++)
	{
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		count++;

		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset <orginalData->startRowOffset - orginalData->numRows))
		{
			offset = offset + (orginalData->numColumns);
			count = offset;
			orginalData->Result[resPlacement] = res;
			resPlacement++;
			orginalData->placementOfResult = resPlacement;
			i = orginalData->startColumnOffset;// have to double offset the for loop as i will increment after this which will take it back to 0	
			res = 0;
		}

		else if (count == (orginalData->numRows + offset))
		{	
			orginalData->Result[resPlacement] = res;
			res = 0;
			count = offset;
			resPlacement++;
			orginalData->placementOfResult = resPlacement;
		}
	}
	pthread_exit(NULL);	
}

int main ( int argc, char* argv[] )
{
	int randomSize = 0;

	// User input
	while (randomSize != 10 | randomSize != 20 | randomSize != 30)
	{
		cout << "Please enter either, 10, 20 or 30 for the size of the matrix you want to do multiplication on:";
		cin >> randomSize;
		if (randomSize == 10 || randomSize == 20 || randomSize == 30)
		{
			cout << endl;
			break;
		}
	}	
	
	srand(time(NULL));

	// Create struct
	struct matrixMultiplicationProps *matProps = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	
	// Declare Matrix A and Matrix B that will be multiplied together later on.
	matProps->rowNumbers = new int [randomSize*randomSize*randomSize];
	matProps->columnNumbers = new int [randomSize*randomSize*randomSize];
	matProps->Result = new int [randomSize*randomSize*randomSize];
	

	// Populate Arrays For Multiplication
	for (int i = 0; i <= (randomSize*randomSize*randomSize) - 1; i++)
	{  
		matProps->rowNumbers[i] = rand() % 10; 
		matProps->columnNumbers[i] = rand() % 10; 

	}

	int count = 0;
	int mat = 0;
	// Displaying matA
    	cout << "Display 3-D Matrix A" << endl;
	cout << setw(3);
   	for (int i = 0; i <= randomSize*randomSize*randomSize - 1; i++)
	{
		cout << matProps->rowNumbers[i] << setw(3);
		count++;
		mat++;
		if (mat == randomSize*randomSize)
		{
			cout << endl;
			mat = 0;
		}
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}
	}

	// Displaying matB in matrix form
	count = 0;
	mat = 0;
	int var = 0;
	int numMat = 0;
    	cout << "Display 3-D Matrix B" << endl;
	cout << setw(3);
   	for (int i = var; i <= randomSize*randomSize*randomSize - 1; i += randomSize)
	{
		cout << matProps->columnNumbers[i] << setw(3);
		count++;
		mat++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
			var++;
			i = var-randomSize;
			
		}
		if (mat == randomSize*randomSize)
		{	
			numMat++;
			cout << endl;
			mat = 0;
			var = numMat*randomSize*randomSize;
			i = numMat*randomSize*randomSize - randomSize;
		}
	}

	// Block size
	int blockSize = 1;

	matProps->blockSizeForThread = blockSize;
	pthread_t threads[randomSize*randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;

	// Threading
	for (int i = 0; i < randomSize*randomSize/blockSize; i++)
	{	
		pthread_create(&threads[i], NULL, rank3TensorMultPThread, (void *)matProps);
	}

	for (int i = 0; i < randomSize*randomSize/blockSize; i++)
	{	
		pthread_join(threads[i], NULL);

	}

	// Displaying matC
	count = 0;
	int i = 0;
	int slice = 1;
	int numRows = 0;
    	cout << endl << endl << "Results: Display Matrix C After threading" << endl;
	cout << endl << "Slice Number: " << slice << endl << endl;
	cout << setw(6);
   	for (i = 0; i <= randomSize*randomSize*randomSize - 1; i++)
	{
		if (numRows == randomSize)
		{
			numRows = 0;

			slice++;
			cout << endl << endl << "Slice Number: " << slice << endl << endl;
		}

		cout << setw(6) << matProps->Result[i] << setw(6);
		count++;

		if (count == randomSize)
		{
			numRows++;
			cout << endl;
			count = 0;
		}

	}	

	cout << endl << "The number of threads used to compute the matrix multiplication is: " << randomSize*randomSize/blockSize << " with a block size of " << blockSize << endl;

 return 0;
}
