/*****************************************************************************
* NHD-2.4-240320CF-Cxxx.ino
* 
* Program for writing to Newhaven Display 2.4" TFTs with ST7789Vi controller. 
* This code is written for the Arduino Uno, and uses 8-bit Parallel Interface. 
* This code is applicable for the following Newhaven Display models: 
* 
* NHD-2.4-240320CF-CTXI#-F
* NHD-2.4-240320CF-CTXI#-FT
* NHD-2.4-240320CF-CSXV#-F
* NHD-2.4-240320CF-CSXN#-F
* NHD-2.4-240320CF-CSXN#-FT
* 
* Newhaven Display invests time and resources providing this open source code,
* Please support Newhaven Display by purchasing products from Newhaven Display!
*
* Copyright: Alee Shah (c) 2021, Newhaven Display International
*
* This code is provided as an example only and without any warranty by Newhaven Display. 
* Newhaven Display accepts no responsibility for any issues resulting from its use. 
* The developer of the final application incorporating any parts of this 
* sample code is responsible for ensuring its safe and correct operation
* and for any consequences resulting from its use.
* See the GNU General Public License for more details. 
* 
*****************************************************************************/

/****************************************************
*         PINOUT: Arduino Uno -> 2.4" TFT           *
*****************************************************/

// The 8 bit data bus is connected to PORTD of the Arduino Uno

////////////////          // /CS signal tied to GND
////////////////          // IM0 signal tied to VDD
#define   RS    8         // RS or D/C signal connected to Arduino digital pin 8
#define   WR    9         // /WR signal connected to Arduino digital pin 9
#define   RD    10        // /RD signal connected to Arduino digital pin 10
#define   RST   11        // /RST signal connected to Arduino digital pin 11

/****************************************************
*                 Function Commands                  *
*****************************************************/

