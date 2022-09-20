/*HW2 Author:
Mikey Joyce*/

//libraries
#include <stdio.h>
#include "input_error.h"

//prototypes
int mom_select(int *myList, int node, int size);
void mergesort(int *myList, int left, int right, int size);
void combine(int *myList, int left, int mid, int right, int size);
int quickselect(int *myList, int left, int right, int target);
int quick_partition(int *myList, int left, int right);
void swapElements(int *first, int *second);

int main(void){
    //open file
    FILE *fPtr = fopen("input_file2.txt", "r");

    //determine if the file exists
    if (fPtr == NULL){
        printf("Could not open file\n");
        return 0;
    }

    int count = 0;  // Line counter (result)
    char temp;  //temp character

    //count amount of lines in file 
    for (temp = getc(fPtr); temp != EOF; temp = getc(fPtr)) if (temp == '\n') count = count + 1;
    
    rewind(fPtr);

    //obtain x
    int x=0;
    fscanf(fPtr, "%d\n", &x);

    //obtain the list
    int myList[count],i=0;
    while(!feof(fPtr)){
        fscanf(fPtr, "%d\n", &myList[i]);
        i++;
    }

    //used to test that we got the correct data set
    /*
    for(int j = 0; j<i; j++){
        printf("%d\n", myList[j]);
    }*/

    int size = sizeof (myList) / sizeof (myList[0]);
    int pivot = mom_select(myList, x, size); // the pivot is the median of medians
    printf("mom_select result: %d\n", pivot);

    //int final = quickselect(myList, 0, size, pivot);
    //printf("kth element: %d\n", final);

    /*printf("Sorted list?\n");
    for(int j = 0; j<i; j++){
        printf("%d\n", myList[j]);
    }*/

    fclose(fPtr);
    fPtr = NULL;

    return 0;
}

//Will need to do this for a command line argument of a file
/*int main(int argc, char **argv){
    FILE *fPtr = fopen(argv[1], "r");
}*/

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

void mergesort(int *myList, int left, int right, int size){
    if(left < right){
        int mid = (left+(right))/2;
        mergesort(myList, left, mid, size);
        mergesort(myList, mid+1, right, size);
        combine(myList, left, mid, right, size);
    }
}

void swapElements(int *first, int *second){
    int temp = *first;
    *first = *second;
    *second = temp;
}

int quick_partition(int *myList, int left, int right){
    //sorts the list in an order to pick the local median
    int pivot = myList[right], x = left;

    for(int i = left; i < right; i++){
        if(myList[i] < pivot){
            swapElements(&myList[x], &myList[i]);
            x++;
        }
    }

    swapElements(&myList[x], &myList[right]);
    return x;
}


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

int mom_select(int *myList, int node, int size){
    if(size < 50){
        mergesort(myList, 0, size-1, size);
        return myList[node-1];
    }

    //group myList into groups of 5 elements each ignore leftovers
    //the nodes in this array represent the starting index in myList[] for each group
    //ex: group[0] is the index of the first group in myList and to access this node do myList[group[0]];
    int groups[size/5];
    //printf("Groups\n");
    for(int i = 0; i < (size/5); i++){
        groups[i] = i*5;
        //printf("%d\n", groups[i]);
    }

    printf("Group medians:\n");
    int group_medians[size/5];
    for(int i = 0; i < (size/5); i++){
        group_medians[i] = quickselect(myList, groups[i], groups[i]+4, groups[i]+3);
        printf("%d\n", group_medians[i]);
    }

    //size/10 is target for pivot and size/5 is the size of group_medians array
    //printf("size/10: %d\n", size/10);
    int pivot = mom_select(group_medians, size/10, size/5);
    /*printf("Group medians after quicksort\n");
    for(int i = 0; i < (size/5); i++){
        printf("%d\n", group_medians[i]);
    }*/
    printf("Pivot: %d\n", pivot);

    return pivot;
}