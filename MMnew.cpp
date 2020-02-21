#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>


using namespace std;

struct matrixMultiplicationProps{
	int numRows;
	int numColumns;
	int* rowNumbers = NULL;
	int* columnNumbers = NULL;
	int* Result = NULL;
};


void* multi(void* args)
{	
	
	struct matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;

	// Displaying matA
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
		//cout << "rowValue: " << orginalData->rowNumbers[count] << " Multiplied By: " << orginalData->columnNumbers[i] << " equals: " << res << endl;
		
		count++;
		
		//cout << resPlacement << endl;
		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset < orginalData->numColumns*orginalData->numColumns - orginalData->numColumns))
		{
			//cout << endl << "New Row" << endl << endl;
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
			//cout << endl;	
			count = 0 + offset;
			resPlacement++;
		}


	}
	cout << endl << endl << "Results: Display Matrix C" << endl;
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
	pthread_t threads[randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	for (int i = 0; i < 1; i++)
	{	
		pthread_create(&threads[i], NULL, multi, (void *)matProps);

	}

	for (int i = 0; i < 1; i++)
	{	
		pthread_join(threads[i], NULL);

	}

	
 return 0;
}
