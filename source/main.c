#include "../include/testing.h"



int main()
{
    TestSort ("tests/small_tests", "output/bubblesort.txt",
              0, 1000, 50, 5, BubbleSort);

    TestSort ("tests/small_tests", "output/insertionsort.txt",
              0, 1000, 50, 5, InsertionSort);

    TestSort ("tests/small_tests", "output/selectionsort.txt",
              0, 1000, 50, 5, SelectionSort);

    TestSort ("tests/small_tests", "output/shellsort.txt",
              0, 1000, 50, 5, ShellSort);

    // TestSort ("tests/medium_tests", "output/2heapsort.txt",
    //           0, 100000, 1000, 1, TwoHeapSort);

    // TestSort ("tests/medium_tests", "output/3heapsort.txt",
    //           0, 100000, 1000, 1, ThreeHeapSort);

    // TestSort ("tests/medium_tests", "output/4heapsort.txt",
    //           0, 100000, 1000, 1, FourHeapSort);

    // TestSort ("tests/medium_tests", "output/5heapsort.txt",
    //           0, 100000, 1000, 1, FiveHeapSort);

    // TestSort ("tests/medium_tests", "output/6heapsort.txt",
    //           0, 100000, 1000, 1, SixHeapSort);

    // TestSort ("tests/medium_tests", "output/7heapsort.txt",
    //           0, 100000, 1000, 1, SevenHeapSort);

    // TestSort ("tests/medium_tests", "output/8heapsort.txt",
    //           0, 100000, 1000, 1, EightHeapSort);

    // TestSort ("tests/medium_tests", "output/9heapsort.txt",
    //           0, 100000, 1000, 1, NineHeapSort);

    // TestSort ("tests/medium_tests", "output/10heapsort.txt",
    //           0, 100000, 1000, 1, TenHeapSort);

    TestSort ("tests/medium_tests", "output/mergerec.txt",
              0, 100000, 1000, 1, MergeRecursiveSort);

    TestSort ("tests/medium_tests", "output/mergeit.txt",
              0, 100000, 1000, 1, MergeIterativeSort);

    TestSort ("tests/medium_tests", "output/lomuto.txt",
              0, 100000, 1000, 1, QuickLomutoSort);

    TestSort ("tests/medium_tests", "output/hoare.txt",
              0, 100000, 1000, 1, QuickHoareSort);

    TestSort ("tests/medium_tests", "output/thick.txt",
              0, 100000, 1000, 1, QuickThickSort);

    TestSort ("tests/test_most_dublicates", "output/lomuto_dub.txt",
              0, 100000, 1000, 1, QuickLomutoSort);

    TestSort ("tests/test_most_duplicates", "output/hoare_dub.txt",
              0, 100000, 1000, 1, QuickHoareSort);

    TestSort ("tests/test_most_duplicates", "output/thick_dub.txt",
              0, 100000, 1000, 1, QuickThickSort);

    return 0;
}
