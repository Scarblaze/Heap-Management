//Vinit More
//BT23CSE083
//R4
#include <stdio.h>
#include <stddef.h> // these are standard libraries declared in header file
#include <math.h>

#define MAX_SIZE 8392
char memory[16416];

typedef struct meta_block
{
    size_t size;
    int isFree;
    struct meta_block *next;
} meta_block;

meta_block *store[13];

void Initialize()
{
    meta_block *temp = (void *)memory;
    temp->size = 0;
    temp->isFree = 1;
    temp->next = NULL;
    store[0] = temp;

    int add = 2;
    for (int i = 1; i < 13; i++)
    {
        temp = (meta_block *)((char *)store[i - 1] + add + sizeof(meta_block));
        store[i] = temp;
        temp->size = 0;
        temp->isFree = 1;
        temp->next = NULL;
        add = add * 2;
    }
}

int findrange(int k)
{
    int index = 0;

    // Check if k is a power of 2
    if ((k & (k - 1)) == 0) { 
        index--;  // Decrement if k is exactly a power of 2
    }

    // Calculate log2(k)
    while (k > 1) {
        index++;
        k = k / 2;
    }

    return index;
}

void *Alloc(int bytes) {
    if (bytes > MAX_SIZE) {
        printf("Out of range\n");
        return NULL;
    }
    
    meta_block *retVal = NULL;
    int done = 0;
    int index = findrange(bytes);

    while (!done && index < 13) {
        // Check if store[index] exists before dereferencing
        if (store[index] && store[index]->isFree == 1 && store[index]->next == NULL) {
            store[index]->isFree = 0;
            store[index]->size = bytes;
            retVal = store[index];
            done = 1;
        } 
        else if (store[index] && store[index]->isFree == 1 && store[index]->size >= bytes) {
            store[index]->isFree = 0;
            store[index]->size = bytes;
            retVal = store[index];
            done = 1;
        } 
        else if (store[index] && (store[index]->size + bytes) < pow(2, index + 1)) {
            meta_block *t = store[index];
            int space = t->size + sizeof(meta_block);
            while (t->next != NULL && !done) {
                if (t->isFree && t->size >= bytes) {
                    t->size = bytes;
                    t->isFree = 0;
                    retVal = t;
                    done = 1;
                }
                t = t->next;
                space += t ? (t->size + sizeof(meta_block)) : 0;
            }

            if (!done && (space + bytes + sizeof(meta_block)) < pow(2, index + 1)) {
                meta_block *temp = (meta_block *)((char *)t + t->size + sizeof(meta_block));
                temp->size = bytes;
                temp->isFree = 0;
                temp->next = NULL;
                t->next = temp;
                retVal = temp;
                done = 1;
            }
        } else {
            index++;
        }
    }

    if (!done) {
        printf("Your memory is not sufficient\n");
    } else {
        printf("Memory allocated successfully for %d size\n", bytes);
    }

    return (void *)retVal;
}

int mergeindex(void *ptr)
{
    int index = 0;
    int found = 0;
    int i = 0;
    
    while (!found && i < 12)  // Ensure it does not go out of bounds
    {
        if ((char *)store[i] <= (char *)ptr && (char *)ptr < (char *)store[i + 1])
        {
            index = i;
            found = 1;
        }
        i++;
    }

    return index;
}

void Merge(int index)
{
    if (store[index] && store[index]->isFree && store[index]->next != NULL && store[index]->next->isFree)
    {
        store[index]->size += store[index]->next->size;
        store[index]->next = store[index]->next->next;
    }

    meta_block *ptr = store[index] ? store[index]->next : NULL;

    while (ptr != NULL)
    {
        if (ptr->isFree && ptr->next != NULL && ptr->next->isFree)
        {
            ptr->size += ptr->next->size;
            ptr->next = ptr->next->next;
        }
        else
        {
            ptr = ptr->next;
        }
    }
}

void Free(void* ptr)
{
    if (((void *)memory <= ptr) && (ptr <= (void *)(memory + 16416)))
    {
        meta_block *temp = (meta_block *)ptr;
        temp->isFree = 1;
        printf("Deallocated successfully\n");

        int index = mergeindex(ptr);
        Merge(index);
    }
    else 
    {
        printf("Please provide a proper pointer allocated by Alloc\n");
    }
}

int main() {
    Initialize();

    printf("\n*-*-* MEMORY ALLOCATION *-*-*\n");
    void* ptr1 = Alloc(2000);
    void* ptr2 = Alloc(4000);
    void* ptr3 = Alloc(2000);

    printf("\n*-*-* MEMORY DEALLOCATION *-*-*\n");
    Free(ptr2);

    printf("\n*-*-* MEMORY ALLOCATION AFTER FREE *-*-*\n");
    void* ptr4 = Alloc(3000);  // Should fit in freed block

    return 0;
}