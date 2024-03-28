# Lab 3 MIPT Algorithms course work report

### Done
1. Quadratic sorts (including Shell sort)
2. Heap sort
3. Merge sort (recursive and iterative)
4. Quick sort (Lomuto, Hoare and thick partitions)
5. Quick sort optimizations

### Report
1. **Quadratic sorts**
*Plot:*
*Result:*
Shell sort shows significantly better performance than other sorts, being twice faster than Selection sort.
Selection sort works faster than Insertion sort, and Bubble sort is the slowest.
<br>

2. **Heap sort**
*Plot:*
*Result:*
<br>

3. **Merge sort**
*Plot:*
*Result:*
Recursive and iterative versions have similar performance, so there is no significant difference between them, except memory usage.
Recursive Merge sort uses $O(n + logn)$ memory, while iterative Merge sort uses $O(n)$ memory because it doesn't need stack for function calls.
<br>


4. **Quick sort**
4.1. Big tests
*Plot:*
*Result:*
Hoare partition is the fastest, then goes Lomuto partition and thick partition.
4.2 Most dublicates tests
*Plot:*
*Result:*
Here thick partition shows better results than in the previous paragraph - better than Lomuto partition. Hoare partition is still the best.


### ToDo
1. Intro sort
2. LSD & MSD sorts
