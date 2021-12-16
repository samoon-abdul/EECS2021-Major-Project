module testPWM;
 // Snippets of this Code was inspired by code from FPGA4STUDENT
 // Accredted in Transcript
 // Variable declarations 
 // This is code to test that PWM works
    reg clockC=0;
    wire outPut;
    reg counter=0; 
  // You can change Time to how many cycles we want
  // Delay needed for validation and method calls to be instantiated
  // End if we have reached the timer value
    always #1 
        if($time == 2) 
            $finish; 
    always #1 
        clockC =! clockC;
 

// You would comment out the ones you wish to run
// For ease of visual access, only run one at a time
 
//duty cycle = 10%, period = 2 cycles
    pulseWidthMod periodTwoCycleTwo(clockC,1,2,outPut); 
// //duty cycle = 30%, period = 2 cycles
//   pulseWidthMod periodTwoCycleThirty(clockC,3,2,outPut); 
// //duty cycle = 70%, period = 2 cycles
//   pulseWidthMod periodTwoCycleSeventy(clockC,7,2,outPutr); 
// //duty cycle = 80%, period = 2 cycles
//   pulseWidthMod periodTwoCycleEighty(clockC,8,2,outPut); 
// //duty cycle = 100%, period = 2 cycles
//   pulseWidthMod periodTwoCycleHundred(clockC,10,2,outPut);

    initial begin
        $monitor ("This is the PWM OUTPUT %b This is the CLOCK CYCLE VALUE %b THIS IS THE TIMER VALUE %g and OUR COUNTER VAR IS NOT USED",outPut,clockC,$time);
    end

endmodule