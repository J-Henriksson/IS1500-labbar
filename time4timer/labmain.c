/* main.c */

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

void handle_interrupt(unsigned cause) {}


void set_leds(int led_mask) {
  volatile int* const leds = (volatile int*) 0x04000000;
  *leds = led_mask;
}

void set_displays(int display_number, int value) {
  if (display_number < 0 || display_number > 5) return; 
  volatile char* const address = (volatile char*) (0x04000050 + display_number * 0x10);
  *address = display_digits[value % 10];
}

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




char timeoutcount = 0;

volatile unsigned int* timer_status  = (volatile unsigned int*) 0x04000020;
volatile unsigned int* timer_control = (volatile unsigned int*) 0x04000024;
volatile unsigned int* timer_periodl = (volatile unsigned int*) 0x04000028;
volatile unsigned int* timer_periodh = (volatile unsigned int*) 0x0400002C;

void labinit(void) {
  *timer_periodl = 0xC6BF;  /* 0x002DC6BF = 2,999,999 */
  *timer_periodh = 0x002D;
  *timer_control = 0x0004;
}

static char timer_TO() {
  if (*timer_status & 0x1) {
    *timer_status  = 0x0;
    *timer_control = 0x0004;
    return 1;
  }
  return 0;
}


static void set_seconds(int tens, int ones) {
  mytime = (mytime & 0xFF00) | ((tens << 4) | ones);
}

static void set_minutes(int tens, int ones) {
  mytime = (mytime & 0x00FF) | ((tens << 12) | (ones << 8));
}

/* === Assignment 2 main =================================================== */

int main() {
  labinit();


  int hours = 0;
  int led_mask = 0;
  int prev_btn = 0;

  while (1) {
    if (timer_TO()) {
      timeoutcount++;
    }

    if (timeoutcount == 10) {
      timeoutcount = 0;
      
      if (led_mask <= 0xF) {
        set_leds(led_mask);
      led_mask++;
    }
      
      if (led_mask > 0xF) {   
      set_displays(0,  mytime & 0xF);
      set_displays(1, (mytime >> 4) & 0xF);
      set_displays(2, (mytime >> 8) & 0xF);
      set_displays(3, (mytime >> 12) & 0xF);
      set_displays(4, hours % 10);
      set_displays(5, hours / 10);

      time2string(textstring, mytime);
      display_string(textstring);
      tick(&mytime);

      if ((mytime & 0xFFFF) == 0x0000) {
        hours++;
      }
      }
    }

    int b = get_btn();
    if (b && !prev_btn) {
      int switches = get_sw();
      int value = switches & 0x03F;
      int ones = value % 10;
      int tens = value / 10;

      switch (switches & 0x300) {
        case 0x100: set_seconds(tens, ones); break;
        case 0x200: set_minutes(tens, ones); break;
        case 0x300: hours = value; break;
        default: break;
      }

      if (switches & 0x040) break;
    }
    prev_btn = b;
  }

  print("program halted");
  while (1) {}
}

/*
Answers

1. In timer_TO(): `*timer_status = 0x0;` clears the flag, then `*timer_control = 0x0004;`
   restarts the timer period.

2. The flag would remain set, so subsequent time-outs would not be detectable (or the device
   would stop generating new events). Our polling would keep seeing the same flag and either
   (a) spin the update block repeatedly without real time elapsing, or (b) never get a new event,
   depending on the timer’s edge/level semantics.

3. `timer_periodh` and `timer_periodl` together form a 32-bit period register that defines the
   reload value (number of clock ticks at 30 MHz). We set `0x002D_C6BF` (2,999,999) to get
   ~100 ms per timeout. `timer_control` starts the timer so it counts down/up against that period.

4. BTN2 edits are edge-detected via `prev_btn`; quick presses are reliable if they cross a loop
   boundary, but contact bounce can produce multiple edges. A simple debounce (e.g., ignore
   further presses for N×100 ms timeouts or require a stable release for a few polls) would make
   it robust. This is easy to add alongside `timeoutcount` using a small cooldown counter.

-------------------------------------------------------------------------------
How we use the timer instead of delay:
- A1 used `delay()` (busy wait) to approximate one second.
- A2 configures the hardware timer for exact 100 ms ticks and polls its event flag.
  We count 10 events → perform the once-per-second update (print, tick, 7-seg).
  This removes busy-waiting and yields precise, crystal-based timing.
*/