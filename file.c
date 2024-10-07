#include<LPC17xx.h>
#include<stdio.h>
#include<math.h>
#include <stdbool.h>
//#include<AN_LCD.h>
#define Ref_Vtg 3.300
#define Full_Scale 0xFFF //12 bit ADC
extern unsigned long int temp1 , temp2;
unsigned long int temp1=0, temp2=0;
double p1,p2,dutyCycle;
int count=0;
float avg = 0;
float alpha = 0.98;
bool state1 = true;
bool state2 = true;
bool insideState = false;
bool outsideIr=false;
bool isPeopleExiting=false;
int j=1;
unsigned int i;
int data1,data2;
#define RS_CTRL 0x00000100? //P0.8
#define EN_CTRL 0x00000200? //P0.9
#define DT_CTRL 0x00000FF0? //P0.4 to P0.7 data lines
void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(char *);
void loop1(int a, int b);
 
int main(void){
unsigned int adc_temp,adc_temp1, adc_temp2;
 
float in_vtg, in_vtg1, in_vtg2;
char vtg[14],dval[14];
char Msg3[] = {"Moisture:"};
 
//char Msg4[] = {"Moisture value:"};
SystemInit();
SystemCoreClockUpdate();
//LPC_SC->PCONP |= (1<<15);//Power for GPIO block
//LPC_SC->PCONP |= (1<<12);//peripheral devices for ADC turned on
lcd_init();
LPC_PINCON->PINSEL1 |= 1<<14;
LPC_PINCON->PINSEL1 |= 1<<16;
LPC_PINCON->PINSEL1 |= 1<<18;//P0.23 as AD0.0(23-16)*2
LPC_SC->PCONP |= (1<<12); //enable the peripheral ADC
temp1 = 0x80;
lcd_com();
delay_lcd(800);
lcd_puts(&Msg3[0]);
temp1 = 0xC0;
lcd_com();
delay_lcd(800);
//lcd_puts(&Msg4[0]);
while(1)
{
	LPC_ADC->ADCR = (1<<0)|(1<<21)|(1<<24);//0x01200001; //ADC0.0, start conversion and operational
		//delay for conversion
	while(((adc_temp = LPC_ADC->ADGDR) & 0x80000000) ==0);
	//wait till 'done' bit is 1, indicates conversion complete
	adc_temp = LPC_ADC->ADGDR;
	adc_temp >>= 4;
	adc_temp &= 0x00000FFF; //12 bit ADC
	in_vtg = (((float)adc_temp * (float)Ref_Vtg))/((float)Full_Scale); //calculating input analog
	//voltage
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	LPC_ADC->ADCR = 0;
	LPC_ADC->ADCR = (1<<1)|(1<<21)|(1<<24);//0x01200001; //ADC0.0, start conversion and operational
		//delay for conversion
	while(((adc_temp1 = LPC_ADC->ADGDR) & 0x80000000) ==0);
	//wait till 'done' bit is 1, indicates conversion complete
	adc_temp1 = LPC_ADC->ADGDR;
	adc_temp1 >>= 4;
	adc_temp1 &= 0x00000FFF; //12 bit ADC
	in_vtg1 = (((float)adc_temp1 * (float)Ref_Vtg))/((float)Full_Scale); //calculating input analog
	//voltage
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	LPC_ADC->ADCR = 0;
	LPC_ADC->ADCR = (1<<2)|(1<<21)|(1<<24);//0x01200001; //ADC0.0, start conversion and operational
		//delay for conversion
	while(((adc_temp2 = LPC_ADC->ADGDR) & 0x80000000) ==0);
	//wait till 'done' bit is 1, indicates conversion complete
	adc_temp2 = LPC_ADC->ADGDR;
	adc_temp2 >>= 4;
	adc_temp2 &= 0x00000FFF; //12 bit ADC
	in_vtg2 = (((float)adc_temp2 * (float)Ref_Vtg)*100)/((float)Full_Scale);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	if(in_vtg<1.5)
		data1=0;
	else
		data1=1;
	if(in_vtg1<1.5)
		data2=0;
	else
		data2=1;
	loop1(data1,data2);
	avg = alpha*avg + (1-alpha)*in_vtg2;


  if((count*1.0)/4 <= 1){
    p1 = (count*1.0)/4;
  }
  else{
    p1 = 1;
  }
 
  if(avg>70){
    p2 = 1;
  }
  else if(avg<30){
    p2 = 0;
  }
  else{
    p2 = (avg-30)/70;
  }
  dutyCycle = (p1+p2)/2;
	if(count==0)
		dutyCycle=0;

	sprintf(vtg,"%d",count);
	//convert the readings into string to display on LCD
 
	sprintf(dval,"%0.2f",dutyCycle);
	for(i=0;i<2000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	temp1 = 0x8A;
	lcd_com();
	delay_lcd(800);
	lcd_puts(&vtg[0]);
	temp1 = 0xCB;
	lcd_com();
	delay_lcd(800);
	lcd_puts(&dval[0]);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	for(i=0;i<200000;i++);
	//for(i=0;i<7;i++)
	//vtg[i] = dval[i] = 0x00;
	//adc_temp = 0;
	//in_vtg = 0;
	}
}
 
void loop1(int data1, int data2) {
    // Assuming data1 and data2 are variables representing sensor readings
    if (!data1 && j == 1 && state1) {
        outsideIr = true;
        for(i=0;i<2000;i++);
        j++;
        state1 = false;
    } else if (!data2 && j == 2 && state2) {
        //Serial.println("Entering inside the room");
        outsideIr = true;
        for(i=0;i<2000;i++);
        j = 1;
        count++;
        //Serial.print("No. of people inside room: ");
        //Serial.println(count);
        state2 = false;
    } else if (!data2 && j == 1 && state2) {
        outsideIr = true;
        for(i=0;i<2000;i++);
        j = 2;
        state2 = false;
    } else if (!data1 && j == 2 && state1) {
        //Serial.println("Exiting from room");
        outsideIr = true;
        for(i=0;i<2000;i++);
        if (count) {
            count--;
        }
        //Serial.print("No. of people inside room: ");
        //Serial.println(count);
        j = 1;
        state1 = false;
    }
    if (data1) {
        state1 = true;
    }
    if (data2) {
        state2 = true;
    }
}

 
void lcd_init()
{
    /* Ports initialized as GPIO */
		LPC_PINCON->PINSEL0 &= 0x00000000; //P0.0 to P0.15
    /* Setting the directions as output */
	LPC_GPIO0->FIODIR |= 0x00000FF0;//DT_CTRL;
    LPC_GPIO0->FIODIR |= 0x00000100; //RS_CTRL; 
	LPC_GPIO0->FIODIR |= 00000200; //EN_CTRL;????
	clear_ports();
	delay_lcd(3200);
temp2 = (0x30);
	wr_cn();
	delay_lcd(30000);
temp2 = (0x30);
wr_cn();
	delay_lcd(30000);
 
temp2 = (0x30);
wr_cn();
delay_lcd(30000);
 
 
temp2 = (0x20);
wr_cn();
delay_lcd(30000);
 
 
    temp1 = 0x28; 
    lcd_com();
    delay_lcd(30000);
temp1 = 0x0c;
lcd_com();
delay_lcd(800);
 
temp1 = 0x06;
lcd_com();
delay_lcd(800);
 
temp1 = 0x01;
lcd_com();
delay_lcd(10000);
 
temp1 = 0x80;
lcd_com();
delay_lcd(800);
return;
}
 
 
void lcd_com(void)
{
temp2 = temp1 & 0xf0;//move data (7-8+1) times : 26 - HN
	//place, 4 - Bits
temp2 = temp2 ; //data lines from 4 to 7
wr_cn();
temp2 = temp1 & 0x0f; //7-4+1
temp2 = temp2 << 4;
wr_cn();
delay_lcd(1000);
return;
}
 
// command nibble o/p routine
void wr_cn(void) //write command reg
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2; // Assign the value to the data lines
LPC_GPIO0->FIOCLR = 0x00000100;// clear bit RS
LPC_GPIO0->FIOSET = 0x00000200;// EN=1
delay_lcd(25);
LPC_GPIO0->FIOCLR = 0x00000200;// EN =0
return;
 
}
 
 
// data o/p routine which also outputs high nibble first
// and lower nibble next
void lcd_data(void)
{
temp2 = temp1 & 0xf0;
temp2 = temp2 ;
wr_dn();
temp2= temp1 & 0x0f;
temp2= temp2 << 4;
wr_dn();
delay_lcd(1000);
return;
}
 
// data nibble o/p routine
void wr_dn(void)
{clear_ports();
LPC_GPIO0->FIOPIN = temp2; // Assign the value to the data lines
LPC_GPIO0->FIOSET = 0x00000100;// set bit RS
LPC_GPIO0->FIOSET = 0x00000200;// EN=1
delay_lcd(25);
LPC_GPIO0->FIOCLR = 0x00000200; // EN =0
return;
}
 
void delay_lcd(unsigned int r1)
{
unsigned int r;
for(r=0;r<r1;r++);
return;
}
 
 
void clr_disp(void)
{
temp1 = 0x01;
lcd_com();
delay_lcd(10000);
return;
}
void clear_ports(void)
{
/* Clearing the lines at power on */
LPC_GPIO0->FIOCLR = 0x00000FF0; //Clearing data lines
LPC_GPIO0->FIOCLR = 0x00000100; //Clearing RS line
LPC_GPIO0->FIOCLR = 0x00000200; //Clearing Enable line
return;
}
void lcd_puts(char *buf1){
unsigned int i=0;
while(buf1[i]!='\0'){
temp1 = buf1[i];lcd_data();
i++;
if(i==27){
	temp1 = 0xc0;
	lcd_com();
}
}
return;
}