
#include <c6x.h>
#include <string.h>

/*Use the profile data buffer as a circular buffer*/
#define CIRCULAR_BUFFER 	1

/*data number must be power of 2 for easy wrap*/
#define PROFILE_DATA_NUM 	(16*1024*1024)

/*since the profile data buffer is huge, normally it is allocated in
big external memory. If it is in external memory, 
do not make this buffer cacheable because this is write only buffer, 
cache is not helpful, and it will cause cache conflict if it is cacheable*/
#pragma DATA_SECTION (uiaProfileData, "External_NonCache_Data")
unsigned int uiaProfileData[PROFILE_DATA_NUM];
unsigned int uiProfileDataIndex= 0;

/*call this function where you want to begin record,
recalling this function actually restarts the record*/
void function_hook_init()
{
	TSCL= 0; 	//enable TSC
	uiProfileDataIndex= 0;
	
	/*0xFFFFFFFF represents a invalid record*/
	memset(uiaProfileData, 0xFFFFFFFF, sizeof(uiaProfileData));
}

void entry_hook(void (* func_addr)())
{
	asm(" dint"); 	//disable interrupt to make sure every record is atomic
	if(uiProfileDataIndex<PROFILE_DATA_NUM-2)
	{
		uiaProfileData[uiProfileDataIndex++]= (unsigned int)func_addr;
		uiaProfileData[uiProfileDataIndex++]= TSCL;
	}
	asm(" rint"); 	//restore interrupt

#if CIRCULAR_BUFFER
	/*wrap data pointer*/
	uiProfileDataIndex &= (PROFILE_DATA_NUM-1); 
#endif

}

void exit_hook(void (* func_addr)())
{
	asm(" dint"); 	//disable interrupt to make sure every record is atomic
	if(uiProfileDataIndex<PROFILE_DATA_NUM-2)
	{
		/*Address always has last 2 bits == 0.  
		So, "| 1" to signal this is an exit record in the buffer. 
		makes it possible to distinguish enter against exit
		in the post processing tool*/
		uiaProfileData[uiProfileDataIndex++]= ((unsigned int)func_addr)|1;
		uiaProfileData[uiProfileDataIndex++]= TSCL;
	}
	asm(" rint"); 	//restore interrupt

#if CIRCULAR_BUFFER
	/*wrap data pointer*/
	uiProfileDataIndex &= (PROFILE_DATA_NUM-1); 
#endif

}

/*if multiple tasks are used, task switch must be recorded with
task switch hook feature of BIOS. Without task switch record,
the function executing when task switch happens can not be 
recognized correctly by post processing tool.
function hook should NOT be generated for this function*/
#pragma NO_HOOKS(task_switch_hook);
void task_switch_hook(unsigned int currentTask, 
	unsigned int nextTask)
{
	asm(" dint"); 	//disable interrupt to make sure every record is atomic
	if(uiProfileDataIndex<PROFILE_DATA_NUM-4)
	{
		/*0xFFFFFFFE is used to indentify this is a task switch record*/
		uiaProfileData[uiProfileDataIndex++]= 0xFFFFFFFE;
		uiaProfileData[uiProfileDataIndex++]= currentTask;
		uiaProfileData[uiProfileDataIndex++]= nextTask;
		uiaProfileData[uiProfileDataIndex++]= TSCL;
	}
	asm(" rint"); 	//restore interrupt

#if CIRCULAR_BUFFER
	/*wrap data pointer*/
	uiProfileDataIndex &= (PROFILE_DATA_NUM-1); 
#endif
}
