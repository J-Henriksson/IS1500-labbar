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
static const int display_digits[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}

void set_leds(int led_mask) {
  volatile int* const leds = (volatile int*) 0x04000000;
  *leds = led_mask;
}

void set_displays(int display_number, int value) {
  if (display_number < 0 || display_number > 5) return; 
  volatile char* const address = (volatile char*) (0x04000050 + display_number * 0x10);
  *address = display_digits[value % 10];
};

int get_sw(void) {
  volatile int * const sw = (volatile int *)0x04000010;
  int v = *sw;
  return v & 0x3FF;           
}

int get_btn(void) {
  volatile int * const btn = (volatile int *)0x040000D0;
  int v = *btn;
  return v & 0x1;             
}

/* For assignment 1 part (d) */
static void startup_led_counter(void) {
  int value = 0;
  set_leds(0);                
  while (1) {
    delay(10);                  
    value++;
    set_leds(value); 
    if ((value & 0xF) == 0xF)
    break;
  }
}

/* Assignment 1 Part (h) */
static void show_time(int hours, int minutes, int seconds) {
 
  set_displays(0, seconds % 10);
  set_displays(1, (seconds / 10) % 6);

  set_displays(2, minutes % 10);
  set_displays(3, (minutes / 10) % 6);

  set_displays(4, hours % 10);
  set_displays(5, (hours / 10) % 10);
}

static void apply_switch_update(int *hours, int *minutes, int *seconds) {
  int sw = get_sw();
  /* Use one of the remaining switches to EXIT: choose SW7 (bit 6). */
  if (sw & (1 << 6)) { /* WHY: not part of top-2 or low-6 */
    /* Signal to caller by setting seconds to -1 (sentinel). */
    *seconds = -1;
    return;
  }

  int sel = (sw >> 8) & 0x3;   /* top2 MSBs for instructions */
  int val =  sw & 0x3F;        /* low6 LSBs */

  if (sel == 0x1) {            /* 01: seconds */
    *seconds = val % 60;
  } else if (sel == 0x2) {     /* 10: minutes */
    *minutes = val % 60;
  } else if (sel == 0x3) {     /* 11: hours */
    *hours   = val % 24;
  }
}

/* Your code goes into main as well as any needed functions. */
int main() {
  labinit();

  /* Part (d): startup LED sequence, then continue into part (h). */
  startup_led_counter();

  int hours = 0, minutes = 0, seconds = 0;

  while (1) {
    show_time(hours, minutes, seconds);

    if (get_btn()) {                /* read switches and update */
      apply_switch_update(&hours, &minutes, &seconds);
      if (seconds < 0) break;       /* exit if flag is set to -1*/

      delay(1);
    }

    delay(2);                       /* ~1 second */
    seconds++;
    if (seconds >= 60) { seconds = 0; minutes++; }
    if (minutes >= 60) { minutes = 0; hours = (hours + 1) % 24; }
  }

  /* End program after exit switch is used. */
  return 0;

  // Enter a forever loop
  while (1) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay(10);          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
  }
}

/*
Questions

1) 15 seconds. Since the loop increments from value=0 to value=15 (breaks when (value & 0xF) == 0xF),
   with one ~1 s delay per step → 15 iterations → ~15 s.

2) a0 (x10). Per the RISC-V ABI, a 32-bit int return value is delivered in register a0.
*/