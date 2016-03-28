#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/interrupt.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/gpio.h>
#include <linux/jiffies.h> 
#include <linux/time.h>

int irq1;
int irq2;
int btncounter1=0;
int btncounter2=0;
MODULE_LICENSE("Dual BSD/GPL");
int brightness = 1023;
/* Declaration of memory.c functions */
static int mygpio_open(struct inode *inode, struct file *filp);
static int mygpio_release(struct inode *inode, struct file *filp);

static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

static void mygpio_exit(void);
static int mygpio_init(void);

int delay;
static void my_function(unsigned long);

struct timeval start, end;
struct timeval start1, end1;

/* Structure that declares the usual file */
/* access functions */
struct file_operations mygpio_fops = {
	read: mygpio_read,
	write: mygpio_write,
	open: mygpio_open,
	release: mygpio_release
};

/* Declaration of the init and exit functions */
module_init(mygpio_init);
module_exit(mygpio_exit);

static unsigned capacity = 128;
static unsigned bite = 128;
module_param(capacity, uint, S_IRUGO);
module_param(bite, uint, S_IRUGO);

/* Global variables of the driver */
/* Major number */
static int mygpio_major = 62;

/* Buffer to store data */
static char *mygpio_buffer;
/* length of the current message */
static int mygpio_len;
/////////////////////////////////////////////////////////////
struct timer_list my_timer;
struct timer_list timer;
int counttime=15;
int counterperiod = 4;
int countervalue = 15;
char counterdirection[5];
char counterstate[8];
int temp;
unsigned LED[4]= {16,31, 29, 28};//{28, 29, 16, 31};
unsigned btn[3]= {31,30,118};
int x;// 0 is running, 1 is stopped
//unsigned btn1=0;// 0 is down, 1 is up
/*unsigned LED0 28;//LSB
unsigned LED1 29;
unsigned LED2 16;
unsigned LED3 31;//MSB*/
int diff;
int diff2;
int delay;
int flag=1;
int flag1=1;
int distance;
int distance2;
int highfirst=0;
int highfirst2=0;
int dflag=0;
int dflag1=0;
int check1=0;
int check2=0;
int check3=0;
int check01=0;
int check02=0;
int check03=0;
/*irqreturn_t gpio_irq1(int irq, void *dev_id, struct pt_regs *regs)
{
	//no debounce, active for both edgesiq3
	iq3flag++;
	
	      
	
       
	
	return IRQ_HANDLED;
}*/

irqreturn_t gpio_irq1(int irq, void *dev_id, struct pt_regs *regs)
{       // printk("irq2kaflag=%d\n",iq3flag);
	//iq3flag++;
	
        if(highfirst==0){
	  //printk("zero\n");
	  highfirst=1;
	return IRQ_HANDLED;
	}
	flag++;
	if(flag%2==0)
	{
	 do_gettimeofday(&start);
	}else
	{
	do_gettimeofday(&end);
	diff=(end.tv_usec-start.tv_usec);
	//printk("diff=%d\n",diff/147);
	}
	if(diff>=30000)
	{
	 highfirst=0;
	 diff=0;
	 
	}
	dflag++;
	if (dflag==5)
	{ distance=diff/147;
	dflag=0;
	printk("%d\n",distance);
	if(distance>=7 && distance<18)
	{printk("line1");
	  check1++;
	  check2=0;
	  check3=0;
	  if(check1>=3){
	   PWM_PWDUTY0=100;
	  }
	}else if(distance>=18 && distance<37)
	{printk("line2");
	  check2++;
	 check1=0;
	 check3=0;
	if(check2>=3){
	  PWM_PWDUTY0=75;
	 }
	}else if(distance>=37 && distance<=50)
	{printk("line3");
	  check3++;
	 check2=0;
	 check1=0;
	 if(check3>=3){
	  PWM_PWDUTY0=50;
	 }
	}else{
	  PWM_PWDUTY0=0;
	  
	}
	
	}	  
	return IRQ_HANDLED;
}
  irqreturn_t gpio_irq2(int irq, void *dev_id, struct pt_regs *regs)
{       // printk("irq2kaflag=%d\n",iq3flag);
	//iq3flag++;
	
        if(highfirst2==0){
	  //printk("zero\n");
	  highfirst2=1;
	return IRQ_HANDLED;
	}
	flag1++;
	if(flag1%2==0)
	{
	 do_gettimeofday(&start1);
	}else
	{
	do_gettimeofday(&end1);
	diff2=end1.tv_usec-start1.tv_usec;
	//printk("diff=%d\n",diff/147);
	}
	if(diff2>=35000)
	{
	 highfirst2=0;
	 diff2=0;
	 
	}
	dflag1++;
	if (dflag1==5)
	{ distance2=diff2/147;
	dflag1=0;
	printk("\t \t %d\n",distance2/147);
	if(distance2>=7 && distance2<18)
	{
	  check01++;
	  check02=0;
	  check03=0;
	  if(check1>=3){
	  
	    PWM_PWDUTY1=100;
	  }
	}else if(distance2>=18 && distance2<37)
	{check02++;
	 check01=0;
	 check03=0;
	if(check02>=3){
	  PWM_PWDUTY1=75;
	 }
	}else if(distance2>=37 && distance2<=50)
	{check03++;
	 check02=0;
	 check01=0;
	 if(check03>=3){
	  PWM_PWDUTY1=50;
	 }
	}else{
	  PWM_PWDUTY1=0;
	  
	}
	
	}
	
	return IRQ_HANDLED;
}
//irqreturn_t gpio_irq3(int irq, void *dev_id, struct pt_regs *regs)
//{       // printk("irq2kaflag=%d\n",iq3flag);
	//iq3flag++;diff=88

	//flag++;
	//flag++;
	
	
	
