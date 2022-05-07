#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

int StartAddress=KERNEL_HEAP_START;
void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2022 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: Allocation using NEXTFIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details


	//TODO: [PROJECT 2022 - BONUS1] Implement a Kernel allocation strategy
	// Instead of the Next allocation/deallocation, implement
	// BEST FIT strategy
	// use "isKHeapPlacementStrategyBESTFIT() ..."
	// and "isKHeapPlacementStrategyNEXTFIT() ..."
	//functions to check the current strategy
	//change this "return" according to your answer


	int firstAddToSendBack=StartAddress;

	int totalFreeFrames = KERNEL_HEAP_MAX-KERNEL_HEAP_START / PAGE_SIZE;
	unsigned int totalSize = 4 * 1024 * 1024 ;
	int sizeReq=ROUNDUP(size,PAGE_SIZE);  //TO ROUND NUMBER TO 4 IF LESS THAN 4  , IF 5 THEREFORE ROUND TO 8 WHICH IS DIVISIBLE BY 4.

	int reqNoOfPages=sizeReq/PAGE_SIZE;





	for(int i=0 ; i<reqNoOfPages;i++)
	{
		if(size>totalFreeFrames*PAGE_SIZE)
		{
					return NULL;
		}
		struct  Frame_Info * frame_info;
		frame_info=NULL;
		int result=allocate_frame(&frame_info);	//Used to allocate a free frame from the free frame list AND TAKES a frame struct to fill it
		//if result = 0 then done successively
		if(result==E_NO_MEM)
		{
			cprintf("No enough memory for page itself!\n");
			return NULL;

		}
		result = map_frame(ptr_page_directory, frame_info, (void*) StartAddress, PERM_WRITEABLE);
		if (result == E_NO_MEM)
		{
			cprintf("No enough memory for page table!\n");
			//free the allocated frame
			free_frame(frame_info) ;
			return NULL;
		}
		StartAddress+=PAGE_SIZE;
	}

	totalFreeFrames --;
	return (void*) firstAddToSendBack;
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [2] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code
	panic("kfree() is not implemented yet...!!");

	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2022 - [3] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
	panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer

	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2022 - [4] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer



	unsigned int virtualAddress=virtual_address;

	//cprintf("va: %p \n",virtualAddress);


	//to get  directory INDEX pdx(va)
	uint32 page_directory_INDEX=PDX(virtualAddress);
	uint32 directoryEntry=ptr_page_directory[page_directory_INDEX];   /// 20 12  el 12 dool framenoOfPageTable
	uint32 frameNoOfPageTable =(directoryEntry>>12); //P

	//page table index //frame no in directory
	uint32 pageTableIndex=PTX(virtualAddress);
	uint32 *ptr_page_table=NULL;
	//if no page table exits create one , (1) else 0 to not create
	int status=get_page_table(ptr_page_directory, (void*) virtualAddress,&ptr_page_table);

	if(status==TABLE_NOT_EXIST)
	{
		return -1;
	}
	int PA;

	if(ptr_page_table != NULL)
	{
		//cprintf("page table isnot equal null ");
		uint32 pTableEntry=ptr_page_table[pageTableIndex];

		if((ptr_page_table[pageTableIndex]&PERM_PRESENT)==1)
		{
			//frame is found in ram .
			//int frameNO_Page=(int)(pTableEntry>>12) ;
			int offset= virtualAddress % PAGE_SIZE;
			PA= ((pTableEntry >>12) * PAGE_SIZE) + offset;
		//	cprintf("PA: %d \n",PA);
			return PA;
		}
		else
		{
			return -1;
		}
	}
	return -1;

}

