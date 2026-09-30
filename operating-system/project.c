#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PAGES 100
#define MAX_FRAMES 100
#define MAX_REQUESTS 1000

int pageTable[MAX_PAGES];



int frameTable[MAX_FRAMES];

int logicalMemorySize;
int physicalMemorySize;
int pageSize;

int numberOfPages;
int numberOfFrames;

int fifoPointer = 0;


int lastUsed[MAX_PAGES];
int currentTime = 0;


int totalRequests = 0;
int pageFaults = 0;



void initializeTables()
{
    int i;

    for (i = 0; i < MAX_PAGES; i++)
    {
        pageTable[i] = -1;
        lastUsed[i] = -1;
    }

    for (i = 0; i < MAX_FRAMES; i++)
    {
        frameTable[i] = -1;
    }

    fifoPointer = 0;
    currentTime = 0;
    totalRequests = 0;
    pageFaults = 0;
}
void displayConfiguration()
{
    printf("\n============================================\n");
    printf("          MEMORY CONFIGURATION\n");
    printf("============================================\n");

    printf("Logical Memory Size  : %d bytes\n", logicalMemorySize);
    printf("Physical Memory Size : %d bytes\n", physicalMemorySize);
    printf("Page Size            : %d bytes\n", pageSize);
    printf("Number of Pages      : %d\n", numberOfPages);
    printf("Number of Frames     : %d\n", numberOfFrames);

    printf("============================================\n");
}
void configureMemory()
{
    printf("\n========== MEMORY CONFIGURATION ==========\n");

    printf("Enter Logical Memory Size (bytes): ");
    scanf("%d", &logicalMemorySize);

    printf("Enter Physical Memory Size (bytes): ");
    scanf("%d", &physicalMemorySize);

    printf("Enter Page Size (bytes): ");
    scanf("%d", &pageSize);

    if (logicalMemorySize <= 0 ||
        physicalMemorySize <= 0 ||
        pageSize <= 0)
    {
        printf("\nInvalid input! Values must be greater than 0.\n");
        return;
    }

    if (logicalMemorySize % pageSize != 0 ||
        physicalMemorySize % pageSize != 0)
    {
        printf("\nError: Memory sizes must be exactly divisible by page size.\n");
        return;
    }

    numberOfPages = logicalMemorySize / pageSize;
    numberOfFrames = physicalMemorySize / pageSize;

    if (numberOfPages > MAX_PAGES ||
        numberOfFrames > MAX_FRAMES)
    {
        printf("\nError: Maximum limit exceeded.\n");
        printf("Maximum Pages  : %d\n", MAX_PAGES);
        printf("Maximum Frames : %d\n", MAX_FRAMES);
        return;
    }

    initializeTables();

    printf("\nMemory configured successfully!\n");

    displayConfiguration();
}
int findFreeFrame()
{
    int i;

    for (i = 0; i < numberOfFrames; i++)
    {
        if (frameTable[i] == -1)
        {
            return i;
        }
    }

    return -1;
}

