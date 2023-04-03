# Parallel-and-Distributed-Sorting-with-MPI
This project implements Quick and Merge sorting algorithms using MPI for parallel and distributed computing.

This project focuses on implementing parallel and distributed sorting algorithms using the Message Passing Interface (MPI) and analyzing their performance. The program is written in the C/C++ programming language and consists of three main parts - creating the input data and implementing the sorting algorithms with and without MPI.

To generate input data, a makearray.cpp file is used to create four arrays of different sizes, with sizes of 2 to the power of 11, 13, 15, and 17. These arrays are used as input data for the sorting algorithms, and their sizes are chosen to cover a wide range of problem sizes to analyze the performance of the sorting algorithms at different scales.

The sorting algorithms implemented in the project are Quick and Merge sort, which are commonly used algorithms for sorting data. The parallel and distributed versions of these algorithms are implemented using MPI to take advantage of the computing power of multiple processors and distributed computing resources.

To analyze the performance of the sorting algorithms, the program calculates GFLOPS, a metric used to measure the number of floating-point operations per second. The program is implemented with and without MPI to compare the performance of the parallel and distributed versions of the sorting algorithms with the sequential version.

Overall, this project provides a comprehensive analysis of the performance of parallel and distributed sorting algorithms using MPI and demonstrates their effectiveness in improving the efficiency of sorting large datasets.

NOTE: To run these files, commands are given on first lines of respective files. This code is ubuntu compatible only.

Step 1: Run makearray.cpp file<br> 1) g++ makearray.cpp -o array.exe<br> 2) .\array 

Step 2: Run Q1.cpp file (Without MPI)<br> 3) g++ Q1.cpp -o q1.exe<br> 4) .\q1 11.txt 11sorted.txt

Step 3: Run Q2.cpp file<br> 5) sudo mpicc Q2.c -o out -lm<br> 6) sudo mpiexec -n 4 ./out 11.txt
