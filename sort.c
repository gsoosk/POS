#include "types.h"
#include "stat.h"
#include "fcntl.h" // for using file defines
#include "user.h" // for using from strlen


void insertionSort(char *arr[], int n) 
{ 
   int i, j; 
   char *key;
   for (i = 1; i < n; i++) 
   { 
       key = arr[i]; 
       j = i-1; 
       while (j >= 0 && atoi(arr[j]) > atoi(key)) 
       { 
           arr[j+1] = arr[j]; 
           j = j-1; 
       } 
       arr[j+1] = key; 
   } 
} 

int main(int argc, char *argv[]) 
{
    if(argc != 6){
        printf(2, "sort: enter 5 numbers please.\n");
        exit();
    }

    int i;
    char* nums[5];
    for(i = 0 ; i < 5 ; i++)
        nums[i] = argv[i+1];
    insertionSort(nums, 5);
    printf(2,"proccess id is %d \n", getpid());
    int fd;
    if((fd = open("sorted.txt", O_CREATE | O_WRONLY)) < 0){
      printf(2, "sort: cannot open file");
      exit();
    }
    for(i = 0 ; i < 5 ; i++)
    {
        if(write(fd, nums[i], strlen(nums[i])) != strlen(nums[i])) {
            printf(2, "sort: write error\n");
            exit();
        }
        if(write(fd, "\n", 1) != 1) {
            printf(2, "sort: write error\n");
            exit();
        }
    }

    exit();
}