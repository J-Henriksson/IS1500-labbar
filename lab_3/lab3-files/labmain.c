/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

static const int seven_seg_digits[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void set_leds(int led_mask) {
  volatile int* leds = (volatile int*) 0x04000000;
  *leds = led_mask;
}

void set_displays(int display_number, int value) {
  if (display_number > 0 && display_number < 7) {
    volatile char* address = (volatile char*) (0x04000050 + (display_number - 1) * 0x10);
    *address = seven_seg_digits[value];
  }
}

int get_sw( void ) {
  volatile int* address = (volatile int*) 0x04000010;
  int switches = *address & 0x3FF;  //isolate 10 LSBs
  return switches;
}

int get_btn(void) {
  volatile int* address = (volatile int*) 0x040000d0;
  int button = *address & 0x1;  //isolate 10 LSBs
  return button;
}



/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}

void set_seconds(int tens, int ones) {
  mytime = mytime & 0xFF00;
  mytime += (tens << 4) + ones;
}

void set_minutes(int tens, int ones) {
  mytime = mytime & 0x00FF;
  mytime += (tens << 12) + (ones << 8);
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int led_mask = 0;
  while (led_mask <= 0xF) {
      set_leds(led_mask);
      led_mask++;
    
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 1 );          // Delays 1 sec 
    tick( &mytime );     // Ticks the clock once
  }

  // Enter a forever loop
  int hours = 0;
  mytime = 0x5759;
  while (1) {
    if ((mytime & 0xFFFF) == 0x0000) {
      hours++;
    } 
    
    //write current time to displays
    int seconds_ones = mytime & 0xF;          //get seconds ones digit
    set_displays(1, seconds_ones);            

    int seconds_tens = (mytime >> 4) & 0xF;   //get seconds tens digit 
    set_displays(2, seconds_tens);            
    
    int minutes_ones = (mytime >> 8) & 0xF;   //get minutes ones digit
    set_displays(3, minutes_ones);

    int minutes_tens = (mytime >> 12) & 0xF;  //get minutes tens digit
    set_displays(4, minutes_tens);

    set_displays(5, hours%10);
    set_displays(6, hours/10);

    //print time to terminal and advance clock
    time2string( textstring, mytime );        // Converts mytime to string
    display_string( textstring );             //Print out the string 'textstring'
    delay( 1 );                               // Delays 1 sec (adjust this value)
    tick( &mytime );                          //tick the clock once

    //manual override
    if (get_btn())
    {
      int switches = get_sw();
      int value = switches & 0x03F; //isolate mode selection switches

      int ones = value % 10;
      int tens = value /10;

      switch (switches & 0x300)
      {
      case 0x100:
        set_seconds(tens, ones);
        break;

      case 0x200:
        set_minutes(tens, ones);
      break;

      case 0x300:
        hours = value;
      break;
      
      default:
        break;
      }

      if (switches & 0x040) //if switch 6 is on, exit loop
      {
        break;
      }
      
    }
    
  }
  
  print("program halted");
  while (1) {}
  
    
  
  

  
}


