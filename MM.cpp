#include <iostream>
#include <vector>
#include <pthread.h>	
#include <typeinfo>


using namespace std;

pthread_mutex_t lock;

struct matrixMultiplicationProps{
	int numRows;
	int numColumns;
	vector<int> rowNumbers;
	vector<int> columnNumbers;
};


void* multi(void* args)
{	
	
	struct matrixMultiplicationProps *orginalData = (struct matrixMultiplicationProps*)args;

	// make new struct to hold original data so the old struct can be used by other threads.
	struct matrixMultiplicationProps *newData = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));	

	// Populate new struct
	newData->numRows = orginalData->numRows;
	newData->numColumns = orginalData->numColumns;

	for (int i = 0; i < orginalData->rowNumbers.size(); i++)
	{	
		newData->rowNumbers.push_back(orginalData->rowNumbers[i]);
		newData->columnNumbers.push_back(orginalData->columnNumbers[i]);
	}

	for (int i = 0; i < newData->columnNumbers.size(); i++)
	{	
		cout << newData->columnNumbers[i] << endl;
	}
	
	
	
	
	
}

int main ( int argc, char* argv[] )
{
	// Randomize length of row and column of the 2d array. N = either 10, 20, 30.
	int diffSizes[3] = {4, 2, 3};
	srand(time(NULL));
	int random = rand()%3;
	int randomSize = diffSizes[random];
	cout << randomSize << endl;

	struct matrixMultiplicationProps *matProps = (struct matrixMultiplicationProps*) malloc(sizeof(struct matrixMultiplicationProps));
	matProps->numRows = randomSize;
	matProps->numColumns = randomSize;
	
	// Declare Matrix A and Matrix B that will be multiplied together later on.
	int matA[randomSize][randomSize];
	int matB[randomSize][randomSize];

	// Generating random values in matA and matB 
    	for (int i = 0; i < randomSize; i++) 
	{ 
	        for (int j = 0; j < randomSize; j++) 
		{ 
            		matA[i][j] = rand() % 10; 
            		matB[i][j] = rand() % 10; 
        	} 
    	} 

	// Displaying matA and populating the struct of the numbers used for the row plane in multiplication
    	cout << "Display Matrix A" << endl;
   	for (int i = 0; i < randomSize; i++)
	{
        	for (int j = 0; j < randomSize; j++) 
		{
            		cout << matA[i][j] << "  ";
			// Check when to start a new row
            		if (j == randomSize - 1) 
			{
                		cout << endl << endl;
            		}
        	}
	}

	// Displaying matB and populating the struct of the numbers used for the column plane in multiplication
    	cout << "Display Matrix B" << endl;
   	for (int i = 0; i < randomSize; i++)
	{
        	for (int j = 0; j < randomSize; j++) 
		{
            		cout << matB[i][j] << "  ";
			// Check when to start a new row
            		if (j == randomSize - 1) 
			{
                		cout << endl << endl;
            		}
        	}
	}

		//for (int i = 0; i < matProps->columnNumbers.size(); i++)
		//{
		//	cout << matProps->columnNumbers[i] << "  ";

		//}

	// Declare threads
	pthread_t threads[randomSize*randomSize];
	pthread_mutex_t count_mutex;
	
	// start with row 1
	int count = 0;
	int thread_count = 0;
	int i = 0; 
	cout << "Performing Matrix Multiplication" << endl;
   	for (i = 0; i <= randomSize - 1; i++)
	{  
		//cout << endl << "Original count: " << count << endl;
		matProps->rowNumbers.push_back(matA[count][i]);
		matProps->columnNumbers.push_back(matB[i][count]);
		//cout << matProps->columnNumbers[i] << endl;
		//cout << matProps->rowNumbers[i] << endl;

		thread_count++;
		if (i == randomSize - 1)
		{
			 
			pthread_create(&threads[thread_count], NULL, multi, (void *)matProps);
			//pthread_join(threads[thread_count], NULL);
			
			cout << endl << "got here 1" << endl;
			count++;
			matProps->rowNumbers.clear();
			matProps->columnNumbers.clear();
			i = -1;	
							
		}
		if(count == randomSize)
		{
			cout << endl << "got here 2" << endl;

			break;
		}
		
		              
	}
 
	
	

	

	/////////////////////////////////////////////////////////////
  	//pthread_t thread;

	 //vector<int> integers;
      //for ( int i = 0; i < 10; i++)
        //integers.push_back(i);


	//pthread_create(&thread, NULL,func,&integers);
	//pthread_join(thread, NULL);
	//cout << endl;
       	//cout << "Main program finished" << endl;
	//cout << endl;
	
 return 0;
}
