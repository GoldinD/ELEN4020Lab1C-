#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>


using namespace std;

	pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

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


void* multi(void* args)
{	
	
	struct matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;
	
	int count = 0;
	int columnCount = 0;
	int numRows = 1;
	int offset = 0;
	int res = 0;
	int resPlacement = 0;
	int i = 0;
    	cout << "Multiplication" << endl;
   	for (i = 0; i <= orginalData->numColumns*orginalData->numColumns - 1; i = i + 1)
	{
		
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		cout << "rowValue: " << orginalData->rowNumbers[count] << " Multiplied By: " << orginalData->columnNumbers[i] << " equals: " << res << endl;
		
		count++;
		
		cout << offset << endl;
		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset < orginalData->blockSizeForThread * orginalData->numRows - orginalData->numRows))
		{
			cout << endl << "New Row" << endl << endl;
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
			cout << endl;	
			count = 0 + offset;
			resPlacement++;
		}


	}
	/*cout << endl << endl << "Results: Display Matrix C With a single thread" << endl;
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

void* rank2TensorMultPThread(void* args)
{	
	

	matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;
	int id = orginalData->id;
	orginalData->startRowOffset = id*orginalData->blockSizeForThread*orginalData->numRows;
	
	id++;
	orginalData->id = id;
	
	int columnCount = 0;
	int numRows = 1;
	int offset = orginalData->startRowOffset;
	cout << "Offset: " << offset << endl;
	int res = 0;
	int count = offset;
	int resPlacement = offset;
	int i = 0;
	cout << endl << endl <<"Thread:      " << id-1 << "     has been called" << endl;
	cout << "Offset limit: " << id*orginalData->blockSizeForThread*orginalData->numRows - orginalData->numRows << endl;
	//cin.get();
    	cout << "Multiplication" << endl;
   	for (i = 0; i <= orginalData->numColumns*orginalData->numColumns - 1; i = i + 1)
	{
		
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		cout << "rowValue: " << orginalData->rowNumbers[count] << " Multiplied By: " << orginalData->columnNumbers[i] << " equals: " << res << endl;
		
		count++;
		
		cout << offset << endl;
		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset < id*orginalData->blockSizeForThread*orginalData->numRows - orginalData->numRows))
		{
			cout << endl << "New Row" << endl << endl;
			offset = offset + (orginalData->numColumns);
			count = 0 + offset;
			numRows++;
			orginalData->Result[resPlacement] = res;
			resPlacement++;
			// have to double offset the for loop as i will increment after this which will take it back to 0
			i = -1;	
			res = 0;
		}
		else if (count == (orginalData->numRows + offset))
		{	
			cout << "Result Placement: " << resPlacement << "Result: " << res << endl;
			orginalData->Result[resPlacement] = res;
			res = 0;
			cout << endl;	
			count = 0 + offset;
			resPlacement++;
		}
		
		


	}

		cout << endl << endl << "Results: Display Matrix C With a single thread" << endl;
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

	}
		
	
	
}

int main ( int argc, char* argv[] )
{
	// Randomize length of row and column of the 2d array. N = either 10, 20, 30.
	int diffSizes[1] = {5};
	srand(time(NULL));
	//int random = rand()%3;
	int random = 0;
	int randomSize = diffSizes[random];
	cout << "Random Size: " << randomSize << endl;

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
	
	// Displaying matA
    	cout << "Display Matrix A" << endl;
   	for (int i = 0; i <= randomSize*randomSize - 1; i++)
	{
		cout << matProps->rowNumbers[i] << "  ";
		count++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}

	}

	// Displaying matB
	count = 0;
    	cout << "Display Matrix B" << endl;
   	for (int i = 0; i <= randomSize*randomSize - 1; i++)
	{
		cout << matProps->columnNumbers[i] << endl;
		count++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}

	}

	// Could do a2d arry but might affect performance. 1d is O(n) and not O(n^2)
	// Conceded on the 1d array - too much abstraction to think about.
	// block size
	// should round up so can work for smaller matrices
	int blockSize = 5;
	matProps->blockSizeForThread = blockSize;
	pthread_t threads[randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		//matProps->id = i;
		//cout << i << endl;
		
		//matProps->startRowOffset = i*blockSize*randomSize;
		pthread_create(&threads[i], NULL, rank2TensorMultPThread, (void *)matProps);
		//cin.get();

	}

	for (int i = 0; i < randomSize/blockSize; i++)
	{	
		pthread_join(threads[i], NULL);

	}

	// Displaying matC
	count = 0;
    	cout << endl << endl << "Results: Display Matrix C After threading" << endl;
   	for (int i = 0; i <= randomSize*randomSize - 1; i++)
	{
		cout << matProps->Result[i] << "  ";
		count++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}

	}

	
 return 0;
}
