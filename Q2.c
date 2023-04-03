// 1) sudo mpicc Q2.c -o out -lm 2) sudo mpiexec -n 4 ./out 11.txt

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <mpi.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>
#include <sys/time.h>

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
	int *leftside = malloc(sizeof(firsthalf) * sizeof(int));
	
	for (int x = 0; x < firsthalf; x++)
		leftside[x] = array[startpoint + x];
	
	// compute second half	
	int secondhalf = endpoint - midpoint;
	int *rightside = malloc(sizeof(secondhalf) * sizeof(int));
	
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



int main(int argc, char* argv[])
{

	MPI_Init(&argc, &argv);

	// condition: 4 processes
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size != 4)
	{
		printf("This application is meant to be run with 4 processes.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// root rank is always zero
	int root_rank = 0;
	
	// current rank
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// allocate size
	int array_size = pow(2, atoi(argv[argc-1]));
		
	// calculations w.r.t. roll-number
	int last_digit = pow(2, 2); 
	int second_last_digit = pow(2, 10);
	int third_last_digit = pow(2, 4);
	int remaining = array_size - (last_digit + second_last_digit + third_last_digit);

	// start & end points
	int end_points[4] = {last_digit, second_last_digit, third_last_digit, remaining};
	int start_points[4] = {0, last_digit, last_digit + second_last_digit, last_digit + second_last_digit + third_last_digit};

	int my_value[100000];
	

	if (my_rank == 0)
	{
		int buffer[array_size];
		
		// read file
		char file_read[] = "13.txt";
		
		FILE* fp;
		fp = fopen(file_read, "r");
		
		char number[3];
		int i = 0;
		
		while (fscanf(fp, "%s", number) == 1)
			buffer[i++] = atoi(number);

		// assigning root 0 rank
		my_rank = root_rank;
		
		// allocate memory
		my_value[end_points[my_rank]];
	
		// disperse array
		MPI_Scatterv(buffer, end_points, start_points, MPI_INT, my_value, end_points[my_rank], MPI_INT, root_rank, MPI_COMM_WORLD);
	}
	else
	{
		// allocate memory
		my_value[end_points[my_rank]];
		
		// disperse array
	    MPI_Scatterv(NULL, NULL, NULL, MPI_INT, my_value, end_points[my_rank], MPI_INT, root_rank, MPI_COMM_WORLD);
	}
	
	// copying arrays
	int my_value1[end_points[my_rank]]; 
	int my_value2[end_points[my_rank]];
	
	for (int i = 0; i < end_points[my_rank]; i++)
	{
		my_value1[i] = my_value[i];
		my_value2[i] = my_value[i];
	}
		
	int operation_count = array_size * log(array_size);//number of op
    	struct timeval start, end;

    	gettimeofday( &start, NULL );
    
	// sort individual array
	quickSort(my_value1, 0, end_points[my_rank] - 1);
	
	// gather individual sorted arrays
	int all_sorted_quick[array_size];
	
	MPI_Gatherv(my_value1, end_points[my_rank] , MPI_INT, all_sorted_quick, end_points, start_points, MPI_INT, 0, MPI_COMM_WORLD); 
	
	// block until point is reached
	MPI_Barrier(MPI_COMM_WORLD);
	
	// write at root
	if (my_rank == root_rank)
	{
		// sorting combined arrays
		quickSort(all_sorted_quick, 0, array_size - 1);
		
		FILE* fp;
		fp = fopen("sorted.txt", "w");
		
		// write
		for (int i = 0; i < array_size; i++)
		{
			fprintf(fp , "%i ", all_sorted_quick[i]);
			fprintf(fp ," ");
		}
		
		fprintf(fp , "\n\n\n");
		
		fclose(fp);
		
		gettimeofday( &end, NULL );

	    /* convert time to Gflop/s */
	    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	    double Gflops = 2e-9*operation_count/seconds;
	    printf( " Performance of Quick Sort in Gflops %.8f Gflop/s\n", Gflops );
	    
	    operation_count += 1000; 
	}
	
	// sort individual array
	quickSort(my_value2, 0, end_points[my_rank] - 1);
	
	// gather individual sorted arrays
	int all_sorted_merge[array_size];
	
	MPI_Gatherv(my_value2, end_points[my_rank] , MPI_INT, all_sorted_merge, end_points, start_points, MPI_INT, 0, MPI_COMM_WORLD); 
	
	// block until point is reached
	MPI_Barrier(MPI_COMM_WORLD);
	
	// write at root
	if (my_rank == root_rank)
	{
		// sorting combined arrays
		quickSort(all_sorted_merge, 0, array_size - 1);
		
		FILE* fp;
		fp = fopen("sorted.txt", "a");
		
		// write
		for (int i = 0; i < array_size; i++)
		{
			fprintf(fp , "%i ", all_sorted_merge[i]);
			fprintf(fp ," ");
		}
		
		fprintf(fp , "\n\n\n");
		
		fclose(fp);
		

	    /* convert time to Gflop/s */
	    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
	    double Gflops = 2e-9*operation_count/seconds;
	    printf( " Performance of Merge Sort in Gflops %.8f Gflop/s\n", Gflops );
	    
	}
	
	// terminate
	MPI_Finalize();

	return EXIT_SUCCESS;
}
