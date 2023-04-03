# include <ctime>
# include <math.h>
# include <time.h>
# include <fstream> 
# include <cstdlib>
# include <iostream>

using namespace std;

bool size1 = false, size2 = false, size3 = false, size4 = false;
void initializer(int size)
{
	// size initializer
	size = pow(2, size);

	// array initializer
	int array[size];
	srand(time(0));
	for (int i = 0; i < size; i++) 
		array[i] = (rand() % 100) + 10; // Range(10,100)	
		
	// array writer
	if (size4)
	{
		ofstream fout("17.txt");
		for (int i = 0; i < size; i++) 
			fout << array[i] << " ";		
	}
	else if (size3)
	{
		ofstream fout("15.txt");
		for (int i = 0; i < size; i++) 
			fout << array[i] << " ";		
	}
	else if (size2)
	{
		ofstream fout("13.txt");
		for (int i = 0; i < size; i++) 
			fout << array[i] << " ";		
	}
	else if (size1)
	{
		ofstream fout("11.txt");
		for (int i = 0; i < size; i++) 
			fout << array[i] << " ";		
	} 
}

int main()
{	
	size1 = true;
	initializer(11);
	size2 = true;
	initializer(13);
	size3 = true;
	initializer(15);
	size4 = true;
	initializer(17);	
		
	cout << "Arrays Created" << endl;
	return 0;
}
