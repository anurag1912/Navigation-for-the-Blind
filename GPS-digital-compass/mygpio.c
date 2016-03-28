#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <asm/uaccess.h>
#include<linux/interrupt.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <asm/gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
unsigned motor[2]= {29, 28};
/* Initialization of all global variables to be used*/
unsigned int north=0;
unsigned int west=0;
unsigned int x=0;
unsigned int i=0;
unsigned int j=0;
unsigned int k=0;
unsigned long ch;
unsigned long stiir;
unsigned char character;
unsigned char flag=0;
unsigned int nchar=0;
unsigned char d1=0;
unsigned char d2=0;
unsigned long d=0;
unsigned long a=0;
unsigned long cread=0;
unsigned long temp;
unsigned int l=0;
unsigned int m=0;
int p=0;
unsigned char step=65;
unsigned int abc=0;
int z=0;
int writeflag=0;
int rec, ret2;
char gll[7] = {'$','G','P','G','L','L','\0'};
char arr[7];
char message[1024];
char mess[1024];
char dicomp[1024];
char final[1024];
char *buffer;
int flag1=1;
int capacity =1024;
int number=0;
int counter=0;;
int y=0;
int time=8;
struct data{
int north;
int west;
int orient;
}cord[310];
int mytimer_len=0;
struct timer_list timer1;

irqreturn_t handler_receive(int irq, void *dev_id); 		//UART Receive Interrupt Handler (Used for the GPS module)
static int mygpio_open(struct inode *inode, struct file *filp);
static int mygpio_release(struct inode *inode, struct file *filp);
static int mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static int mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int mygpio_init(void);
static void mygpio_exit(void);
void timer1_routine(unsigned data);





struct file_operations mygpio_fops = {
	read: mygpio_read,
	write: mygpio_write,
	open: mygpio_open,
	release: mygpio_release
};


void timer1_routine(unsigned data){
del_timer(&timer1);
north=cord[time].north;
  west=cord[time].west;
  cread=cord[time].orient;
  time++;
if(flag1==1){
  
			if(west<=cord[8].west-400){     /* The GPS module takes around 8-10 second to load. Here we are dealing with a predefined location as we dont have the maps*/
			printk("write direction\n");
			}
			
			
			else if(west<(cord[232].west)){
				while((cread >= 2200 || cread <=2800)){
				  int i=0;
				  printk("right motor ON\n");
				  pxa_gpio_set_value(motor[0],1);
				  while(i<1000){
				
				i++;
				}
				}
					
				printk("GO STRAIGHT\n");
				pxa_gpio_set_value(motor[1],1);
			}
			else if(west>cord[232].west){
			flag1=0;
			
			}
			timer1.expires=jiffies + (1*HZ);
			add_timer(&timer1);
			

}
		
 else if(flag1==0){
			if(north<cord[234].north){
				printk("right turn  ahead");
				timer1.expires=jiffies + (1*HZ);
				add_timer(&timer1);
				
			}
			else if(north<cord[302].north ){
						while(cread <= 200 || cread >=600){
						
						  printk("Left motor ON");
						  pxa_gpio_set_value(motor[0],0);
						  while(i<1000){
				
						  i++;
						}
						}
							timer1.expires=jiffies + (1*HZ);
								add_timer(&timer1);
							printk("GO STRAIGHT\n");
							pxa_gpio_set_value(motor[0],1);
		   				}
			
			else if(north==cord[303].north){
			printk("you have arrived ur destination");
			del_timer(&timer1);
			printk("stop motor");
			pxa_gpio_set_value(motor[0],0);
			pxa_gpio_set_value(motor[1],0);
			
			}
			
				
	}	
	




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


return 0;
}

/*The write function gathers data from the user program i.e. the look-up table for the data obtained for the path is stored on the SD card and is transmitted to the kernel module.
This is used to compare with the current position obtained from the GPS and digital compass*/
static ssize_t mygpio_write(struct file *filp, const char *buf,
							size_t count, loff_t *f_pos)
{


	

///* do not eat more than a bite */
count=strlen(buf);
memset(buffer,'\0',capacity);
if (copy_from_user(buffer, buf, count))
				      {
					      return -EFAULT;
				      }
if(writeflag==0)		//flag to indicate whether data is north co-ordinate or west coordinate or orientation information.
		{number=0;
		z=0;
		while(buffer[z]!='a')		//convert char array to int value	
		{number = number*10 + (buffer[z]-'0');
		z++; 
		}
		cord[y].north=number;
		printk("%d\t",cord[y].north);
		writeflag++;


		}


else if(writeflag==1)			//west co-ordinate obtained here!
		      {number=0;
		      z=0;

		      while(buffer[z]!='a')			//convert char array to int value
					  {number = number*10 + (buffer[z]-'0');	//structure to store the co-ordinates so obtained
					  z++; 	

					  }


		      cord[y].west=number;			//structure to store the co-ordinates so obtained
		      printk("%d\t",cord[y].west);
		      writeflag++;
}


else{					//orientation information obtained here!
number=0;
z=0;

while(buffer[z]!='a')			//convert char array to int value
{number = number*10 + (buffer[z]-'0');	
z++; 

}


cord[y].orient=number;			//structure to store the co-ordinates so obtained
printk("%d\n",cord[y].orient);
writeflag=0;




y++;

}
return count;
}

