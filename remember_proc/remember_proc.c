/**
* Adapted by David Mandelbaum to be remember proc
* Time updated: 3/6/18 11:30 PM
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
//#include <asm-generic/uaccess.h>
#include <linux/uaccess.h> //https://acassis.wordpress.com/2011/09/21/implicit-declaration-of-function-copy_from_user/

MODULE_LICENSE("GPL");  /* Kernel needs this license. */

#define ENTRY_NAME "remember"
#define PERMS 0666
#define PARENT NULL

/* Function declarations */
ssize_t procfile_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos);

ssize_t procfile_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos);

//unsigned long copy_to_user(char __user *buf,  char* ret_buf[], int ret);
//unsigned long copy_from_user(char *page, const char __user *buf, size_t count);


/* Global variables go here */

char user_message[80];
int message_entered = 0;

/* This global structure is necessary to bind the regular file read and write 
 * operations to our special "read" and "write" functions instead. Don't
 * modify. (structs in C are like objects in other languages.)
*/ 
static struct file_operations remember_proc_ops = {
   .owner = THIS_MODULE,
   .read = procfile_read,
   .write = procfile_write,
};


/* This function is called to create the special proc file entry on 
 * module load.  This file is created as /proc/remember. */
int remember_proc_init(void) {

   proc_create_data(ENTRY_NAME, PERMS, NULL, &remember_proc_ops, NULL);
   
   /* This message will print in /var/log/syslog or on the first try. */
   printk("/proc/%s created\n", ENTRY_NAME);
   return 0;
}

/* This function is called when someone tries to READ from the file
 * /proc/remember. */
ssize_t procfile_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    
   /* Static variable?  What does this do inside a C function? Take a look 
    * at the accepted answer in the link below:
    * http://stackoverflow.com/questions/572547/what-does-static-mean-in-a-c-program
    */
   static int finished = 0;
   //int ret;
  // char ret_buf[80];

   /* Are we done reading? If so, we return 0 to indicate end-of-file */
   if (finished) {
	finished=0;
	return 0;
   }

   finished = 1;

   /* This message will print in /var/log/syslog or on the first try. */
   printk("/proc/%s read called.\n", ENTRY_NAME);


   /* Take the string "EMPTY\n" and put it in ret_buf if no current input saved.  
      Otherwise, return the saved message written to proc.
      Copy ret_buf into the user-space buffer called buf.  buf is what gets
    * displayed to the user when they read the file. */
   
   if(message_entered == 1)
   {
        //ret = sprintf(ret_buf,"%s", user_message);
   }
   else
   {
        strcpy(user_message, "EMPTY\n\0");
       // ret = sprintf(ret_buf,"%s", user_message);
   }
   printk("return_line is...");
  // printk("ret equals %d", ret);
   if(copy_to_user(buf, user_message, 80)) {
      printk("copy to user did not work");
      //ret = -EFAULT;  //failed, let's get out of here
   }

   /* Returning the number of characters returned to the reader. */
   return sizeof(user_message);
}

/* This function is called when someone tries to WRITE to the file
 * /proc/remember. */
ssize_t procfile_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    //char *page; /* don't touch */
    
    /* Allocating kernel memory, don't touch. */
    /*page = (char *) vmalloc(count);
    if (!page)
       return -ENOMEM;   */

    /* Copy the data from the user space.  Data is placed in page. */ 
    if (copy_from_user(user_message, buf, count)) {
       //vfree(page);
       return -EFAULT;
    }
    //strcpy(user_message,page);//https://stackoverflow.com/a/308712
   // strcat(page, "0\n");
    //strcpy(user_message, page);
    //strcat(user_message, "\n\0");
   
    
    
    /* Free the allocated memory, don't touch. */
    //vfree(page); 
    /* Now do something with the data, here we just print it */
    printk("User has sent message\n");
    message_entered = 1;
    //test
    //printk("/proc/%s write leaves string present as %s.\n", ENTRY_NAME, user_message);
    /* Return the number of bytes written to the file. */
    return count;
}

/* Called when module is unloaded.  Function removes the proc file and
 * prints a message to /var/log/syslog. */
void remember_proc_exit(void)
{
   remove_proc_entry(ENTRY_NAME, NULL);
   printk("Removing /proc/%s.\n", ENTRY_NAME);
}

/* Necessary module stuff.  Says init function is remember_proc_init and
 * exit function is remember_proc_exit. */
module_init(remember_proc_init);
module_exit(remember_proc_exit);

