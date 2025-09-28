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
extern void enable_interrupt();

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";
char timeoutcount = 0;

static const int seven_seg_digits[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

volatile unsigned int* timer_status  = (volatile unsigned int*) 0x04000020;
volatile unsigned int* timer_control = (volatile unsigned int*) 0x04000024;
volatile unsigned int* timer_periodl = (volatile unsigned int*) 0x04000028;
volatile unsigned int* timer_periodh = (volatile unsigned int*) 0x0400002C;

int prime = 1234567;
int hours = 0;

// timer registers


char timer_TO() {
  if (*timer_status & 0x1) {
    *timer_status  = 0x0;      // clear TO
    *timer_control = 0x5;   // restart timer with interrupt
    return 1;
  }
  return 0; //no timeout
}

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
void handle_interrupt(unsigned cause) {
  if (timer_TO()) timeoutcount++;
  
  if (timeoutcount == 10)
  {
    timeoutcount = 0;                          // reset timeoutcount

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

    tick( &mytime );  //tick the clock once

    //increment hour counter when minutes roll over
    if ((mytime & 0xFFFF) == 0x0000) {
      hours++;
    }
  }
  
}

/* Add your code here for initializing interrupts. */
void labinit(void) {
  // write 2 999 999 to the period register (100 ms - 1 extra cycle)
   *timer_periodl = 0xC6BF; // lower 16 bits
   *timer_periodh = 0x002D; // upper 16 bits
  
   *timer_control = 0x0005; // start bit to 1, all other to zero

   enable_interrupt();
}

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
  labinit();
  while (1) {
    print ("Prime: ");
    prime = nextprime( prime );
    print_dec( prime );
    print("\n");
  }
}


