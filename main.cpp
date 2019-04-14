#include "mbed.h"

#define BTN_1 0x01  // Button 1 interrupt trigger
#define BTN_2 0x02  // Button 2 interrupt trigger
#define TICKER 0x04 // Ticker interrupt trigger
#define TASK_1 0x21 // 100001 = (WEST<-EAST = GREEN / NORTH->SOUTH = RED)
#define TASK_2 0x32 // 100010 = (WEST<-EAST = YELLOW / NORTH->SOUTH = RED, YELLOW)
#define TASK_3 0xC  // 001100 = (WEST<-EAST = RED / NORTH->SOUTH = GREEN)
#define TASK_4 0x16 // 010110 = (WEST<-EAST = RED, YELLOW / NORTH->SOUTH = RED)
#define INTERVAL 10 // GREEN lights will last at least 10 seconds

typedef enum tState 
{ 
  STATE_A,  // WEST<-EAST = GREEN / NORTH->SOUTH = RED
  STATE_B,  // WEST<-EAST = YELLOW / NORTH->SOUTH = RED, YELLOW
  STATE_C,  // WEST<-EAST = RED / NORTH->SOUTH = GREEN
  STATE_D   // WEST<-EAST = RED, YELLOW / NORTH->SOUTH = RED
} tState;

tState state;
char trigger; // Bit set to indicate which interrupt triggers

// BusOut for LEDs
BusOut task(D2, D3, D4, D5, D6, D7);

// Interrupts with buttons
InterruptIn button1(D8);
InterruptIn button2(D9);
void int_button1() {trigger |= BTN_1;}
void int_button2() {trigger |= BTN_2;}

// Timeout or ticker for normal cycle
Ticker ticker;
void ticker_trig() {trigger |= TICKER;}

// Green lights will last at least 10 seconds, if no cars are coming from another direction
void resetTicker() {
    ticker.detach(); 
    ticker.attach(&ticker_trig, INTERVAL);
}

// Task to do if interrupt occurs in certain state
void do_task(int n) {
    
    task = n;
    
    // To change from GREEN to RED, implement a YELLOW light of 2 seconds
    if(state == STATE_B || state == STATE_D) {
        wait(2);
    }
    else {
        resetTicker();
    }
}

int main()
{
    ticker.attach(&ticker_trig, INTERVAL);
    button1.mode(PullDown);
    button1.rise(&int_button1);
    button2.mode(PullDown);
    button2.rise(&int_button2);
    
    state = STATE_A;
    do_task(TASK_1);

    while(1) {
        
        while (!trigger) {
            sleep();
        }
        
        switch (state) {
            
            case STATE_A:
            // BTN_2 => If cars are only coming in one direction (Button is pressed), move to GREEN in that direction.
            // TICKER => If no cars are coming, stay in a GREEN state. (Cycle through all four states).
                if (trigger & BTN_2 || trigger & TICKER) {
                    state = STATE_B;
                    do_task(TASK_2);
                    state = STATE_C;
                    do_task(TASK_3);
                }
            trigger = 0;
            break;
            
            case STATE_C:
            // BTN_1 => If cars are only coming in one direction (Button is pressed), move to GREEN in that direction.
            // TICKER => If no cars are coming, stay in a GREEN state. (Cycle through all four states).
                if (trigger & BTN_1 || trigger & TICKER) {
                    state = STATE_D;
                    do_task(TASK_4);
                    state = STATE_A;
                    do_task(TASK_1);
                }
            trigger = 0;
            break;
            
        } 
    } 
}
