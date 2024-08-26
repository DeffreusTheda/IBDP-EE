/*
 * Taken from https://learn.arm.com/learning-paths/smartphones-and-mobile/mte/mte/
 * Memory Tagging Extension (MTE) example for Linux
 *
 * Compile with gcc and use -march=armv8.5-a+memtag
 *    gcc gadget.c -o gadget -march=armv8.5-a+memtag
 *
 * Compilation should be done on a recent Arm Linux machine for the .h files to include MTE support.
 * See https://github.com/DeffreusTheda/IBDP-External-Assessment?tab=readme-ov-file#testing-the-gadget-linux.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <time.h>

/*
 * Insert a random logical tag into the given pointer.
 * IRG instruction.
 */
#define insert_random_tag(ptr) ({                     \
        uint64_t __val;                               \
        asm("irg %0, %1" : "=r" (__val) : "r" (ptr)); \
        __val;                                        \
})

/*
 * Set the allocation tag on the destination address.
 * STG instruction.
 */
#define set_tag(tagged_addr) do {                                      \
        asm volatile("stg %0, [%0]" : : "r" (tagged_addr) : "memory"); \
} while (0)

// Function to measure time
unsigned long long rdtsc() {
  unsigned long long ret;
  __asm__ volatile ("mrs %0, cntvct_el0" : "=r" (ret));
  return ret;
}

// TIKTAG-v1 style gadget
void tiktag_gadget(unsigned char *ptr, size_t secret_index) {
  unsigned long long start, end;
  unsigned char temp;

  // Flush the cache line containing ptr[secret_index]
  __builtin___clear_cache((char*)&ptr[secret_index], (char*)&ptr[secret_index + 1]);

  // Speculatively access ptr[secret_index]
  start = rdtsc();
  temp = ptr[secret_index];
  end = rdtsc();

  // Use the value to prevent optimization
  if (temp) {
    asm volatile("" : : "r" (temp));
  }

  // Measure timing
  unsigned long long time = end - start;
  printf("Access time: %llu cycles\n", time);
}

int main(void) {
  unsigned char *ptr; // pointer to memory for MTE demonstration

  /*
   * Use the architecture dependent information about the processor
   * from getauxval() to check if MTE is available.
   */
  if ( !((getauxval(AT_HWCAP2)) & HWCAP2_MTE) ) {
    printf("MTE is not supported\n");
    return EXIT_FAILURE;
  }
  printf("MTE is supported\n");

  /*
   * Enable MTE with synchronous checking
   */
  if ( prctl(PR_SET_TAGGED_ADDR_CTRL,
             PR_TAGGED_ADDR_ENABLE | PR_MTE_TCF_SYNC | (0xfffe << PR_MTE_TAG_SHIFT),
             0, 0, 0) ) {
    perror("prctl() failed");
    return EXIT_FAILURE;
  }

  // Allocate a larger memory area
  size_t page_size = sysconf(_SC_PAGESIZE);
  ptr = mmap(NULL, page_size * 2, PROT_READ | PROT_WRITE | PROT_MTE,
             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap() failed");
    return EXIT_FAILURE;
  }

  /*
   * Print the pointer value with the default tag (expecting 0) */
  printf("pointer is %p\n", ptr);

  /*
   * Write the first 2 bytes of the memory with the default tag
   */
  ptr[0] = 0x36;
  ptr[1] = 0x39;

  /*
   * Read back to confirm the writes
   */
  printf("ptr[0] = 0x%hhx ptr[1] = 0x%hhx\n", ptr[0], ptr[1]);

  /*
   * Generate a random tag and store it for the address (IRG instruction)
   */
  ptr = (unsigned char *) insert_random_tag(ptr);

  /*
   * Set the key on the pointer to match the lock on the memory  (STG instruction)
   */
  set_tag(ptr);

  /*
   * Print the pointer value with the new tag
   */
  printf("pointer is now %p\n", ptr);

  /*
   * Write the first 2 bytes of the memory again, with the new tag
   */
  ptr[0] = 0x36;
  ptr[1] = 0x39;

  /*
   * Read back to confirm the writes
   */
  printf("ptr[0] = 0x%hhx ptr[1] = 0x%hhx\n", ptr[0], ptr[1]);

  // Create a secret value (simulating the MTE tag)
  unsigned char secret = 0x45;
  size_t secret_index = page_size + 0x10;  // Place secret in the second page
  ptr[secret_index] = secret;

  printf("Attempting TIKTAG-v1 style attack...\n");

  // Perform multiple TIKTAG-v1 style accesses
  for ( int i = 0; i < 1000; i++ ) {
    tiktag_gadget(ptr, secret_index);
  }

  /*
   * Write to memory beyond the 16 byte granule (offsest 0x10)
   * MTE should generate an exception
   * If the offset is less than 0x10 no SIGSEGV will occur.
   */
  printf("Expecting SIGSEGV...\n");
  ptr[0x10] = 0x55;

  /*
   * Program only reaches this if no SIGSEGV occurs
   */
  printf("...no SIGSEGV was received\n");

  return EXIT_SUCCESS;
}
