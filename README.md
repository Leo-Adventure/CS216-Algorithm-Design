# CS216-Algorithm-Design
The repository contains the code of OJ and Project in Sustech CS216 2022 

## OJ1 Stable Matching

This week OJ questions' topic is stable matching, concentrating on one-to-one matching and one-to-many matching.

## OJ2 Greedy algorithm

OJ questions in this week are about using greedy algorithm wisely.

## OJ3 Simulation and Shortest Path

The question A is about the simulation of the machanism of LRU

The question B is about the shortest path of the weighted graph and reverse of modulo.

## OJ4 Greed algorithm and Distance transformation

The question A is about the greedy algorithm 

The question B is about the transformation between **Manhattan Distanc** and **Chebyshev Distance**

## OJ5 Divide and Conquer: Peano curve and Segment Tree

The question A is about the Peano curve, which is the advanced level of Hilbert curve, quite interesting.

The question B is the application of segment Tree, which is quite useful in updating the interval value and finding the sum of a given interval in $O(nlogn)$

## OJ7 Dynamic Planning

The question A is about the simple dynamic planning and searching with memorization.

The question B is about the dynamic planning and interesting derivation of formula.

## Assignment1 Report on modern CPU caching

Please write a simple report to introduce the caching mechanism used in modern CPUs. Try to answer the following, but not limit to, questions in your report.

1. What is the cache performance measurement?

2. What are the cache replacement policies, LRU?

3. Any other policies and methods to improve performance?

​	The report should not exceed 2 pages in font size 10.

## Assignment2 Design an algorithm to find the closest pair of a 3D point cloud 

In this assignment, I have designed, implemented and analyzed the algorithm to find the closest pair of 3D point cloud.

The procedure of designing the algorithm has been introduced detailedly.

What's more, I have uploaded the implemented code to the directory “Assignment/Assignment2/closed-pair.cpp”

## Assignment3 Using FFT to accelerate the convolution

In this assignment, I just called the FFT in Python to complete it.

## Project 1 Zip Compressor

Implement a ZIP compressor using Java or C/C++. 

1.  It can compress one sinle file into a zip file. Multiple-file compression is optional.
2.  The compressed zip file can be decompressed successfully by professional zip software. Such as it can be decompressed by double clicking in Windows/MacOS. 
3.  You can implement the commonly used compression algorithm DEFLATE and static Huffman in DEFLATE. 
4.  Please submit by the deadline. Otherwise 0 score. Sumit your report in PDF which clearly introduces your implementation and the highlights, and submit your source code in a zip file. 
5. If you reference some source code from others, please state it clearly in the report and give references at the end of the report.

### Progress

#### 2022/4/6

Complete the zero-compress convertion of simple txt file (only contains "Hello World!") to zip file, which can be decompressed by OS.

Know the meaning of each segment of a zip file.

#### 2022/4/7

Complete the zero-compress convertion of simple file (not only contains "Hello World!!!") to zip file, which can be decompressed by OS

replace each segment of zip file to what it should be.

The key segments are:

- last modified time and date (quite troublesome)
- the crc32 encryption 
- the size of file before and after compressing.
- offset of start of central directory with respect to the starting disk number

#### 2022/4/8

- implement the LZ77 algorithm but the alogorithm have not been tested.
- Implement the bit stream emitter and LZ77 encoding.
- Tomorrow the algorithm will be tested and static Huffman will be implemented.
- Fighting

#### 2022/4/9

- verify the LZ77 algorithm and implement the static Huffman compression.
- Huffman has been tested, meaning that deflate has been done
- Next I will combine the header and deflate algorithm, good luck!

#### 2022/4/10

- Correct some Bugs and combine the header and deflate successfully~
- complete the zip deflate using static huffman compression~ Hurray!
- Next I will improve the algorithm and do many experiments~
- improve the structure of the zip files, dividing them into three files——zip.hpp, zip.cpp, main.cpp and write CMakeLists.txt to run them easily.
- update some bugs(central directory size bug ~)

The `zip.hpp` file is header file, and the `zip.cpp` is the source file.

#### 2022/4/11

- Going to fix the bug that I can not compress the file whose size is larger than 20KB

### How to run it

Your device should support the minimum cmake version of 3.16 and the C++ version is C++17.

just arrange the file structure as shown, then run the

```bash
cmake . && make
```

commands so that the executable file named `zip` will be generated.

Then run `./zip` and input the source txt file you want to compress and input the zip file name you want to generate, then a zip file will be generated.

#### 2022/4/16

- fix the bug that the Chinese text file cannot be compressed.
- do some experiments and update report.