static int mygpio_init(void)
{
printk(KERN_INFO "Module is loaded\n=====\n");
int ret, result;
gpio_direction_output(motor[0],0);
gpio_direction_output(motor[1],0);
/*Initialize structure to store information obtained from the user level program*/
for(z=0;z<310;z++)
		  {cord[z].north=0;
		  cord[z].west=0;
		  cord[z].orient=0;
}

buffer = kmalloc(capacity, GFP_KERNEL); 
if (!buffer)
	{ 
		printk(KERN_ALERT "Insufficient kernel memory\n"); 
		result = -ENOMEM;
		
	} 
memset(buffer, '\0', capacity);			//Initialize buffer to 0
mytimer_len = 0;	
ret = register_chrdev(61, "mygpio", &mygpio_fops);
pxa_set_cken(CKEN5_STUART,1);		//Supply clock to STUART module 
pxa_set_cken(CKEN14_I2C,1);		//Supply clock to the I2C module
pxa_gpio_mode(GPIO46_STRXD_MD);		//RXD is input pin
pxa_gpio_mode(GPIO47_STTXD_MD);		//TXD is output pin for the STUART



/* The GPS module updates every 1 sec and transmits at 9600 baud with 8 data bits,1 stop pit and no parity bit */	
STIER = 0;				
STMCR = 0x08;    				
STFCR = 0x00000000;               	// Non-FIFO mode
STLCR = 0x00000083;         	 	// Prepare to access DLL
STDLL = 0x00000060;                 // Set port speed to 9600 Baud
STDLH = 0;                         	// Set port speed to 9600 Baud
STLCR = 0x00000003;  		// 8-bit chars, No parity, and 1 stop bit
STIER =0x00000041;                  // Enable this UART
memset(dicomp, '\0', capacity);		//Initialize char array to store orientation info. for the instantaneous data received.
memset(message, '\0' ,capacity);	//Initialize char array to store the GPS co-ordinates for the instantaneous data received.
memset(mess, '\0' ,capacity);		
rec = IRQ_STUART;				
ret2 = request_irq(rec, handler_receive, SA_INTERRUPT, "gpio_irq_receive", NULL);	//Interrupt handler for UART receive complete.	
init_timer(&timer1);									/*Initialize timer for comparing data with current value received and the look-up table to check if the user is heading in the 
										    right direction and is oriented correctly*/	
timer1.function = timer1_routine;
timer1.expires = jiffies + 1*HZ; /* 1 second */
add_timer(&timer1);


return 0;
}

static void mygpio_exit(void)
{	//printk("%s\n",message);
free_irq(rec, NULL);

unregister_chrdev(61, "mygpio");
del_timer(&timer1);
	printk(KERN_INFO "Module is unloaded\n");
}



