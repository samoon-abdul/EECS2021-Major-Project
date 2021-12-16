module pulseWidthMod (clockC,duty,period,outPut);
    reg [8:0] counterZero=0; // represents timer 0
    reg [8:0] counterTwo=0; // represents timer 2
    reg[16:0] counterOne=0; // represents timer 1 

    input clockC;
    input [8:0] duty;
    input [8:0] period;

    output reg outPut;
    reg [8:0] counter;

    // we will only use timer 0
    // infinite loop
    always @ (negedge clockC) 
    // start loop 
    begin 
    // if the counter value is less than the duty cycle value we //will turn output high or 1 
        if (counterZero<duty)
            assign outPut=1; 
    // else output signal will be low or 0
        else 
            assign outPut=0;
    // if the counter registers value is greater than the period 
    // of the cycle then we will reset counterOnes value which //simulates what happens when we reach the TOP value
        if (counterZero>=period) 
            counterOne = 0; 
    // else we will increment the counter value 
        else 
            counterZero = counterZero + 1; 
    // This allows for a constant display of the counterOne value
        $display("%g",counterZero); 

    end
endmodule