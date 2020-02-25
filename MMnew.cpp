#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>
#include <iomanip>
#include <time.h> 

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
	int numRowsEvaluate; // Number of rows to evaluate per thread.
};

// Normal 2d matrix multiplication.
void* multi(void* args)
{	
	
	struct matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;
	
	int count = 0;
	int numRows = 1;
	int offset = 0;
	int res = 0;
	int resPlacement = 0;
	int i = 0;
   	for (i = 0; i <= orginalData->numColumns*orginalData->numColumns - 1; i = i + 1)
	{
		
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		count++;

		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset < orginalData->numRows * orginalData->numRows - orginalData->numRows))
		{
			offset = orginalData->numColumns * numRows;
			count = 0 + offset;
			numRows++;
			orginalData->Result[resPlacement] = res;
			resPlacement++;
			// have to double offset the for loop as i will increment after this which will take it back to 0
			i = -1;			

		}
		else if (count == (orginalData->numRows + offset))
		{	
			orginalData->Result[resPlacement] = res;
			res = 0;
			count = 0 + offset;
			resPlacement++;
		}


	}
}

void* rank2TensorMultPThread(void* args)
{	
	

	matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;
	int id = orginalData->id;
	orginalData->startRowOffset = id*orginalData->blockSizeForThread*orginalData->numRows;
	
	id++;
	orginalData->id = id;
	
	int numRows = 1;
	int offset = orginalData->startRowOffset;
	int res = 0;
	int count = offset;
	int resPlacement = offset;
	int i = 0;

   	for (i = 0; i <= orginalData->numColumns*orginalData->numColumns - 1; i++)
	{
		
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		count++;
		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset < id*orginalData->blockSizeForThread*orginalData->numRows - orginalData->numRows))
		{
			offset = offset + (orginalData->numColumns);
			count = offset;
			numRows++;
			orginalData->Result[resPlacement] = res;
			resPlacement++;
			i = -1;	// have to double offset the for loop as i will increment after this which will take it back to 0
			res = 0;
		}
		else if (count == (orginalData->numRows + offset))
		{	
			orginalData->Result[resPlacement] = res;
			res = 0;
			count = offset;
			resPlacement++;
		}
	}
	
	pthread_exit(NULL);

	/* // Used this for debugging purposes to output what has been computed in each thread. 
	   // Un comment this and the cin.get() in the thread for loop to obtain the output for each thread.
	   // However it is understood that this is not 100% accurate results as race conditions become a threat to the results when
	   // the program runs on its on with no intereference from the user during the multiplication (using cin.get()). 
	cout << endl << endl << "Results: Display Matrix C after a single thread" << endl;
	count = 0;
	// Displaying matC
   	for (int i = 0; i < orginalData->numColumns*orginalData->numColumns; i++)
	{
		cout << orginalData->Result[i] << "  ";
		count++;
		if (count == orginalData->numColumns)
		{
			cout << endl;
			count = 0;
		}

	}*/	
}

int main ( int argc, char* argv[] )
{
	// Define clock
	clock_t t;
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
	// Creat te struct
	struct matrixMultiplicationProps *matProps = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	
	// Declare Matrix A and Matrix B that will be multiplied together later on.
	int matA[randomSize][randomSize];
	int matB[randomSize][randomSize];
	matProps->rowNumbers = new int [randomSize*randomSize];
	matProps->columnNumbers = new int [randomSize*randomSize];
	matProps->Result = new int [randomSize*randomSize];
	

	// Populate Arrays For Multiplication
	for (int i = 0; i <= (randomSize*randomSize) - 1; i++)
	{  
		matProps->rowNumbers[i] = rand() % 10; 
		matProps->columnNumbers[i] = rand() % 10; 

	}

	int count = 0;
	int mat = 0;
	// Displaying matA	
    	cout << "Display 3-D Matrix A" << endl;
	cout << setw(3);
   	for (int i = 0; i <= randomSize*randomSize- 1; i++)
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

	// Displaying matB
	count = 0;
	mat = 0;
	int var = 0;
	int numMat = 0;
    	cout << "Display 3-D Matrix B" << endl;
	cout << setw(3);
   	for (int i = var; i <= randomSize*randomSize - 1; i += randomSize)
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

	// Could do a2d arry but might affect performance. 1d is O(n) and not O(n^2)
	// Conceded on the 1d array - too much abstraction to think about.
	// block size
	// should round up so can work for smaller matrices
	int blockSize = 5;
	
	// Dealing with matrix sizes that cannot deal with the block size selected. Therefore, do a single thread for those matrices.
	if (randomSize%blockSize != 0)
	{
		blockSize = randomSize;
	}

	matProps->blockSizeForThread = blockSize;
	pthread_t threads[randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	t = clock();
	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		//cin.get();
		pthread_create(&threads[i], NULL, multi, (void *)matProps);
		

	}

	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		pthread_join(threads[i], NULL);

	}
	t = clock() - t;

	// Displaying matC which is the result matrix
	count = 0;
    	cout << endl << endl << "Results: Display Matrix C After threading" << endl;
	cout << setw(6);
   	for (int i = 0; i <= randomSize*randomSize - 1; i++)
	{
		cout << matProps->Result[i] << setw(6);
		count++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}

	}

	cout << endl << "The number of threads used to compute the matrix multiplication is: " << randomSize/blockSize << " with a block size of " << blockSize << endl;

	cout << endl << ((float)t)/CLOCKS_PER_SEC;

 return 0;
}
