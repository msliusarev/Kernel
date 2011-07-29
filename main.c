#include <system.h>
#include <multiboot.h>

/* You will need to code these up yourself! */
unsigned char* memcpy(unsigned char* dest, const unsigned char* src, int count)
{
	/* Add code here to copy 'count' bytes of data from 'src' to
	 * 'dest', finally return 'dest' */
	int i;
	for (i = 0; i < count; i++)
		dest[i] = src[i];
	return dest;
}

unsigned char* memset(unsigned char* dest, unsigned char val, int count)
{
	/* Add code here to set 'count' bytes in 'dest' to 'val'.
	 * Again, return 'dest' */
	int i;
	for (i = 0; i < count; i++)
		dest[i] = val;
	return dest;
}

unsigned short* memsetw(unsigned short* dest, unsigned short val, int count)
{
	/* Same as above, but this time, we're working with a 16-bit
	 * 'val' and dest pointer.  Your code can be an exact copy of
	 * the above, provided that your local variables if any, are
	 * unsigned short */
	int i;
	for (i = 0; i < count; i++)
		dest[i] = val;
	return dest;
}

/* We will use this later on for reading from the I/O ports to get data
 * from devices such as the keyboard.  We are using what is called
 * 'inline assembly' in these routines to actually do the work */
unsigned char inportb(unsigned short _port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/* We will use this to write to I/O ports to send bytes to devices.  This
 * will be used in the next tutorial for changing the textmode cursor
 * position.  Again, we use some inline assembly for the stuff that simply
 * cannot be done in C */
void outportb(unsigned short _port, unsigned char _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/* This is a very simple main() function.  All it does is sit in an
 * infinite loop.  This will be like our 'idle' loop */
void _main(struct multiboot_info* mbt, unsigned int magic)
{
	/* Setup the very core components of the kernel / CPU operation */
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	
	/* Enable IRQs */
	__asm__ __volatile__ ("sti");

	/* Install and handle various devices in the system */
	timer_install();
	kb_install();

	/* You would add commands after here */
	init_video();
	puts("Hello World Again!\n");
	mem_install(mbt, magic);

	/* ...and leave this loop in.  There is an endless loop in
	 * 'start.asm' also, if you accidently delete this next line */
	for (;;);
}

