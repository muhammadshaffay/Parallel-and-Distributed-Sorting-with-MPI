// 1) g++ makearray.cpp -o array.exe 2) .\array 3) g++ Q1.cpp -o q1.exe 4) .\q1 11.txt 11sorted.txt

# include <ctime>
# include <math.h>
# include <time.h>
# include <vector>
# include <fstream> 
# include <cstdlib>
# include <string.h>
# include <iostream>
# include <sys/time.h>

using namespace std;

int quick(int *array, int startpoint, int endpoint) 
{ 	
	int temp = 0, current = startpoint - 1, pivot = array[endpoint];
	for (int index = startpoint; index < endpoint; index++) 
	{
		if (array[index] > pivot)
			continue;
			 
		else if (array[index] <= pivot) 
		{
			current++;
			
			// swap
			temp = array[current];
			array[current] = array[index];
			array[index] = temp;
		}
	}
	
	// swap 
	temp = array[current + 1];
	array[current + 1] = array[endpoint];
	array[endpoint] = temp;	
	
	return ++current;
}

void quickSort(int *array, int startpoint, int endpoint) 
{
	if (startpoint < endpoint) 
	{
		// initialize pivot point
		int pivotpoint = quick(array, startpoint, endpoint);
		
		// from start to pivot
		quickSort(array, startpoint, pivotpoint - 1);
		
		// from pivot to end
		quickSort(array, pivotpoint + 1, endpoint);
	}
}

void merge(int *array, int startpoint, int midpoint, int endpoint) 
{
	int index = startpoint;
	
	// compute first half
	int firsthalf = midpoint - startpoint + 1;
	int *leftside = new int [firsthalf];
	
	for (int x = 0; x < firsthalf; x++)
		leftside[x] = array[startpoint + x];
	
	// compute second half	
	int secondhalf = endpoint - midpoint;
	int *rightside = new int[secondhalf];
	
	for (int y = 0; y < secondhalf; y++)
		rightside[y] = array[midpoint + 1 + y];
	
	int x = 0, y = 0;
	
	while (x < firsthalf && y < secondhalf) 
	{
		if (leftside[x] <= rightside[y]) 
		{
		  array[index] = leftside[x];
		  x++;
		} 
		else if (leftside[x] > rightside[y]) 
		{
		  array[index] = rightside[y];
		  y++;
		}
		index++;
	}
	
	// merge first half
	while (x < firsthalf)
	{
		array[index] = leftside[x];
		x++; index++;
	}
	
	// merge second half
	while (y < secondhalf) 
	{
		array[index] = rightside[y];
		y++; index++;
	}
}

void mergeSort(int *array, int startpoint, int endpoint) 
{
	if (endpoint > startpoint) 
	{
		// initialize midpoint
		int midpoint = (((endpoint - startpoint) / 2) + startpoint);
		
		// from start to mid
		mergeSort(array, startpoint, midpoint);
		
		// from mid to end
		mergeSort(array, midpoint + 1, endpoint);
		
		// merge sorted parts
		merge(array, startpoint, midpoint, endpoint);
	}
}

void write(int* array1, int*array2, int size, char* writefilename)
{
	ofstream fout(writefilename);
	for (int i = 0; i < size; i++) 
		fout << array1[i] << " "; 
		
	fout << "\n\n\n" ;
	
	for (int i = 0; i < size; i++) 
		fout << array2[i] << " "; 

}

int main(int arg, char **argv)
{
	if (arg < 3)
		return 0;
		
	char *readfilename = argv[1];
	char *writefilename = argv[2];
		
	ifstream inputFile(readfilename);
	vector<int> array;
	
	// read file via vector
	int number;
    while (inputFile >> number) 
        array.push_back(number);
    
    int size = array.size();

	int *array1 = array.data();
	int *array2 = array.data();

	// Gflops arguments
	int operation_count = size;
    struct timeval start, end;
    double seconds, Gflops;
		
	// merge sort
	gettimeofday( &start, NULL );
	quickSort(array1, 0, size - 1);
	gettimeofday( &end, NULL );
	
	seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	Gflops = 2e-9 * operation_count / seconds;
    printf("Performance of Quick Sort in Gflops %.8f Gflop/s\n", Gflops);
            
	// quick sort
	gettimeofday( &start, NULL );
	mergeSort(array2, 0, size - 1);
	gettimeofday( &end, NULL );
	
	seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	Gflops = 2e-9 * operation_count / seconds;
    printf("Performance of Merge Sort in Gflops %.8f Gflop/s\n", Gflops);
	
	write(array1, array2, size, writefilename);
		
    return 0;
}
