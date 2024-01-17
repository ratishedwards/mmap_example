#include<stdio.h>
#include<string.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>

void * get_shared_memory(size_t size);

int main(int argc, char *argv[]){

	int  SHARED_MMAP_SIZE = 2 * sizeof(int);
	int x = 10;
	int y = 10;

	void *shared_memory = get_shared_memory(SHARED_MMAP_SIZE);
	if ( shared_memory == MAP_FAILED ){
		exit(EXIT_FAILURE);
	}

	memcpy(shared_memory, (void *)&x, sizeof(int));	
	memcpy(((int *)shared_memory) + 1, (void *)&y, sizeof(int));	
	//copy parent message to shared_memory
	//memcpy(shared_memory, parent, sizeof(parent));

	int pid = fork();
	if ( pid == 0 ){
		int a = *(int *)shared_memory;
		int b = *((int *)shared_memory + 1);
		int sum = a + b;
		// copy sum back to shared_memory
		memcpy(shared_memory, (void *)&sum, sizeof(int));
		return 0;
	}

	waitpid(pid, NULL, 0);
	printf("Sum is : %d\n", *(int *)shared_memory);
	return 0;
}

void * get_shared_memory(size_t size){
	// set read write permissions and visibility
	int permissions = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, permissions, visibility, -1, 0);
}
