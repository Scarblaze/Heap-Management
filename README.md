# 🧠 Heap Management Using Buddy Allocation System (C Project)

This project is a simulation of a **heap memory management system** implemented in **C** using the **Buddy Allocation System**. It emulates memory allocation, deallocation, and block merging strategies similar to what happens in actual OS-level heap management.

---

## 💡 Overview

In this project, memory management is achieved by dividing a fixed-size memory pool into partitions based on powers of two (`2^i`) using a Buddy Allocation System.

Each memory block is represented by a structure `meta_block`, which contains:
- Size of the block
- Free/Allocated status
- Pointer to the next block

A global array of linked lists (`store[13]`) maintains free/allocated blocks for sizes ranging from `2^1` to `2^13`.

---

## ⚙️ Functionality

# 🧷 Initialize()
Initializes the store[] array with base blocks of sizes 2^1 to 2^13.

# 🔍 findrange(int k)
Determines the correct index in store[] corresponding to the size k.

# 🧠 Alloc(int bytes)
Allocates memory block of given size.

Returns a pointer to the block.

Uses linked list traversal to reuse or split space intelligently.

# 🧼 Free(void* ptr)
Frees the given block.

Marks it reusable and attempts to merge it with adjacent free buddies.

# 🔗 Merge(int index)
Merges adjacent free blocks in store[index] to form larger blocks.



