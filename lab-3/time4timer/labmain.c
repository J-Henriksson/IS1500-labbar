/* main.c */

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

void handle_interrupt(unsigned cause) {}
void labinit(void) {}

void set_leds(int led_mask) {
  /* A1: MMIO write to LEDs at 0x04000000 */
  volatile int* const leds = (volatile int*) 0x04000000;
  *leds = led_mask;
}

void set_displays(int display_number, int value) {
  /* A1: 6 seven-seg displays, rightmost is index 0 */
  if (display_number < 0 || display_number > 5) return; 
  volatile char* const address = (volatile char*) (0x04000050 + display_number * 0x10);
  *address = display_digits[value % 10]; /* A1: active-low 7-seg digit LUT */
}

int get_sw(void) {
  /* A1: switches at 0x04000010, return 10 LSBs */
  volatile int * const sw = (volatile int *)0x04000010;
  int v = *sw;
  return v & 0x3FF;           
}

int get_btn(void) {
  /* A1: BTN2 at 0x040000D0, return LSB only */
  volatile int * const btn = (volatile int *)0x040000D0;
  int v = *btn;
  return v & 0x1;             
}

/* A1(d): 4-bit LED counter 0→15 with ~1 s delays (approx) */
static void startup_led_counter(void) {
  int value = 0;
  set_leds(0);                
  while (1) {
    delay(10);                  
    value++;
    set_leds(value); 
    if ((value & 0xF) == 0xF) break;
  }
}

/* A1(h): show HH:MM:SS across six digits (hours left, seconds right) */
static void show_time(int hours, int minutes, int seconds) {
  set_displays(0, seconds % 10);
  set_displays(1, (seconds / 10) % 6);
  set_displays(2, minutes % 10);
  set_displays(3, (minutes / 10) % 6);
  set_displays(4, hours % 10);
  set_displays(5, (hours / 10) % 10);
}

/* A1(h): BTN2 triggers read of switches:
   MSB pair selects {sec|min|hour}, low-6 provide value; SW7 exits */
static void apply_switch_update(int *hours, int *minutes, int *seconds) {
  int sw = get_sw();
  if (sw & (1 << 6)) { *seconds = -1; return; }
  int sel = (sw >> 8) & 0x3;
  int val =  sw & 0x3F;
  if (sel == 0x1) { *seconds = val % 60; }
  else if (sel == 0x2) { *minutes = val % 60; }
  else if (sel == 0x3) { *hours   = val % 24; }
}

/* === Assignment 2 timer (polling) ======================================= */

char timeoutcount = 0;

/* A2: Timer MMIO registers */
volatile unsigned int* timer_status  = (volatile unsigned int*) 0x04000020;
volatile unsigned int* timer_control = (volatile unsigned int*) 0x04000024;
volatile unsigned int* timer_periodl = (volatile unsigned int*) 0x04000028;
volatile unsigned int* timer_periodh = (volatile unsigned int*) 0x0400002C;

/* A2: timeout flag; clear & restart on hit */
static char timer_TO() {
  if (*timer_status & 0x1) {
    *timer_status  = 0x0;   /* reset event-flag to 0 (board-specific clear) */
    *timer_control = 0x0004;/* restart timer (periodic) */
    return 1;
  }
  return 0;
}

/* A2:100 ms period @ 30 MHz: 2,999,999 (0x002D_C6BF) */
static void timer_init_100ms(void) {
  *timer_periodl = 0xC6BF;  /* low 16 bits */
  *timer_periodh = 0x002D;  /* high 16 bits */
  *timer_control = 0x0004;  /* start */
}

/* A2: helpers to write BCD nibbles in mytime (MM:SS) */
static void set_seconds(int tens, int ones) {
  mytime = (mytime & 0xFF00) | ((tens << 4) | ones);
}
static void set_minutes(int tens, int ones) {
  mytime = (mytime & 0x00FF) | ((tens << 12) | (ones << 8));
}

/* === Assignment 2 main =================================================== */

int main() {
  timer_init_100ms();   /* A2: use precise HW timer instead of delay() */
  startup_led_counter();/* A1(d): initial LED count */

  int hours = 0;
  int led_mask = 0;
  int prev_btn = 0;

  while (1) {
    if (timer_TO()) {            /* A2: one 100 ms event occurred */
      timeoutcount++;            /* count to 10 */
    }

    if (timeoutcount == 10) {    /* A2(c): once per second */
      timeoutcount = 0;

      if (led_mask <= 0xF) { set_leds(led_mask); led_mask++; } /* keep A1 visual */

      /* A1(h): write current time to six displays */
      set_displays(0,  mytime & 0xF);
      set_displays(1, (mytime >> 4) & 0xF);
      set_displays(2, (mytime >> 8) & 0xF);
      set_displays(3, (mytime >>12) & 0xF);
      set_displays(4, hours % 10);
      set_displays(5, hours / 10);

      /* A1: print time to terminal and advance BCD MM:SS once per second */
      time2string(textstring, mytime);
      display_string(textstring);
      tick(&mytime);

      /* A1: when 59:59, increment hours */
      if ((mytime & 0xFFFF) == 0x0000) {
        hours++;
      }
    }

    /* A1(h): BTN2 edge → apply switch-driven edits; SW7 exits */
    int b = get_btn();
    if (b && !prev_btn) {
      int switches = get_sw();
      int value = switches & 0x03F;
      int ones = value % 10;
      int tens = value / 10;

      switch (switches & 0x300) {
        case 0x100: set_seconds(tens, ones); break;
        case 0x200: set_minutes(tens, ones); break;
        case 0x300: hours = value;           break;
        default: break;
      }

      if (switches & 0x040) break; /* exit */
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