void comm_out(unsigned char c)
{
  digitalWrite(RS, LOW);
  PORTD = c;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

void data_out(unsigned char d)
{
  digitalWrite(RS, HIGH);
  PORTD = d;
  digitalWrite(WR, LOW);
  digitalWrite(WR, HIGH);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Window Set Function
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void window_set(unsigned s_x, unsigned e_x, unsigned s_y, unsigned e_y)
{
  comm_out(0x2a);    //SET column address
  digitalWrite(RST, HIGH);
  data_out((s_x)>>8);     //SET start column address
  data_out(s_x);
  data_out((e_x)>>8);     //SET end column address
  data_out(e_x);
  
  comm_out(0x2b);    //SET page address
  digitalWrite(RST, HIGH);
  data_out((s_y)>>8);     //SET start page address
  data_out(s_y);
  data_out((e_y)>>8);     //SET end page address
  data_out(e_y);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fill Screen (All Red -> All Green -> All Blue) Function
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void disp()
{
  {
  unsigned int i; 
  comm_out(0x2C);              //command to begin writing to frame memory
  for(i=0;i<38400;i++)         //fill screen with blue pixels
  {
            data_out(0x00);
            data_out(0x1F);
            data_out(0x00);
            data_out(0x1F);
  }
  for(i=0;i<38400;i++)         //fill screen with green pixels
  {
            data_out(0x07);
            data_out(0xE0); 
            data_out(0x07);
            data_out(0xE0); 
  }
        
  for(i=0;i<38400;i++)         //fill screen with red pixels
  {
            data_out(0xF8);
            data_out(0x00);
            data_out(0xF8);
            data_out(0x00);
  }      
  delay(300);
}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fill Screen (Red, Green & Blue Lines) Function
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void disp2()
{
  {
  unsigned int i; 
  comm_out(0x2C);              //command to begin writing to frame memory
  for(i=0;i<12800;i++)         //fill screen with blue pixels
  {
            data_out(0x00); //R
            data_out(0x1F); //R
            data_out(0x00); //R
            data_out(0x1F); //R

            data_out(0xF8); //B
            data_out(0x00); //B
            data_out(0xF8); //B
            data_out(0x00); //B

            data_out(0x07); //G
            data_out(0xE0); //G
            data_out(0x07); //G
            data_out(0xE0); //G          
            
  }            
 }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Border and Fill Function
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Border_Fill()
{
  unsigned int i,j;
  window_set(0,239,0,0);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<240;i++)        //Bottom White Border
  {
    for (j=0;j<1;j++)                
    {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    }
  }
  delay(100);
  window_set(0,0,0,319);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<1;i++)        //Left White Border
  {
    for (j=0;j<320;j++)                
    {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    }
  }
  delay(100);
  window_set(0,239,319,319);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<240;i++)      //Top White Border
  {
    for (j=0;j<1;j++)                
    {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    }
  }
  delay(100);
  window_set(239,239,0,319);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<1;i++)        //Right White Border
  {
    for (j=0;j<240;j++)                
    {
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    data_out(0xFF);
    }
  }
  delay(100);
  window_set(1,238,1,318);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<238;i++)      //fill inside with Black Pixels
  {
    for (j=0;j<318;j++)                
    {
      data_out(0x00);
      data_out(0x00); 
      data_out(0x00);
      data_out(0x00); 
    }
  }
  delay(250);
  window_set(0,239,0,319);
  comm_out(0x2C);
  digitalWrite(RST, HIGH);
  for(i=0;i<38400;i++)    //fill screen with White Pixels
  {
            data_out(0xFF);
            data_out(0xFF);
            data_out(0xFF);
            data_out(0xFF);
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fill Screen (All Black) Function
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void disp3()
{
  unsigned int i; 
  window_set(0,239,0,319);
  comm_out(0x2C);              //command to begin writing to frame memory
        for(i=0;i<38400;i++)   //fill screen with black pixels
  {
            data_out(0x00);
            data_out(0x00);
            data_out(0x00);
            data_out(0x00);
  }
}

/****************************************************
*         Initialization and Setup Routine          *
*****************************************************/

void setup()
{
  DDRB = 0xFF;  //Enable All outputs on PortB   
  PORTB = 0x00;
  DDRC = 0xFF;  //Enable All outputs on PortC   
  PORTC = 0x00;
  DDRD = 0xFF;  //Enable All outputs on PortD 
  PORTD = 0x00;
  
  digitalWrite(RD, HIGH);
  digitalWrite(WR, LOW);
  digitalWrite(RST, LOW);
  delay(250);
  digitalWrite(RST, HIGH);
  delay(250);
  
  comm_out(0x28);   //display off
  comm_out(0x11);  //exit SLEEP mode
  delay(100);
  
  comm_out(0x36);  //MADCTL: memory data access control
  data_out(0x80);
  
  comm_out(0x3A);  //COLMOD: Interface Pixel format  *** 65K-colors in 16bit/pixel (5-6-5) format when using 16-bit interface to allow 1-byte per pixel
  data_out(0x55);
  
  comm_out(0xB2);  //PORCTRK: Porch setting
  data_out(0x0C);
  data_out(0x0C);
  data_out(0x00);
  data_out(0x33);
  data_out(0x33);
  
  comm_out(0xB7);  //GCTRL: Gate Control
  data_out(0x35);
  
  comm_out(0xBB);  //VCOMS: VCOM setting
  data_out(0x2B);
 
  comm_out(0xC0);  //LCMCTRL: LCM Control
  data_out(0x2C);
  
  comm_out(0xC2);  //VDVVRHEN: VDV and VRH Command Enable
  data_out(0x01);
  data_out(0xFF);
  
  comm_out(0xC3);  //VRHS: VRH Set
  data_out(0x11);
  
  comm_out(0xC4);  //VDVS: VDV Set
  data_out(0x20);
  
  comm_out(0xC6);  //FRCTRL2: Frame Rate control in normal mode
  data_out(0x0F);
  
  comm_out(0xD0);  //PWCTRL1: Power Control 1
  data_out(0xA4);
  data_out(0xA1);
  
  comm_out(0xE0);  //PVGAMCTRL: Positive Voltage Gamma control  
  data_out(0xD0);
  data_out(0x00);
  data_out(0x05);
  data_out(0x0E);
  data_out(0x15);
  data_out(0x0D);
  data_out(0x37);
  data_out(0x43);
  data_out(0x47);
  data_out(0x09);
  data_out(0x15);
  data_out(0x12);
  data_out(0x16);
  data_out(0x19);
  
  comm_out(0xE1);  //NVGAMCTRL: Negative Voltage Gamma control  
  data_out(0xD0);
  data_out(0x00);
  data_out(0x05);
  data_out(0x0D);
  data_out(0x0C);
  data_out(0x06);
  data_out(0x2D);
  data_out(0x44);
  data_out(0x40);
  data_out(0x0E);
  data_out(0x1C);
  data_out(0x18);
  data_out(0x16);
  data_out(0x19);
  
  comm_out(0x2A);  //X address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x00);
  data_out(0xEF);

  comm_out(0x2B);  //Y address set
  data_out(0x00);
  data_out(0x00);
  data_out(0x01);
  data_out(0x3F);
  delay(10);
  
  comm_out(0x29);  //display ON
  delay(10);
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop()
{ 
  disp();
  delay(500);
  disp2();
  delay(500);
  Border_Fill();
  delay(500);
}