//	printk("%lld\n",(jiffies-th)/HZ*1000000);
	

	
        
		
	//x= pxa_gpio_get_value(btn[0]);
	//printk("%d\n",x);
	/*int th, tl;
        if(){
     	
       th=jiffies;
	}
	else{
	  tl=jiffies;
	  diff = tl - th;
	  printk("Pulsewidth = %lu",jiffies_to_usecs(diff));
	}
	 //if(iq3flag==1){
	 printk("Button IRQ2\n");//no debounce, active for both edges
	// btncounter2++;
       // if(btncounter2%2!=0)
//			    {counttime+=1;
//	}else{counttime-=1;
//	      }
//	 }
//	  else{
//	 btncounter1--;
//	 timeflag=0;*/

	 
       
       
//	return IRQ_HANDLED;
//}
/*irqreturn_t gpio_irq3(int irq, void *dev_id, struct pt_regs *regs)
{
	//printk("\nButton IRQ3\n");
		switch (brightness)
	{
	case 1023: {brightness=10; break;}
	case 10 :  {brightness=200;  break;}
	case 200:  {brightness=1023; break;}
	}

	return IRQ_HANDLED;
}

/////////////////////////////////////////////////////////////
static void my_function(unsigned long data)
 
{       int x;
         //printk("myfunctiokaflagatbegin=%d\n",iq3flag);
       for(x=0;x<172;x++)

       {
       }
    //  printk("countime=%d\n",counttime);
        iq3flag=0;
	int i;
	//printk("btncnter 2:%d\n",btncounter2);
//if button is not pressed, count down, running
	if(btncounter1%2!=0&&btncounter2%2==0)
	{	
		sprintf(counterdirection, "Down");
		sprintf(counterstate, "Run");
                if(timeflag==0){

		  counttime=0;
		}
		temp = counttime;
		
		for(i = 0;temp>=0 && i<4; i++)
		{
			if (i == 0)
			{	if (counttime%2)
				PWM_PWDUTY1=brightness;
				else
				PWM_PWDUTY1 = 0;
				
			}
			else
			
			pxa_gpio_set_value(LED[i],temp%2);
			
			//LED[i]= temp%2;
			temp = temp/2;
		}
		if(counttime>0){
			counttime -= 1;
		}
		else {
			counttime = 15;
		}
		
		my_timer.expires = jiffies + (counterperiod*HZ)/4;
		add_timer(&my_timer);	
	}
//if button1 is pressed, count up, running
	else if(btncounter1%2!=0&&btncounter2%2!=0)
	{
		int i;
		sprintf(counterdirection , "Up");
		sprintf(counterstate, "Run");

	         if(timeflag==0){
                    counttime=0;
		   counttime=-1;
		}
		temp = counttime+1;
		
		for(i = 0;temp>=0 && i<4; i++)
		{
			if (i == 0)
			{	if (counttime%2)
				PWM_PWDUTY1=brightness;
				else
				PWM_PWDUTY1 = 0;
				
			}

			else
			
			pxa_gpio_set_value(LED[i],temp%2);
			//LED[i]= temp%2;
			temp = temp/2;
		}
		if(counttime < 15){
			counttime += 1;
		}
		else {
			counttime = 0;
		}
		my_timer.expires = jiffies + (counterperiod*HZ)/4;
		add_timer(&my_timer);
	      
	}
//if button0 is pressed, count down, stopped
	else
	{
		sprintf(counterdirection, "Down");
		sprintf(counterstate, "Hold");
		temp = counttime;
		if (counttime%2)
				PWM_PWDUTY1=brightness;
				else
				PWM_PWDUTY1 = 0;
				
		
		my_timer.expires = jiffies + (counterperiod*HZ)/4;
		  add_timer(&my_timer);
		//add_timer(&my_timer);
	/*	int i;
		for(i = 0;temp>=0 && i<4; i++)
		{
			pxa_gpio_set_value(LED[i], temp%2);
			//LED[i]= temp%2;
			temp = temp/2;
		}
		if(counttime>0){
			counttime -= 1;
		}
		else {
			counttime = 15;
		}	
		
	}
//if both are pressed, count up, stopped
	/*else if(btn1 != 0  && btn0 != 0)
	{
		sprintf(counterdirection , "up");
		sprintf(counterstate, "stopped");
		temp = counttime;
	/*	int i;
		for(i = 0;temp>=0 && i<4; i++)
		{
			pxa_gpio_set_value(LED[i], temp%2);
			//LED[i]= temp%2;
			temp = temp/2;
		}
		if(counttime < 15){
			counttime += 1;
		}
		else {
			counttime = 0;
		}
		
//} 	//init_timer(&my_timer);	
	//my_timer.expires = jiffies + HZ;// + j*HZ;
	//my_timer.function = my_function; 
	//my_timer.data = mygpio_buffer;

  timeflag=1;
  //printk("myfunctiokaflagatend=%d\n",iq3flag);
		
}
uct timeval start, end;
///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void timer1_routine(unsigned long data){
 // printk("line6");
  if(headerflag==1)
  {
    printk("Value Speed State Direction Brightness\n");
    headerflag=0;
  }
  char speed; 
  char brightpwr; 
  if(counterperiod==1||counterperiod==2)
  {
    speed='H';
  }
  else if( counterperiod==3)
  {
    speed='M';
  }
  else
  {
    speed='L';
  }
 if(brightness==1023)
 {brightpwr='H';
  }
 else if(brightness==200)
 {
  brightpwr='M';
 }
 else
{
 brightpwr='L';
}
  
    //printk("Value Speed State Direction Brightness");
      printk("%5d %5c %5s %9s %10c \n",counttime,speed,counterstate,counterdirection,brightpwr);
      timer.expires = jiffies + delay*HZ;// + j*HZ;
      add_timer(&timer);

}*/

