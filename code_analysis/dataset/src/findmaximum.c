#include <stdio.h>

#define ARRAY_SIZE 10

// Function to calculate the average of array elements
double calculateAverage(int arr[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

// Function to find the maximum number in the array
int findMaximum(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main() {
    int numbers[ARRAY_SIZE] = {10, 20, 35, 45, 50, 60, 70, 80, 90, 100};
    double average = calculateAverage(numbers, ARRAY_SIZE);
    int maximum = findMaximum(numbers, ARRAY_SIZE);

    printf("Average of array elements: %.2f\n", average);
    printf("Maximum number in array: %d\n", maximum);

    return 0;
}