irqreturn_t handler_receive(int irq, void *dev_id) {
stiir = STLSR;			//Get status register value
stiir&= 0x00000001;		//Get receiver buffer full bit 
if(stiir==1)			//Is the buffer full?
	    {
	    ch = STRBR;			
	    ch = ch & 0x000000ff;
	    character = (char)ch;

				if(character=='$' && x==0)	//We want to parse the NMEA sentences (Example format is $GLL,4221.05564,N,07106.55825,W,012841.00,A,A*7D) in such a way so as to only get the latitude and longitude fix. We leave out all other data.	
				{
				arr[x]=character;
				x++;
				}

				else if(x>=1 && x<=5)
				{


				arr[x]=character;
				x++;


				}

				else if(x==6)			
				{
				arr[x]='\0';
				if(strcmp(arr,gll)==0)		//Latitude and longitude data obtained here. GLL is the identifier and we parse only that sentence.
						      {
						      if(character!='\r') 		

									{
									message[j]=character;		//Store character in message array 
									j++;
									}
									else{
									message[j]='\0';
									strcpy(mess,message);		//Copy the entire received array in the 'mess' array so that message can receive new characters from the GPS module.
									x=0;
									j=0;



									/*I2C interface for the digital compass. The compass has several modes. The program to set the appropriate mode was written earlier and stored in the EEPROM. At power-on the compass conforms to the set mode.*/





									ICR=0;
										ICR|=0x00000060;	//The gumstix is going to be the master and the digital compass module HMC 6352 is going to be the slave.

										IDBR=0x42;		//Address of the slave and the last bit indicates that this is a write. Command 0x42 sets up the write to the slave, in this case the digital compass. 
										ICR|=0x00000009;	//Set TB=1 to transmit data. Clock is 100kbps as defined in the ICR register.  	

										while((ISR & ISR_ITE)!=64); //Poll and see if transmit has been completed and acknowledgement has been received by the slave.

									printk("ISR on transmit write %x\n", ISR);		//Debug statement to check if transmission status bit has been set.
											
										ICR&=0xfffffff8;			
										IDBR=0x41;					//Command to receive the orientation information. Once this has been received the module transmits the orientation in tenths of degrees i.e 0 to 3599 in two bytes MSB first. 
										
										ICR|=0x00000008;	//setting ICR[TB]
										while((ISR & ISR_ITE)!=64);			//Poll and see if transmit has been completed and acknowledgement has been received by the slave.

									printk("ISR on transmit command A %x\n", ISR);


									/* We are now setting up to receive the data from the slave at this point*/		
									ICR&=0xfffffff8;
										IDBR=0x43;					//Master = gumstix, Slave = Compass. Master transmits slave address with read command. 		
																//Master transmits, Slave receives data.
										ICR|=0x00000009;	//setting ICR[TB]1
										while((ISR & ISR_ITE)!=64);			//Transmit complete and acknowledgement from slave received?

									printk("ISR on transmit read address %x\n", ISR);	
											
										ICR&=0xfffffff8;      //Setting first read
										ICR|=0x00000008;		
										while((ISR & ISR_IRF) !=128);			//Transmit/Receive Buffer full?
										
									printk("ISR on data receive MSB %x\n",ISR);
										d1=IDBR;					//Store IDBR value in a temp variable.
										//printk("%x\n",d1);				



									ICR&=0xfffffff8;      //Setting second read
										ICR|=0x0000000e;				
										while((ISR & ISR_IRF) !=128);			//Transmit/Receive Buffer full?
										printk("ISR on data receive LSB %x\n",ISR);
										d2=IDBR;					//Store IDBR in another temp variable.
										//printk("%x\n",d2);
									d=0;
									d=d1;
									d<<=8;							//Left shift 8 times the value to accomodate the Least significant byte.
									a=0;
									a=d2;							
									//printk("under :In degrees: %u\n", d|a);


									cread = d|a;	
									/*Store the int value in cread variable. We now have the current position and orientation info at this point. We OR the two values as we don't want to modify either of the
									values but just want to combine them to get a single number.*/

									a=0;
									d=0;
									d1=0;

									/* Check to see if the position data is valid or not. A valid data has the 'A'(active) flag set in the GLL sentence. Incorrect data has the 'V' (void) flag set */
									while(mess[a]!='V' && mess[a]!='\0')
									a++;

									if(mess[a]!='V')
									{
									/*parse data received because it is valid*/
									a=0;
									while(mess[a]!='.')   /*Example format is $GLL,4221.05564,N,07106.55825,W,012841.00,A,A*7D. For our path the degrees and minutes dont change as they correspond to thousands of feet.
									The sec and milli sec only change hence we extract only that info.*/

									a++;
									a++;
									while(mess[a]!=',')
									{north = north*10 + (mess[a]-'0');		//north sec and milisec info converted to int value
									a++;
									}
									while(mess[a]!='.')
									a++;
									a++;
									while(mess[a]!=',')
									{west = west*10 + (mess[a]-'0');		//west sec and milli-sec info converted to int value
									a++;
									}
									a=0;

									flag=1;
									}


//Re-initialize everything as one entire operation of receiving and parsing the valid data has been done.
									nchar=0;
									cread=0;
									a=d=d1=d2=k=0;
									memset(dicomp,'\0',capacity);
									memset(mess,'\0',capacity);
									memset(message, '\0' ,capacity);

									}

							}


				else x=0;
				}


	}
	return IRQ_HANDLED;
}









module_init(mygpio_init);
module_exit(mygpio_exit);