static int mygpio_init(void)
{
	int result;

	/* Registering device */

	result = register_chrdev(mygpio_major, "nirmit_mygpio", &mygpio_fops);
	if (result < 0)
	{
		printk(KERN_ALERT
			"mygpio: cannot obtain major number %d\n", mygpio_major);
		return result;
	}

	/* Allocating mygpio for the buffer */
	mygpio_buffer = kmalloc(capacity, GFP_KERNEL); 
	memset(mygpio_buffer,0,capacity);
	if (!mygpio_buffer)
	{ 
		printk(KERN_ALERT "Insufficient kernel memory\n"); 
		result = -ENOMEM;
		goto fail; 
	} 
	
	mygpio_len = 0;

	//printk(KERN_ALERT "Inserting mygpio module\n"); 

	//gpio_direction_output(LED[0],1);
	//gpio_direction_output(LED[1],1);
	//gpio_direction_output(LED[2],1);
	//gpio_direction_output(LED[3],1);
	gpio_direction_input(btn[0]);
	gpio_direction_input(btn[1]);
	int irq1 = IRQ_GPIO(btn[0]);
	int irq2 = IRQ_GPIO(btn[1]);
	//int irq3 = IRQ_GPIO(btn[1]);
	//int x;
	pxa_gpio_mode(GPIO17_PWM1_MD);
	pxa_gpio_mode(GPIO16_PWM0_MD);
	pxa_set_cken(CKEN1_PWM1, 1);
	pxa_set_cken(CKEN0_PWM0, 1);
	
	PWM_PWDUTY1=(unsigned)0;
	PWM_PWDUTY0=(unsigned)0;
	//PWM_CTRL1=63;
	PWM_PERVAL1=(unsigned)10;
	PWM_PERVAL0=(unsigned)10;
	PWM_CTRL1=(unsigned)99;
	PWM_CTRL0=(unsigned)99;
	
	
	/*if (request_irq(irq3, &gpio_irq3, SA_INTERRUPT 
				"mygpio", NULL) != 0 ) {
                printk ( "irq3 not acquired \n" );
                return -1;
        }else{
                printk ( "irq3 %d acquired successfully \n", irq3);
	}*/
	if (request_irq(irq1, &gpio_irq1, SA_INTERRUPT | SA_TRIGGER_RISING | SA_TRIGGER_FALLING,
				"mygpio", NULL) != 0 ) {
                printk ( "irq1 not acquired \n" );
                return -1;
        }else{
                printk ( "irq1 %d acquired successfully \n", irq1);
	}
       if (request_irq(irq2, &gpio_irq2, SA_INTERRUPT | SA_TRIGGER_RISING| SA_TRIGGER_FALLING,
				"mygpio", NULL) != 0 ) {
                printk ( "irq2 not acquired \n" );
                return -1;
        }else{
                printk ( "irq2 %d acquired successfully \n", irq2 );
	}
    

/*	//setup the timer
	init_timer(&my_timer);	
	my_timer.expires = jiffies + HZ;// + j*HZ;
	my_timer.function = my_function; 
	my_timer.data = mygpio_buffer;
	add_timer(&my_timer);
	*/
	return 0;

fail: 
	mygpio_exit(); 
	return result;
}

