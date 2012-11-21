/*
 * Title: A simple character device module for the Linux kernel 3.6.2
 * Class: CPSC 457 Fall 2012
 * Author: Mea Wang
 * Copyright (c) 2012 Mea Wang. All rights reserved.
 */
 
#include<linux/module.h>    // for printk()
#include<linux/init.h>      // for module_init(), module_exit()
#include <linux/fs.h>       // for register_chrdev(), unregister_chrdev()
#include <linux/slab.h>     // for kmemalloc(), kfree()
#include <asm/uaccess.h>    // for copy_from_user(), copy_to_user()

MODULE_LICENSE("GPL");

// The implementation of each file operation
int chardev_open (struct inode *inode,struct file *filep);
int chardev_release (struct inode *inode,struct file *filep);
ssize_t chardev_read (struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t chardev_write (struct file *filep,const char *buff,size_t count,loff_t *offp );

// Redefine the callback functions for the standard file operations.
struct file_operations chardev_fileops = {
    open: chardev_open,
    read: chardev_read,
    write: chardev_write,
    release: chardev_release,
};

static int init_chardevice(void);
static void shutdown_chardevice(void);

module_init(init_chardevice);           // driver initialization entry point
module_exit(shutdown_chardevice);       // driver exit entry point


/***************************************************************************
**********Implementation of the functions in this module *******************
***************************************************************************/

const int chunkSize = 20;       // The size of a chunk in the buffer
int bufferCapacity = 0;  // Initial buffer capacity, will grow as needed
char * chardev_buffer;    // The buffer for the device
int space;

// Initialize and register the character device in the kernel
static int init_chardevice(void)
{
    printk(KERN_INFO "Initializing character device...\n");
    
    chardev_buffer = (char*) kmalloc (bufferCapacity, GFP_KERNEL);
    // Register a major number for character devices
    if(register_chrdev(457, "char_device", &chardev_fileops))
        printk(KERN_ALERT "Failed to register character device.\n");
    else
        printk(KERN_INFO "Character device registered successfully.\n");
    
    return 0;
}

// The module_cleanup function: remove the driver from the kernel
static void shutdown_chardevice(void)
{
    // Releases the major number
    unregister_chrdev(457,"char_device");
    kfree(chardev_buffer);
    printk(KERN_INFO "Character device unregistered successfully.\n");

    return;
}

// This function is invoked when 'open' system call is performed on the device file
int chardev_open(struct inode *inode, struct file *filep)
{    
    // Extract the file access mode bits.
    int mode = filep->f_flags & O_ACCMODE;
    
    // Open the file for writing (O_WRONLY -- write only)     
    if (mode == O_WRONLY)
    {
        // The character sequence is truncated for each new open
        kfree(chardev_buffer);          // truncate the buffer
        chardev_buffer = (char*) kmalloc (chunkSize, GFP_KERNEL);
        bufferCapacity = chunkSize;
        space = chunkSize;
        printk(KERN_INFO "Character device truncated for writing.\n");
        return 0;
    }

    // Open the file for read only (O_RDONLY -- read only)     
    else if (mode == O_RDONLY)
    {
        printk(KERN_INFO "Character device opened for reading.\n");
        return 0;
    }
    
    // We do not support any other mode
    else
    {
        printk(KERN_ALERT "Character device: Unknown open mode (in open()).\n");
        return -1;
    }
}

// This function is invoked when the 'close' system call is performed on the device file
int chardev_release(struct inode *inode, struct file *filep)
{
    printk(KERN_INFO "Character device closed.\n");
    return 0;
}

ssize_t chardev_read(struct file *filep, char *buff, size_t offset, loff_t *offp )
{
    int numBytes = chunkSize;
    
    // Copy from kernel space buffer to user space
    if (copy_to_user(buff, chardev_buffer, numBytes) != 0)
    {
        printk(KERN_ALERT "Character device: userspace copy failed!\n");
        numBytes = 0;
    }
    else
    {
        printk(KERN_INFO "Character device: userspace copy successful.\n");
    }

    return numBytes;
}

ssize_t chardev_write(struct file *filep,const char *buff,size_t count,loff_t *offp )
{
	char * temp_buffer;
	int space;

	if(space == 0){
		temp_buffer = (char*) kmalloc (bufferCapacity + chunkSize, GFP_KERNEL);
		memcpy(temp_buffer, chardev_buffer, bufferCapacity);
		bufferCapacity += chunkSize;
	}
    // Copy from user space buffer to kernel space chardev_buffer
    if (copy_from_user(chardev_buffer, buff, count) != 0 )
    {
        printk(KERN_ALERT "Character device: kernel copy failed!\n" );
        count = 0;
    }
    else
    {
    	
        printk(KERN_INFO "Character device: kernel copy successful.\n" );
    }
    
    return count;
}

