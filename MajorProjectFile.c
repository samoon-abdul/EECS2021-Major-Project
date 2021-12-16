#ifndef F_CPU
#define F_CPU 16000000UL
#endif
        

/* Imports/Include statements*/
#include <xc.h> 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>


/* Global variables and definition */
#define LED      PB0
#define LED_DDR  DDRB
#define LED_PORT PORTB

/* Buttons */
#define BUTTON_ONE PC5
#define BUTTON_TWO PC4
#define BUTTON_PIN PINC
#define BUTTON_PORT PORTC

/* Motor wires */
#define directionalPin1 (1<<PB1)
#define stepPin1 (1<<PB2)
#define directionalPin2 (1<<PD7)
#define stepPin2 (1<<PD6)
#define stepPinPort1 PORTB
#define stepPinPort2 PORTD

#define DELAYTIME 200
#define PULSE_WIDTH 0x40 // 64


/* Function declarations */
void clockwiseRotation(void);
void antiClockwiseRotation(void);
void pulseWidthModulationStart(void);
void delayAsm(int);
void incrementDelay(int);

int main(void) {
    //-------- ASM for Variable Declaration --------// 
    // stack init to prepare for volt. ints
    // This is asm code that serves the same func. as var declaration 
    // It creates space based on bit size, we have 2 bits for ints
    // and 4 for floats

    // vol for startFirstWhile
    asm("STD Y+2, R1\n"
            "STD Y+1, R1");

    // vol for startSecondWhile
    asm("STD Y+4, R1\n"
            "STD Y+3, R1");

    //vol for stopFirstMotor
    asm("LDI R24, 0x01\n"
            "LDI R25, 0x00\n"
            "STD Y+6, R25\n"
            "STD Y+5, R24");

    //vol for stopSecondMotor
    asm("STD Y+8, R25\n"
            "STD Y+7, R24");

    //float vol for tempVar
    asm("STD Y+9, R1\n"
            "STD Y+10, R1\n"
            "STD Y+11, R1\n"
            "STD Y+12, R1");

    //float var for regenerativeVariable
    asm("STD Y+17, R1\n"
            "STD Y+18, R1\n"
            "STD Y+19, R1\n"
            "STD Y+20, R1");



    // -------- Variables --------- //
    int volatile startFirstWhile = 0;
    int volatile startSecondWhile = 0;
    int volatile stopMotorOne = 1;
    int volatile stopMotorTwo = 1;
    float volatile tempVar = 0;
    float volatile delayVariable = 0;
    float volatile regenerativeVariable = 0;
    float volatile batteryVariable = 0;
    // -------- Initializations --------- //
    pulseWidthModulationStart();

    //            BUTTON_PORT |= (1 << BUTTON_ONE); /* initialize BUTTON_ONE for button pressing */
    //            DDRC &= ~(1 << BUTTON_ONE); /* set as output */
    //            BUTTON_PORT |= (1 << BUTTON_TWO); /* initialize BUTTON_TWO for button pressing */
    //            DDRC &= ~(1 << BUTTON_TWO); /* set as output */
    //            DDRB = 0x01; /* set up PB0 as LED output */
    asm("SBI 0x08, 5"); /* initialize BUTTON_ONE for button pressing */
    asm("CBI 0x07, 5"); /* set as output */
    asm("SBI 0x08, 4"); /* initialize BUTTON_TWO for button pressing */
    asm("CBI 0x07, 4"); /* set as output */
    asm("LDI R24, 0x01\n" /* set up PB0 as LED output */
            "OUT 0x04, R24");


    asm("SEI"); /* enable interrupts */


    // ------ Event loop for Button LED Sequence ------ //
    while (1) {

        // Button two is pressed, turn LED off, start first motor whileLoop 
        // and break
        if ((PINC & (1 << BUTTON_TWO))) {// look for button press //
            LED_PORT = (1 << LED);
            //asm("LDI R24, 0x01\n"
            //"OUT 0x05, R24");

            startFirstWhile = 1;

            ////int var for changing first startWhile to 1
            //asm("STD Y+2, R25\n"
            //"STD Y+1, R24");

            break;
        }

        // Button one is pressed, increment delay variable while also 
        // checking if it is higher than 10,000. If it is, break 
        // and start first motor whileLoop and break 
        if ((PINC & (1 << BUTTON_ONE)) == 0) { // look for button press //

            LED_PORT = (0 << LED); // pressed, turn LED on// 
            //asm("OUT 0x05, R1");
            if (delayVariable > 10000) {
                LED_PORT = (1 << LED);
                // start first while in asm
                //asm("LDI R24, 0x01\n"
                //"OUT 0x05, R24");
                startFirstWhile = 1;
                break;
            }
            delayVariable = delayVariable + 0.2;

        } else { // Turn LED off
           // PORTB = (1 << LED);
            asm("LDI R24, 0x01\n"
            "OUT 0x05, R24");

        }



    }





    //Delay Converter to usable numbers, OCROA count used to divide 
    // regenerativeVariable is based on 30% energy loss assumption 
    if (delayVariable > 0) {
        delayVariable = ((delayVariable) / 256);
        regenerativeVariable = (delayVariable * 0.70);
    }


    // ------ Event Loop to spin driving motor clockwise ------ //
    if (startFirstWhile) {
        // Turn DDRB on for dir pin1
        DDRB |= directionalPin1;
        //asm("SBI 0x04, 1");
        // Turn DDRB on for step pin1
        DDRB |= stepPin1;
        tempVar = 0;
        //asm("SBI 0x04, 2");

        // Increment tempVar, if it is greater than delayVar then break and 
        // turn on second motor while loop
        while (1) {
            if (tempVar > delayVariable) {
                DDRB &= ~(1 << directionalPin1);
                //asm("CBI 0x04, 2");
                DDRB &= ~(1 << stepPin1);
                //asm("CBI 0x04, 4");
                startSecondWhile = 1;

                //                        // asm to change startSecondWhile to true or 1
                asm("STD Y+4, R25\n"
                    "STD Y+3, R24");

                break;
            } else {
                tempVar = tempVar + 1.5;
                clockwiseRotationMotor1();


            }
        }
    }
    // ------ Event Loop to spin regenerative motor counterclockwise ------ //
    if (startSecondWhile) {
        //DDRD |= directionalPin2;
         asm("SBI 0x0A, 7");
        //DDRD |= stepPin2;
         asm("SBI 0x0A, 6");
        // set tempVar to 0 so that we can use it again
        tempVar = 0;


        // Increment tempVar, if it is greater than delayVar then break and 
        // turn on second motor while loop
        while (1) {
            if (tempVar > regenerativeVariable) {
                //DDRB &= ~(1 << directionalPin1);
                asm("CBI 0x04, 2");
                //DDRB &= ~(1 << stepPin1);
                asm("CBI 0x04, 4");
                break;
            } else {

                tempVar = tempVar + 2.5;
                batteryVariable = batteryVariable + 0.5;
                antiClockwiseRotationMotor2();

            }
        }
    }

    return 0;
} /* End of main */

