/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern int nextprime( int );
extern void _enable_interrupt();

char timeoutcount = 0;

volatile unsigned int* timer_status  = (volatile unsigned int*) 0x04000020;
volatile unsigned int* timer_control = (volatile unsigned int*) 0x04000024;
volatile unsigned int* timer_periodl = (volatile unsigned int*) 0x04000028;
volatile unsigned int* timer_periodh = (volatile unsigned int*) 0x0400002C;

// timer registers


char timer_TO() {
  if (*timer_status & 0x1) {
    *timer_status  = 0x0000;      // clear TO
    *timer_control = 0x0007;   // restart timer
    return 1;
  }
  return 0; //no timeout
}


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {
  if (timer_TO() && timeoutcount++ == 10) {
      print("timeout\n");
      timeoutcount = 0;
  }
}


/* Add your code here for initializing interrupts. */
void labinit(void) {
  _enable_interrupt();

  // write 2 999 999 to the period register (100 ms - 1 extra cycle)
   *timer_periodl = 0xC6BF; // lower 16 bits
   *timer_periodh = 0x002D; // upper 16 bits
  
   *timer_control = 0x0007; // start bit to 1, all other to zero
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  // Enter a forever loop
  while (1) {
  }
}


