#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

void* func(void* args)
{
	// Casting very important
	//long long *limit_ptr = (long long*) vargp;
	//long long limit = *limit_ptr;
	vector<int>* v = static_cast<vector<int>*>(args);
	vector<int> vec1= *v;
	cout << "Vector size: " << vec1[5];
	
	
}

int main ( int argc, char* argv[] )
{

  vector<int> integers;
  pthread_t thread;


      for ( int i = 0; i < 10; i++)
        integers.push_back(i);


	pthread_create(&thread, NULL,func,&integers);
	pthread_join(thread, NULL);
	cout << endl;
       	cout << "Main program finished" << endl;
	cout << endl;

 return 0;
}
