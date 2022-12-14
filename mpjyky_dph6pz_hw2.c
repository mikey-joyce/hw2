/*HW2 Authors:
Mikey Joyce
Devin Hackman*/

//libraries
#include <stdio.h>
#include <stdlib.h>
#include "input_error.h"

//prototypes
void combine(int *myList, int left, int mid, int right, int size);
void my_mergesort(int *myList, int left, int right, int size);
void swapElements(int *first, int *second);
int quick_partition(int *myList, int left, int right);
int quickselect(int *myList, int left, int right, int target);
void getPartitionSizes(int *myList, int pivot, int size, int *partitionSizes);
void mom_partitions(int *myList, int *small, int *large, int pivot, int size, int *partitionSizes);
int getXthElement(int *small, int *large, int *partitionSizes, int x);
int mom_select(int *myList, int node, int size);

int main(int argc, char **argv){
    //open file
    // needs a test for only one command line argument
    FILE *fPtr = fopen(argv[1], "r");
    if(argc > 2){
        exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
    }

    //determine if the file exists, exits file with 
    if (fPtr == NULL){
        exit(INPUT_FILE_FAILED_TO_OPEN);
        return 0;
    }

    int count = 0;  // Line counter (result)
    char temp;  //temp character

    //count amount of lines in file 
    for (temp = getc(fPtr); temp != EOF; temp = getc(fPtr)) if (temp == '\n') count = count + 1;
    //printf("Count: %d\n",count);
    if (0 == count) {
        exit(PARSING_ERROR_EMPTY_FILE);
    }
    
    rewind(fPtr);

    //obtain x
    int x=0;
    if(!fscanf(fPtr, "%d\n", &x)){
        exit(PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED);
    }

    //obtain the list
    int myList[count],i=0;

    // needs a test for an incorrect character
    while(!feof(fPtr)){
        if(fscanf(fPtr, "%d\n", &myList[i])){
            if(myList[i]<0){
                exit(PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED);
            }
            i++;
        }
        else{
            exit(PARSING_ERROR_INVALID_CHARACTER_ENCOUNTERED);
        }
    }

    //obtain 
    int size = sizeof (myList) / sizeof (myList[0]);
    //if there are less elements in myList than x then print -1
    if (size < x){
        printf("%d\n", -1); //answer
        return -1;
    }

    int pivot = mom_select(myList, x, size); // the pivot is the median of medians

    // gets the sizes of the small and the large partition and stores it in partitionSizes
    int partitionSizes[2];
    getPartitionSizes(myList, pivot, size, partitionSizes);
    int small[partitionSizes[0]], large[partitionSizes[1]];

    //fills the partitions with the proper elements
    mom_partitions(myList, small, large, pivot, size, partitionSizes);
    //returns result which is the xth smallest element
    int result = getXthElement(small, large, partitionSizes, x);
    
    printf("%d\n", result);//answer

    if(fclose(fPtr) != 0){
        exit(INPUT_FILE_FAILED_TO_CLOSE);
    }
    fPtr = NULL;

    return result;
}

/*
This function combines the two different sides of our array to be one fully
sorted array and marks the completion of our mergesort algorithm.
*/
void combine(int *myList, int left, int mid, int right, int size){
    int leftIndex, rightIndex, i;
    int temp[size];

    for(leftIndex = left, rightIndex = mid+1, i=left; leftIndex <= mid && rightIndex <= right; i++){
        if(myList[leftIndex] <= myList[rightIndex]){
            temp[i] = myList[leftIndex++];
        }
        else{
            temp[i] = myList[rightIndex++];
        }
    }

    while(leftIndex <= mid){
            temp[i++] = myList[leftIndex++];
    }

    while(rightIndex <= right){
        temp[i++] = myList[rightIndex++];
    }

    for(i = left; i <= right; i++){
        myList[i] = temp[i];
    }
}

/*
This function will sort a given array recursively from smallest to largest.
*/
void my_mergesort(int *myList, int left, int right, int size){
    if(left < right){
        int mid = (left+(right))/2;
        my_mergesort(myList, left, mid, size);
        my_mergesort(myList, mid+1, right, size);
        combine(myList, left, mid, right, size);
    }
}

