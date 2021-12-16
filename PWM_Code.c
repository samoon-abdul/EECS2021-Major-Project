#ifndef F_CPU
#define F_CPU 16000000UL
#endif



/* Method that is used to initialize PWM
 *
 * @param none
 * @return none 
 * 
 */
void pulseWidthModulationStart() {

    asm("LDI R24, 0x40\n" 
        "STS 0x88, R24"); /* Load Pulse width */
    asm("STS 0x89, R1"); // OCR1AH set to 0 REG 
    asm("SBI 0x0A, 5"); /* PortD.5 as o/p */ 
    asm("LDI R24, 0x81\n" /* Non-Inverted PWM */
        "STS 0x80, R24"); /* Non-Inverted PWM */
    asm("LDI R24, 0x01\n" /* Start PWM */ 
        "STS 0x81, R24"); /* Start PWM */

}

int main() {
    pulseWidthModulationStart();
    //Select Fast PWM output as OC0A pin 
    asm("SBI 0x0A, 6");  

    // Duty cycle of 70% to OCR0A 
    asm("LDI R24, 0xBF\n" 
         "OUT 0x27, R24");
    
    
    //Non-Inverting Fast PWM mode 3 using OCRA
    // 1 << to all COM0A1, WGM01, WGM00 
    asm("IN R24, 0x24\n" 
         "ORI R24, 0xBF\n"
          "OUT 0x24, R24");

    
    //No-Prescalar set for CS00
    asm("IN R24, 0x25\n" 
        "ORI R24, 0x01\n"
        "OUT 0x25, R24"); 

    //infinite while
    while(1);

    return 0;
}
