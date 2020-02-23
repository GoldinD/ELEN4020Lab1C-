#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>


using namespace std;

struct matrixMultiplicationProps{
	int id = 1;
	int numRows;
	int numColumns;
	//int depth;
	int* rowNumbers = NULL;
	int* columnNumbers = NULL;
	int* Result = NULL;

	int blockSizeForThread;
	int startRowOffset;
	int startColumnOffset = 0;
	int placementOfResult;
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
   	for (i = 0; i <= orginalData->numColumns*orginalData->numColumns - 1; i++)
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
	

	//int columnCount = 0;
	//int currentRow = 1;
	int offset = orginalData->startRowOffset;
	//cout << "Offset: " << offset << endl;
	int res = 0;
	int count = offset;
	int resPlacement = offset;
	//cout << endl << endl <<"Thread:      " << id-1 << "     has been called" << endl;
	//cout << "ColumnOffset limit:                         " << id%orginalData->numRows << endl;
	//cin.get();
    	//cout << "Multiplication" << endl;
   	for (int i = orginalData->startColumnOffset; i <= orginalData->startColumnOffset + orginalData->numColumns*orginalData->numColumns - 1; i++)
	{
		
		res += orginalData->rowNumbers[count]*orginalData->columnNumbers[i];
		//cout << "rowValue: " << orginalData->rowNumbers[count] << " Multiplied By: " << orginalData->columnNumbers[i] << " equals: " << res << endl;
		
		count++;
		
		//cout << offset << endl;
		if ((i == orginalData->numColumns*orginalData->numColumns - 1) && (count == (orginalData->numRows + offset)) && (offset <orginalData->startRowOffset - orginalData->numRows))
		{
			//cout << endl << "New Row" << endl << endl;
			offset = offset + (orginalData->numColumns);
			count = 0 + offset;
			//currentRow++;
			orginalData->Result[resPlacement] = res;
			resPlacement++;
			orginalData->placementOfResult = resPlacement;
			// have to double offset the for loop as i will increment after this which will take it back to 0
			i = orginalData->startColumnOffset;	
			res = 0;
		}
		else if (count == (orginalData->numRows + offset))
		{	
			//cout << "Result Placement: " << resPlacement << "Result: " << res << endl;
			orginalData->Result[resPlacement] = res;
			res = 0;
			//cout << endl;	
			count = 0 + offset;
			resPlacement++;
			orginalData->placementOfResult = resPlacement;
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

int main ( int argc, char* argv[] )
{
	// Randomize length of row and column of the 2d array. N = either 10, 20, 30.
	int diffSizes[1] = {100};
	srand(time(NULL));
	//int random = rand()%3;
	int random = 0;
	int randomSize = diffSizes[random];
	cout << "Random Size: " << randomSize << endl;

	struct matrixMultiplicationProps *matProps = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	//matProps->depth = randomSize;
	
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
    	cout << "Display Matrix A" << endl;
   	for (int i = 0; i <= randomSize*randomSize*randomSize - 1; i++)
	{
		cout << matProps->rowNumbers[i] << "  ";
		count++;
		mat++;
		if (mat == randomSize*randomSize)
		{
			cout << endl << endl << endl;
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
    	cout << "Display Matrix B" << endl;
   	for (int i = 0; i <= randomSize*randomSize*randomSize - 1; i++)
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
	int blockSize = 1;


	matProps->blockSizeForThread = blockSize;
	pthread_t threads[randomSize*randomSize/blockSize];
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;


	for (int i = 0; i < randomSize*randomSize/blockSize; i++)
	{	
		pthread_create(&threads[i], NULL, rank3TensorMultPThread, (void *)matProps);
		//cin.get();
	}

	for (int i = 0; i < randomSize*randomSize/blockSize; i++)
	{	
		pthread_join(threads[i], NULL);

	}

	// Displaying matC
	count = 0;
	int i = 0;
    	cout << endl << endl << "Results: Display Matrix C After threading" << endl;
   	for (i = 0; i <= randomSize*randomSize*randomSize - 1; i++)
	{
		cout << matProps->Result[i] << "  ";
		count++;
		if (count == randomSize)
		{
			cout << endl;
			count = 0;
		}

	}	
	cout << i << endl;

	// Dealing with matrix sizes that cannot deal with the block size selected. Therefore, do a single thread for those matrices.
	/* if (randomSize%blockSize != 0)
	{
		blockSize = randomSize;
	}

	





*/

	
 return 0;
}
