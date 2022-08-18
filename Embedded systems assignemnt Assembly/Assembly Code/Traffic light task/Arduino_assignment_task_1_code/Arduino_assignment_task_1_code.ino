//To control the pedestrain light you need to enter the number 1 key for the first traffic ligth set and use number 2 key  to controll the second sets pedestrain light. 


//Setup function runs and setups all necessary features. Ran once.
void setup() {
 Serial.begin(9600);//Establishes serial communication between Arduino board and serial montior.
 //asm is the start of the embed assembler instruction set within C.
 //volatile means the variable can be modified from outside
 asm volatile (
 // load immediate the binary value 11111111 into register 19 for output pins .
 "ldi r19, 0xFF  \n" //r19 = 11111111 
 //Store register 19 to I/o location 0x04 portB.
 "out 0x04, r19 \n" //portB = 11111111.
 //All pins are set to output.
  );
}
//C loop function. will loop forever performing the traffic light sequence.
void loop(){
asm volatile (
  "start:         \n" //Label tells the assembler where to start execution.
  "rcall .t1      \n"//Calls label t1 which holds the first set of four phase traffic light.
  "cbi 0x05, 0x04 \n"//Clears bit 0x04 (pin 12)from I/O register 0x05 (Turn OFF Red LED).
  );
ped1Light();
asm volatile( 
  "rcall .t2      \n"//Calls label t2 which holds the second set of four phase traffic light.
  "cbi 0x05, 0x00 \n"//Clears bit 0x00 (pin 53)from I/O register 0x05 (Turn OFF Ped LED).
  );
ped2Light();
asm volatile(
  "rjmp start   \n" //use rjmp instead of jmp is faster, jumps back to start ti repeat the sequence.
 //double loop tick*tick: used to tell code to stop execution for a couple of second.
//r8 and r9 both decrement from 255. However, different values can be used. 
//Max value 255*255 65,025.  
  "delay1:       \n" //Delay1 label start of nested delay loop
  "dec r8        \n"//decrement register 8 by 1.
  "brne delay1   \n" //branche if delay1 is not equal 0.
  "dec r9        \n" //decrement register 9 by 1.
  "brne delay1   \n"//branche if delay1 is not equal to 0.
  "dec r16       \n" //decrement register 16 by 1.
  "brne delay1   \n"//branche if delay1 is not equal 0.
  "ret           \n"//return back to subroutine
//triple loop tick*tick*tick: Used to code to stop execution however for longer.
//r8 ,r9, r10 all have the value 255 which slowlys decrements to give the delay.
//Max value is 255*255*255 16,581,375. 
  "delay2:       \n" //Delay2 label start of nested delay loop
  "dec r8        \n"//decrement register 8 by 1.
  "brne delay2   \n"//branche if delay1 is not equal 0.
  "dec r9        \n"//decrement register 9 by 1.
  "brne delay2   \n"//branche if delay1 is not equal to 0.
  "dec r10       \n"//decrement register 10 by 1.
  "brne delay2   \n"//branche if delay1 is not equal to 0.
  "dec r16       \n" ///decrement register 16 by 1.
  "brne delay2   \n"//branche if delay1 is not equal 0
  "ret           \n"//return back to subroutine
//Start of the first set of four phase traffic light sequence.
  ".t1:           \n"//t1 label used to call traffic light sequence.
//phase 1: red light only.
  "Sbi 0x05, 0x01 \n"//set bit 0x01 (pin 52) in I/O register 0x05(turns on Red LED). 
  "Sbi 0x05, 0x05 \n"//set bit 0x05 (pin 10) in I/O register 0x05(turns on Red LED). 
  "ldi r16,140    \n"//Load immediately 140 into register 16 (which is 2 seconds). 
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
//phase 2: Red and ambler lighton.
  "sbi 0x05, 0x02 \n"//set bit 0x02 (pin 51)in I/O register 0x05 on (turns on Amber LED). 
  "ldi r16,173    \n"//Load immediately 173 into register 16 (which is 3 seconds).  
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
//phase 3: green light on only.
   "cbi 0x05, 0x01\n"//Clears bit 0x01 (pin 52)from I/O register 0x05 (Turn OFF Red LED). 
   "cbi 0x05, 0x02\n"//Clears bit 0x02 (pin 51)from I/O register 0x05 (Turn OFF Amber LED).
   "sbi 0x05, 0x03\n"//set bit 0x03 (pin 50) in I/O register 0x05(turns on Green LED).
   "ldi r16,3     \n"//Load immediately 3 into register 16 (which is 10 seconds).               
   "rcall delay2  \n"//call function delay2 which will delay the execution of code.
//phase 4: ambler light on only.
   "cbi 0x05, 0x03\n"//Clears bit 0x03 (pin 50)from I/O register 0x05 (Turn OFF Green LED).
   "sbi 0x05, 0x02\n"//set bit 0x02 (pin 51)in I/O register 0x05 on (turns on Amber LED). 
   "ldi r16,140   \n"//Load immediately 140 into register 16 (which is 2 seconds). 
   "rcall delay1  \n"//call function delay1 which will delay the execution of code.
//shifts back to red.
   "sbi 0x05, 0x01\n"//set bit 0x01 (pin 52) in I/O register 0x05(turns on Red LED). 
   "cbi 0x05, 0x02\n"//Clears bit 0x02 (pin 51)from I/O register 0x05 (Turn OFF Amber LED).
   "ldi r16,100  \n"//Load immediately 100 into register 16 (which is 1 seconds). 
   "rcall delay1 \n"//call function delay1 which will delay the execution of code.
   "ret          \n"//return back to subroutine
   
//Start of the second set of four phase traffic light sequence.
  ".t2:           \n" 
  //phase 1: red light only.
  "Sbi 0x05, 0x01 \n"//set bit 0x01 (pin 52) in I/O register 0x05(turns on Red LED). 
  "Sbi 0x05, 0x05 \n"//set bit 0x05 (pin 10) in I/O register 0x05(turns on Red LED). 
  "ldi r16,140    \n"//Load immediately 140 into register 16 (which is 2 seconds). 
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
  //phase 2: Red and ambler lighton.
  "sbi 0x05, 0x06 \n"//set bit 0x06 (pin 11) in I/O register 0x05(turns on Amber LED). 
  "ldi r16,173    \n"//Load immediately 173 into register 16 (which is 3 seconds). 
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
  //phase 3: green light on only.
   "cbi 0x05, 0x05\n"//Clears bit 0x05 (pin 10)from I/O register 0x05 (Turn OFF Red LED). 
   "cbi 0x05, 0x06\n"//Clears bit 0x06 (pin 11)from I/O register 0x05 (Turn OFF Amber LED). 
   "sbi 0x05, 0x07\n"//set bit 0x07 (pin 12) in I/O register 0x05(turns on Green LED). 
   "ldi r16,3     \n"//Load immediately 3 into register 16 (which is 10 seconds).               
   "rcall delay2  \n"//call function delay2 which will delay the execution of code.
  //phase 4: ambler light on only.
   "cbi 0x05, 0x07\n"//Clears bit 0x07 (pin 12)from I/O register 0x05 (Turn OFF Green LED). 
  "sbi 0x05, 0x06 \n"//set bit 0x06 (pin 11) in I/O register 0x05(turns on Amber LED). 
   "ldi r16,173    \n"//Load immediately 173 into register 16 (which is 3 seconds). 
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
  //shifts back to red.
  "Sbi 0x05, 0x05 \n"//set bit 0x05 (pin 10) in I/O register 0x05(turns on Red LED). 
   "cbi 0x05, 0x06\n"//Clears bit 0x06 (pin 11)from I/O register 0x05 (Turn OFF Amber LED). 
   "ldi r16,100   \n"//Load immediately 100 into register 16 (which is 1 seconds).  
  "rcall delay1   \n"//call function delay1 which will delay the execution of code.
   "ret           \n"
    );
 }