/* Method that spins first motor clockwise, simulates pulses (PWM)
 * to spin the motor clockwise. Delay is needed so that the motor can stop
 * and the viewer can see it spin, otherwise we wouldn't have the delays
 *
 * @param none
 * @return none 
 * 
 */
void clockwiseRotationMotor1(void) {

    unsigned volatile char i;
    // High pulse to spin Motor clockwise



    stepPinPort1 |= directionalPin1;
    //asm("SBI 0x05, 1");
    //Rotate One full cycle

    stepPinPort1 |= stepPin1;
    //asm("SBI 0x05, 2");
    _delay_ms(25);
    stepPinPort1 &= ~stepPin1;
    // asm("CBI 0x05, 2");
    _delay_ms(50);

}

/* Method that is used to initialize PWM
 *
 * @param none
 * @return none 
 * 
 */
void pulseWidthModulationStart() {
    //OCR1AL; /* Load Pulse width */
    //OCR1AH;
    //DDRD PortD.5 as o/p */
    //TCCR1A /* Non-Inverted PWM */
    //TCCR1B /* Start PWM */
    int x;
    asm("LDI R24, 0x40\n"
            "STS 0x88, R24"); /* Load Pulse width */
    asm("STS 0x89, R1"); // OCR1AH set to 0 REG
    asm("SBI 0x0A, 5"); /* PortD.5 as o/p */
    asm("LDI R24, 0x81\n" /* Non-Inverted PWM */
            "STS 0x80, R24"); /* Non-Inverted PWM */
    asm("LDI R24, 0x01\n" /* Start PWM */
            "STS 0x81, R24"); /* Start PWM */

}

