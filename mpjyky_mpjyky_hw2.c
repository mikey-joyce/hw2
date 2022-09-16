/*HW2 Author:
Mikey Joyce*/

//libraries
#include <stdio.h>
#include "input_error.h"

//prototypes
int mom_select(int *myList, int node, int size);
void mergesort(int *myList, int left, int right, int size);
void combine(int *myList, int left, int mid, int right, int size);

int main(void){
    //open file
    FILE *fPtr = fopen("input_file1.txt", "r");

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
    //int x=0;
    //fscanf(fPtr, "%d\n", &x);

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
    int m = mom_select(myList, myList[0], size);
    printf("%d\n", m);

    printf("Sorted list?\n");
    for(int j = 0; j<i; j++){
        printf("%d\n", myList[j]);
    }

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

int mom_select(int *myList, int node, int size){
    if(size < 50){
        mergesort(myList, 0, size-1, size);
    }
    return size;
}