//Pedlight function is design to detect when the 1 key is pressed to trigger the ped light for set 1.
void ped1Light(){
  asm volatile (
   "ldi r16,50  \n"//Load immediately 50 into register 16 (which is half a seconds). 
   "rcall delay1\n"//call function delay1 which will delay the execution of code.
  );
  int i;
  for(i =0; i <=100; i++){
volatile char data = Serial.read();//Seria;.read will scan what ever is entered within the serial monitor.     
  if(data == '1'){//If data reads 1. then perform the following.
     asm volatile (
      "sbi 0x05, 0x00 \n"//set bit 0x00 (pin 11) in I/O register 0x05(turns on PED1 LED).
     );      
  };    
  }  
}
//Ped2ight function is design to detect when the 2 key is pressed to trigger the ped light for set 2.
void ped2Light(){
  asm volatile (
   "ldi r16,50    \n"//Load immediately 50 into register 16 (which is half a seconds).
   "rcall delay1\n"//call function delay1 which will delay the execution of code.
  );
  int i;
  for(i =0; i <=100; i++){
volatile char data = Serial.read(); //Seria;.read will scan what ever is entered within the serial monitor.        
  if(data == '2'){//If data reads 2. then perform the following.
     asm volatile (
      "sbi 0x05, 0x04 \n"//set bit 0x04 (pin 11) in I/O register 0x05(turns on PED2 LED).
     );
   }; 
  }      
}
