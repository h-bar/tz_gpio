#ifndef _OPTEE_H
#define _OPTEE_H

#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#define TEE_NUM_PARAMS      4
#define TEE_SUCCESS         0
#define TEE_ERROR_NOT_IMPLEMENTED 1

typedef uint32_t TEE_Result;

typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEE_UUID;

typedef union {
	struct {
		void *buffer;
		uint32_t size;
	} memref;
	struct {
		uint32_t a;
		uint32_t b;
	} value;
} TEE_Param;

static void* core_mmu_get_va(int offset, uint32_t option) {
	int fd = open("/dev/mem",O_RDWR|O_SYNC);
	if(fd < 0) {
		printf("Can't open /dev/mem\n");
		return NULL;
	}
	return mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);
}


#endif /*_OPTEE_H*/