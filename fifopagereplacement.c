#include <stdio.h>
#define MAX 100

int isPageInMemory(int memory[],int num_frames,int page){
    for (int i=0;i<num_frames;i++) {
        if (memory[i]==page) {
            return 1; 
        }
    }
    return 0; 
}
void printMemory(int memory[],int num_frames){
    for(int i=0;i<num_frames;i++){
        if(memory[i] == -1) printf("- ");
        else printf("%d ",memory[i]);
    }
    printf("\n");
}
int main(){
    int num_frames,num_pages;
    int memory[MAX],pages[MAX];
    int front=0,rear=0; 
    int page_faults=0;
    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);
    printf("Enter the number of pages: ");
    scanf("%d", &num_pages);
    printf("Enter the page reference string: ");
    for(int i=0;i<num_pages;i++){
        scanf("%d",&pages[i]);
    }
    for(int i=0;i<num_frames;i++){
        memory[i]=-1;
    }
    for(int i=0;i<num_pages;i++){
        printf("Page %d -> ", pages[i]);
        if(!isPageInMemory(memory,num_frames,pages[i])) {
            memory[rear] = pages[i];
            rear = (rear + 1) % num_frames;
            page_faults++;
            printMemory(memory, num_frames);
        }else{
            printf("No page fault.\n");
        }
    }
    printf("\nTotal number of page faults: %d\n",page_faults);
    return 0;
}