# FIFO Page Replacement

## Objective

To implement the First-In-First-Out (FIFO) page replacement algorithm
in C and understand how pages are replaced when the memory frames are full.

## Concept

FIFO stands for **First-In-First-Out**.

The page that entered memory first will be removed first when a new page
needs to be loaded and all frames are occupied.

### Example

Reference string:

```text
1 2 3 4 1 2