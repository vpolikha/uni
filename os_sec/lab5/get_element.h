#include <stdio.h>
#include <string.h>
#define ARR_SIZE 5
#define MAX_LENGTH 10
static char * array[ARR_SIZE] = {
    "Element 1",
    "Element 2",
    "Element 3",
    "Element 4",
    "Element 5",
};
// static char buf[MAX_LENGTH];
static __thread char buf[MAX_LENGTH]; /* Use thread-local storage */

char * getElement(int index)
{
    if (index >= ARR_SIZE) {
        fprintf(stderr, "Unknown element\n");
    } else {
        strncpy(buf, array[index], MAX_LENGTH - 1);
    }

    return buf;
}
