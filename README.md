# 18FallAlgorithm
Sogang Univ. 18 Fall Semester Assignment
---------------------------------------
## Assignment 1: Maximum Subsequence Sum

1. Goal
    + Finding a subsequence which has the maximum subsequence.
    + Understanding how each algorithm works.

2. How to run file
    1. use makefile to make binary file
    2. the input form must be 
        - example
        ```bash
        ./mms20171635 input00001.txt 1
        ```

3. Time Complexity of Index
    1. O(n^2)
    2. O(nlogn)
    3. O(n)

4. Result File: result_[inputfilename].txt <br>
    1st line: input file name<br>
    2nd line: algorithm index<br>
    3rd line: input size (the number of elements in the original sequence)<br>
    4th line: index of the leftmost number in the subsequence found<br>
    5th line: index of the rightmost number in the subsequence found<br>
    6th line: sum of the subsequence<br>
    7th line: running time in milliseconds<br>

-------------------------------------------
## Assignment 2: Master of Sorting

1. Goal
    + Understand performance of different sorting algorithms.

2. How to run file
    1. use makefile to make binary file
    2. the input form must be
        - example
        ```bash
        ./mms20171635 input00001.txt 1
        ```
3. Algorithms used in assignment
    1. Insertion sort: O(n^2)
    2. Heap sort: O(nlogn)
    3. Quick sort: O(nlogn) || O(n^2)
    4. Median of Three Quick sort: O(nlogn)

4. Result File: result_[index]_[inputfilename].txt <br>
    1st line: input file name<br>
    2nd line: algorithm index<br>
    3rd line: input size (the number of elements)<br>
    4th line: running time in second<br>
    5th line: list of sorted data<br>