/* Method that spins second motor counterclockwise, simulates pulses (PWM)
 * to spin the motor counterclockwise. Delay is needed so that the motor can stop
 * and the viewer can see it spin, otherwise we wouldn't have the delays
 *
 * @param none
 * @return none 
 * 
 */

void antiClockwiseRotationMotor2(void) {

    unsigned volatile char i;
    //send low pulse for anti-clockwise direction
    stepPinPort2 &= ~directionalPin2;
    //asm("CBI 0x0B, 7");
    //send 200 pulses to rotate One full cycle

    stepPinPort2 |= stepPin2;
    //asm("SBI 0x0B, 6");
    _delay_ms(25);
    stepPinPort2 &= ~stepPin2;
    //asm("CBI 0x0B, 6");
    _delay_ms(25);


}

/* ASM function used to decrement values. Was not used in this as inline was
 * not as efficient in testing of code. This inline code simulates a while loop
 * and decrements the param i by 1 until it is 0 
 * 
 *
 * @param i the integer that is decremented from
 * @return none 
 * 
 */
void delayAsm(int i) {
    // pushes stacks and calls on bytes of memory size 0x5E
    asm("PUSH R28\n"
            "PUSH R29\n"
            "RCALL 0x5E\n"
            "IN R28, 0x3D\n"
            "IN R29, 0x3E\n");

    //loads into registers and sets direction  flag in register , 2 for r25 and 1 in r24 
    asm("LDI R24, 0x64\n"
            "LDI R25, 0x00\n"
            "STD Y+2, R25\n"
            "STD Y+1, R24");

    // return back to main and load direction flags into registers 
    asm("LDD R24, Y+1\n"
            "LDD R25, Y+2\n"
            "CALL main");
}

//void delayIncre(int i){
//asm("        std Y+1,R1\n"
//    "   std Y+2,R1\n"
//    "     std Y+3,R1\n"
//    "   std Y+4,R1\n"
//    "    ldd r24,Y+1\n"
//    "    ldd r25,Y+2\n"
//    "    ldd r26,Y+3\n"
//    "    ldd r27,Y+4\n"
//    "    std Y+5,r24\n"
//    "    std Y+6,r25\n"
//    "    std Y+7,r26\n"
//    "    std Y+8,r27\n"
//    "    ldi r18,0\n"
//    "    ldi r19,0\n"
//    "    ldi r20,lo8(-64)\n"
//    "    ldi r21,lo8(63)\n"
//    "    ldd r22,Y+5\n"
//    "    ldd r23,Y+6\n"
//    "    ldd r24,Y+7\n"
//    "    ldd r25,Y+8\n"
//    "    rcall __addsf3\n"
//    "    std Y+9,r22\n"
//    "    std Y+10,r23\n"
//    "    std Y+11,r24\n"
//    "    std Y+12,r25\n"
//    "    ldd r24,Y+9\n"
//    "    ldd r25,Y+10\n"
//    "    ldd r26,Y+11\n"
//    "    ldd r27,Y+12\n"
//    "    std Y+1,r24\n"
//    "    std Y+2,r25\n"
//    "    std Y+3,r26\n"
//    "    std Y+4,r27\n"
//    "    nop\n"
//    "adiw r28,12");
//    
//}