static void mygpio_exit(void)
{
	del_timer(&my_timer);	
	/* Freeing the major number */
	unregister_chrdev(mygpio_major, "nirmit_mygpio");

	/* Freeing buffer memory */
	if (mygpio_buffer)
	{
		kfree(mygpio_buffer);
	}
	free_irq(IRQ_GPIO(btn[0]), NULL);
	free_irq(IRQ_GPIO(btn[1]), NULL);

	

}

static int mygpio_open(struct inode *inode, struct file *filp)
{
	
	return 0;
}

static int mygpio_release(struct inode *inode, struct file *filp)
{
	
	return 0;
}

static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{ 

	/*char str[256];
	int len;memset(str,'\0',256);
	
	//read through the list and return an end signal to indicate the end of linked list cand copy to user
	len = sprintf(str,"%d", counttime==15?0:counttime+1);
		
	if(*f_pos >= len)
		return 0;
	if (copy_to_user(buf, str, 128))
	{
		return -EFAULT;
	}
        memset(str,'0',256);
	 memset(str,'\0',256);
	//Changing reading position as best suits 
	*f_pos += len;  
	return len;*/
	return 0;
}

static ssize_t mygpio_write(struct file *filp, const char *buf,size_t count, loff_t *f_pos)
{
	/*int j=0;
	int k=0;
	int temp=0;
	//printk("here");
        
        memset(mygpio_buffer,'\0',capacity);
	if (copy_from_user(mygpio_buffer, buf, count))
	{
		return -EFAULT;
	}
	//printk("%s",mygpio_buffer);

       
	//separate the message receive from user, the message start with "f" is the period, and the message start with "v" is the counter value
	if(count == 3)
	{	
		if(mygpio_buffer[0]== 'f')
		{
			if(mygpio_buffer[1]>=49 && mygpio_buffer[1]<=57){
				j = (mygpio_buffer[1]-48)+j*10;	//period
				counterperiod = j;
			}return count;
		}
		if(mygpio_buffer[0]== 'v')
		{					  
			if(mygpio_buffer[1]>=48 && mygpio_buffer[1]<=57)//from 0 to 9
			{
				k= mygpio_buffer[1]-48;
				counttime = k;
			}
			else if(mygpio_buffer[1]>=97 && mygpio_buffer[1]<=102)//from a to f
			{	
				k= mygpio_buffer[1]-87;
				counttime = k;
			}
			return count;
		}
		
	}
	//printk("here2");
	del_timer(&timer);     

        while(mygpio_buffer[temp]!='a')
	{
	  j = (mygpio_buffer[temp]-48)+j*10;	
	 temp++;
	}
	if(j==0)
	{
	return count;
	}
        //printk("delay=%d\n",j);
	init_timer(&timer);
 	timer.function = timer1_routine;
	timer.expires = jiffies + j*HZ; /* 1 second 
	add_timer(&timer);
	headerflag=1;
	delay=j;
	*/

	
return 0;
}