void allocatePages()
{
    int i;
    int frame;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    for (i = 0; i < numberOfPages; i++)
    {
        pageTable[i] = -1;
        lastUsed[i] = -1;
    }

    for (i = 0; i < numberOfFrames; i++)
    {
        frameTable[i] = -1;
    }
    for (i = 0; i < numberOfPages && i < numberOfFrames; i++)
    {
        frame = findFreeFrame();

        if (frame != -1)
        {
            pageTable[i] = frame;
            frameTable[frame] = i;
            lastUsed[i] = ++currentTime;
        }
    }

    fifoPointer = 0;

    printf("\nPages allocated successfully.\n");
}
void displayPageTable()
{
    int i;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\n============================================\n");
    printf("                 PAGE TABLE\n");
    printf("============================================\n");

    printf("+--------+----------+----------------+\n");
5    printf("| Page   | Frame    | Status         |\n");
    printf("+--------+----------+----------------+\n");

    for (i = 0; i < numberOfPages; i++)
    {
        if (pageTable[i] == -1)
        {
            printf("| %-6d | %-8s | Page Fault     |\n",
                   i, "-");
        }
        else
        {
            printf("| %-6d | %-8d | In Memory      |\n",
                   i, pageTable[i]);
        }
    }

    printf("+--------+----------+----------------+\n");
}
void displayFrameTable()
{
    int i;

    if (numberOfFrames <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\n============================================\n");
    printf("                 FRAME TABLE\n");
    printf("============================================\n");

    printf("+----------+----------+\n");
    printf("| Frame    | Page     |\n");
    printf("+----------+----------+\n");

    for (i = 0; i < numberOfFrames; i++)
    {
        if (frameTable[i] == -1)
        {
            printf("| %-8d | %-8s |\n", i, "Free");
        }
        else
        {
            printf("| %-8d | %-8d |\n", i, frameTable[i]);
        }
    }

    printf("+----------+----------+\n");
}

void translateAddress()
{
    int logicalAddress;
    int pageNumber;
    int offset;
    int frameNumber;
    int physicalAddress;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\nEnter Logical Address: ");
    scanf("%d", &logicalAddress);

    if (logicalAddress < 0 ||
        logicalAddress >= logicalMemorySize)
    {
        printf("\nInvalid logical address!\n");
        printf("Valid range: 0 to %d\n", logicalMemorySize - 1);
        return;
    }

    pageNumber = logicalAddress / pageSize;
    offset = logicalAddress % pageSize;

    totalRequests++;

    frameNumber = pageTable[pageNumber];

    printf("\n============================================\n");
    printf("         ADDRESS TRANSLATION\n");
    printf("============================================\n");

    printf("Logical Address : %d\n", logicalAddress);
    printf("Page Number     : %d\n", pageNumber);
    printf("Offset          : %d\n", offset);

    if (frameNumber == -1)
    {
        pageFaults++;

        printf("Frame Number    : Not Available\n");
        printf("Physical Address: Not Available\n");
        printf("\n*** PAGE FAULT ***\n");
        printf("Page %d is not currently in memory.\n",
               pageNumber);
    }
    else
    {
        physicalAddress =
            (frameNumber * pageSize) + offset;

        currentTime++;
        lastUsed[pageNumber] = currentTime;

        printf("Frame Number    : %d\n", frameNumber);
        printf("Physical Address: %d\n",
               physicalAddress);
    }

    printf("============================================\n");
}
int findFIFOFrame()
{
    int frame = fifoPointer;

    fifoPointer++;

    if (fifoPointer >= numberOfFrames)
    {
        fifoPointer = 0;
    }

    return frame;
}
void fifoReplacement()
{
    int logicalAddress;
    int pageNumber;
    int offset;
    int frame;
    int oldPage;
    int physicalAddress;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\nEnter Logical Address: ");
    scanf("%d", &logicalAddress);

    if (logicalAddress < 0 ||
        logicalAddress >= logicalMemorySize)
    {
        printf("\nInvalid logical address!\n");
        return;
    }

    pageNumber = logicalAddress / pageSize;
    offset = logicalAddress % pageSize;

    totalRequests++;
    if (pageTable[pageNumber] != -1)
    {
        frame = pageTable[pageNumber];

        physicalAddress =
            frame * pageSize + offset;

        printf("\nPage %d is already in memory.\n",
               pageNumber);

        printf("Frame Number    : %d\n", frame);
        printf("Physical Address: %d\n",
               physicalAddress);

        return;
    }
    pageFaults++;

    printf("\n*** PAGE FAULT ***\n");
    printf("Page %d is not in memory.\n", pageNumber);

    frame = findFreeFrame();
    if (frame == -1)
    {
        frame = findFIFOFrame();

        oldPage = frameTable[frame];

        if (oldPage != -1)
        {
            pageTable[oldPage] = -1;
        }

        printf("FIFO Replacement:\n");
        printf("Page %d removed from Frame %d.\n",
               oldPage, frame);
    }

    pageTable[pageNumber] = frame;
    frameTable[frame] = pageNumber;

    physicalAddress =
        frame * pageSize + offset;

    printf("Page %d loaded into Frame %d.\n",
           pageNumber, frame);

    printf("Physical Address: %d\n",
           physicalAddress);
}
int findLRUFrame()
{
    int i;
    int lruPage = -1;
    int lruTime = INT_MAX;
    int selectedFrame = -1;

    for (i = 0; i < numberOfFrames; i++)
    {
        int page = frameTable[i];

        if (page == -1)
        {
            return i;
        }

        if (lastUsed[page] < lruTime)
        {
            lruTime = lastUsed[page];
            lruPage = page;
            selectedFrame = i;
        }
    }

    return selectedFrame;
}
void lruReplacement()
{
    int logicalAddress;
    int pageNumber;
    int offset;
    int frame;
    int oldPage;
    int physicalAddress;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\nEnter Logical Address: ");
    scanf("%d", &logicalAddress);

    if (logicalAddress < 0 ||
        logicalAddress >= logicalMemorySize)
    {
        printf("\nInvalid logical address!\n");
        return;
    }

    pageNumber = logicalAddress / pageSize;
    offset = logicalAddress % pageSize;

    totalRequests++;

    if (pageTable[pageNumber] != -1)
    {
        frame = pageTable[pageNumber];

        currentTime++;
        lastUsed[pageNumber] = currentTime;

        physicalAddress =
            frame * pageSize + offset;

        printf("\nPage %d is already in memory.\n",
               pageNumber);

        printf("Frame Number    : %d\n",
               frame);

        printf("Physical Address: %d\n",
               physicalAddress);

        return;
    }
    pageFaults++;

    printf("\n*** PAGE FAULT ***\n");
    printf("Page %d is not in memory.\n",
           pageNumber);

    frame = findLRUFrame();

    oldPage = frameTable[frame];

    if (oldPage != -1)
    {
        pageTable[oldPage] = -1;

        printf("LRU Replacement:\n");
        printf("Page %d removed from Frame %d.\n",
               oldPage, frame);
    }

    pageTable[pageNumber] = frame;
    frameTable[frame] = pageNumber;

    currentTime++;
    lastUsed[pageNumber] = currentTime;

    physicalAddress =
        frame * pageSize + offset;

    printf("Page %d loaded into Frame %d.\n",
           pageNumber, frame);

    printf("Physical Address: %d\n",
           physicalAddress);
}
void translateMultiple()
{
    int count;
    int i;
    int logicalAddress;
    int pageNumber;
    int offset;
    int frameNumber;
    int physicalAddress;

    if (numberOfPages <= 0)
    {
        printf("\nPlease configure memory first.\n");
        return;
    }

    printf("\nHow many logical addresses? ");
    scanf("%d", &count);

    if (count <= 0 || count > MAX_REQUESTS)
    {
        printf("\nInvalid number of addresses.\n");
        return;
    }

    printf("\nEnter %d logical addresses:\n", count);

    for (i = 0; i < count; i++)
    {
        scanf("%d", &logicalAddress);

        printf("\n--------------------------------------------\n");

        if (logicalAddress < 0 ||
            logicalAddress >= logicalMemorySize)
        {
            printf("Logical Address: %d\n",
                   logicalAddress);
            printf("Invalid Address!\n");
            continue;
        }

        pageNumber = logicalAddress / pageSize;
        offset = logicalAddress % pageSize;

        totalRequests++;

        frameNumber = pageTable[pageNumber];

        printf("Logical Address: %d\n",
               logicalAddress);
        printf("Page Number    : %d\n",
               pageNumber);
        printf("Offset         : %d\n",
               offset);

        if (frameNumber == -1)
        {
            pageFaults++;

            printf("Frame Number   : Not Available\n");
            printf("Physical Address: PAGE FAULT\n");
        }
        else
        {
            physicalAddress =
                frameNumber * pageSize + offset;

            currentTime++;
            lastUsed[pageNumber] = currentTime;

            printf("Frame Number   : %d\n",
                   frameNumber);
            printf("Physical Address: %d\n",
                   physicalAddress);
        }
    }

    printf("--------------------------------------------\n");
}
void displayStatistics()
{
    float faultRate = 0.0;

    if (totalRequests > 0)
    {
        faultRate =
            ((float)pageFaults / totalRequests) * 100;
    }

    printf("\n============================================\n");
    printf("             MEMORY STATISTICS\n");
    printf("============================================\n");

    printf("Logical Memory Size : %d bytes\n",
           logicalMemorySize);

    printf("Physical Memory Size: %d bytes\n",
           physicalMemorySize);

    printf("Page Size           : %d bytes\n",
           pageSize);

    printf("Number of Pages     : %d\n",
           numberOfPages);

    printf("Number of Frames    : %d\n",
           numberOfFrames);

    printf("Total Requests      : %d\n",
           totalRequests);

    printf("Page Faults         : %d\n",
           pageFaults);

    printf("Page Fault Rate     : %.2f%%\n",
           faultRate);

    printf("============================================\n");
}
void displayMenu()
{
    printf("\n\n");
    printf("============================================\n");
    printf("       PAGING MEMORY MANAGEMENT SIMULATOR\n");
    printf("============================================\n");

    printf("1. Configure Memory\n");
    printf("2. Display Configuration\n");
    printf("3. Allocate Pages\n");
    printf("4. Display Page Table\n");
    printf("5. Display Frame Table\n");
    printf("6. Translate Logical Address\n");
    printf("7. Translate Multiple Addresses\n");
    printf("8. FIFO Page Replacement\n");
    printf("9. LRU Page Replacement\n");
    printf("10. Display Statistics\n");
    printf("0. Exit\n");

    printf("============================================\n");
    printf("Enter your choice: ");
}
int main()
{
    int choice;

    logicalMemorySize = 0;
    physicalMemorySize = 0;
    pageSize = 0;
    numberOfPages = 0;
    numberOfFrames = 0;

    initializeTables();

    do
    {
        displayMenu();
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                configureMemory();
                break;

            case 2:
                if (numberOfPages == 0)
                    printf("\nPlease configure memory first.\n");
                else
                    displayConfiguration();
                break;

            case 3:
                allocatePages();
                break;

            case 4:
                displayPageTable();
                break;

            case 5:
                displayFrameTable();
                break;

            case 6:
                translateAddress();
                break;

            case 7:
                translateMultiple();
                break;

            case 8:
                fifoReplacement();
                break;

            case 9:
                lruReplacement();
                break;

            case 10:
                if (numberOfPages == 0)
                    printf("\nPlease configure memory first.\n");
                else
                    displayStatistics();
                break;

            case 0:
                printf("\nExiting Paging Simulator...\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 0);

    return 0;
}