/*
This function takes two array elements first and second and swaps them
*/
void swapElements(int *first, int *second){
    int temp = *first;
    *first = *second;
    *second = temp;
}

/*
This function returns a position for our quickselect algorithm 
*/
int quick_partition(int *myList, int left, int right){
    //sorts the list in an order to pick the local median
    int pivot = myList[right], position = left;

    int i = 0;

    for(i = left; i < right; i++){
        if(myList[i] < pivot){
            swapElements(&myList[position], &myList[i]);
            position++;
        }
    }

    swapElements(&myList[position], &myList[right]);
    return position;
}


/*
This function is used to return the median of each group of 5 that way
we can have an array of local medians.
*/
int quickselect(int *myList, int left, int right, int target){
    //Returns the median of each group of 5, aka the local median
    //node is the target position, which is the middle of the group of 5
    int position = quick_partition(myList, left, right); //this is the temp position

    if(position == target-1){
        return myList[position];
    }
    else if (target - 1 < position){
        return quickselect(myList, left, position-1, target);
    }
    else{
        return quickselect(myList, position+1, right, target);
    }
}

/*
This function determines the size of our mom_select partitions and stores it in the given
array called partitionSizes. The first element of partitionSizes is the size of the small
partition, which contains elements smaller than or equal to the pivot. The second element
of partitionSizes is the size of the large partition, which contains elements greater than
the pivot. 
*/
void getPartitionSizes(int *myList, int pivot, int size, int *partitionSizes){
    int smallIndex=0, largeIndex=0;

    int i=0;

    for(i=0; i < size; i++){
        if(myList[i] <= pivot){
            smallIndex++;
        }
        else{
            largeIndex++;
        }
    }

    partitionSizes[0] = smallIndex, partitionSizes[1] = largeIndex;
}

/*
This function takes empty partitions small and large and fills them using the given list myList
and the partitionSizes array. It uses the pivot point to determine to which array (small or large)
to put the myList element in.
*/
void mom_partitions(int *myList, int *small, int *large, int pivot, int size, int *partitionSizes){
    int smallIndex=0, largeIndex=0;

    int i=0;

    for(i=0; i < size; i++){
        if(myList[i] <= pivot){
            small[smallIndex] = myList[i];
            smallIndex++;
        }
        else{
            large[largeIndex] = myList[i];
            largeIndex++;
        }
    }
}

/*
This function takes our partitions and if x is less than or equal to the small partition
then it sorts the small partition and returns the xth smallest element. Otherwise it 
sorts the large partition and returns the xth smallest element. 
*/
int getXthElement(int *small, int *large, int *partitionSizes, int x){
    if(x <= partitionSizes[0]){
        my_mergesort(small, 0, partitionSizes[0]-1, partitionSizes[0]);
        return small[x-1];
    }
    else{
        my_mergesort(large, 0, partitionSizes[1]-1, partitionSizes[1]);
        return large[x-partitionSizes[0]-1];
    }

    return -1;
}

/*
This is the main median of medians algorithm based on the psuedocode given in class.
It takes an array, the xth element, and the size of the array and returns the median
of medians of the array myList. This will be used as our pivot point later.
*/
int mom_select(int *myList, int node, int size){
    if(size < 50){
        my_mergesort(myList, 0, size-1, size);
        return myList[node-1];
    }

    //group myList into groups of 5 elements each ignore leftovers
    //the nodes in this array represent the starting index in myList[] for each group
    //ex: group[0] is the index of the first group in myList and to access this node do myList[group[0]];
    int groups[size/5];

    int i = 0;

    for(i = 0; i < (size/5); i++){
        groups[i] = i*5;
    }

    int group_medians[size/5];
    for(i = 0; i < (size/5); i++){
        group_medians[i] = quickselect(myList, groups[i], groups[i]+4, groups[i]+3);
    }

    //size/10 is target for pivot and size/5 is the size of group_medians array
    int pivot = mom_select(group_medians, size/10, size/5);

    return pivot;
}