#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h> 
int array[15]; //Array will be used to store our 15 random numbers and to be sorted.
int size = 15;//This store the size of the array to be used with loops.
int Display;//This is used within the inline assebmly to display the swapped cotnetns to screen.
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
void loop() {
int value = Serial.available();//This will read the user input from serial monitor and store it in variable value.
if(value > 0){//Variable value will be used in a basic if statement which start the program when value is greater than 0. 

int k;//K will be used within our first loop to store random values within the array.
for(k = 0; k < size ; k++){
 int Random =rand()%255;//Integer Random will generate a random number between 0 -255.
//Using the method rand() which is a feature of C this will generate a random number.
// i divided the rand() number by the max amount which 255 and then used "%" to get its reaminder which will be less than the max.
asm volatile(//This section of the array moves the random number into the index postion within the array.
"mov %0, %1    \n"//Using assembly i can create two output oncstraints which be used to store random values itno the array.
  :"+r" (array[k]),"+r" (Random)//"+r" will use any register 0-31 and to reuse the same memory location for input and output. 
);
}

Serial.print("\n Unsorted:");//Intergrated print will display information onto the serial monitor.
//Displays the unsorted array list into the serial monitor.
int f;
for(f = 0; f <size; f++){//A new loop which simply displays the unsorted array.
Serial.print(array[f]);
Serial.print(" ");
}
Serial.print("\n");
//The start of my bubble sort algortihm. This uses two arrays which which perfrom 210 iteration.
int i;
for(i =0; i <=size; i++){
int j;
for(j =0; j < size-1; j++){
  
int *CurrentNode = &array[j]; //Points to the address of the currentelement
int *NextNode = &array[j+1];//Points to the address of the next element after the currrent.
int temp; //This will be used to store the current value temporarily for the current & next node to swap. 
int pos1 = array[j];//This is the index position of the currrent node.
int pos2 = array[j +1];//This is the index position of the next node.

 asm (
"mov r20, %1    \n"//current node is moved into regsiter 20.
"mov r21, %2    \n"//Next node is moved into regsiter 21.
"mov r22, %3    \n"//pos1 is moved into regsiter 22.
"mov r23, %4    \n"//pos2 is moved into regsiter 23.
"mov r30, %5    \n"//temp is moved into regsiter 30.
"cp r22, r23    \n"//Compare r23 to r24 (Currentnode to nextnode)
"BRSH  3f       \n"//branch to 3 if same or highier.
"rjmp 4f        \n"//jump to branch 4 if they are not the same or highier.
"3:             \n"//branch 3 label.
"mov r30, r20   \n"//mov current node into temp node
"mov r20, r21   \n"//mov next node into current node.
"mov r21, r30   \n"//mov temp value into next node.
"mov %3, r20    \n"//Store the new current node value into indexed  pos 1 
"mov %4, r21    \n"//Store the new next node value into indexed  pos 2
"4:             \n"//Brnach 4 label will end this iteration with no altered array if the condition are not met.
  :"=r" (Display),"+r" (*CurrentNode), "+r" (*NextNode)  :"r" (pos1), "r" (pos2),"r" (temp):"r20","r21","r22","r24","r30"
 );//       0                 1                  2               3           4           5        
}//Use of different output and input constraitns as well as a list of clobbers.
}
int smallest = array[0];//the first element in the array when sorted will be the smallet value.
int largest = array[size-1];//The last value witihn the array when sorted will be the largest value.
Serial.print("Smallset: ");
Serial.print(smallest);//Prints the smallest value to the serial monitor.
Serial.print("\n");
Serial.print("Largest: ");
Serial.print(largest);//pritns th largest number to the array.
Serial.print("\n");
Serial.print("   sorted:");

int c;
for(c = 0; c <size; c++){//This for loop will iterate printing out the new sorted array list to the serial monitor.
Serial.print(array[c]);
Serial.print(" ");
}
Serial.print("\n");
asm volatile(//Start of a new instruction set which will display both the small & largest number in 8 bit format using LED'S.
"rcall .LED    \n"//Calls functil label LED which will display the binary format.
"rjmp .end     \n"//Jumps to label end which will end this instruction section.
//double loop tick*tick: used to tell code to stop execution for a couple of second.
//r8 and r9 both decrement from 255. However, different values can be used. 
//Max value 255*255 65,025.  
"delay:        \n" //Delay1 label start of nested delay loop
"dec r8        \n"//decrement register 8 by 1.
"brne delay    \n" //branche if delay1 is not equal 0.
"dec r9        \n" //decrement register 9 by 1.
"brne delay    \n"//branche if delay1 is not equal to 0.
"dec r16       \n" //decrement register 16 by 1.
"brne delay    \n"//branche if delay1 is not equal 0.
"ret           \n"//return back to subroutine       \n"//return back to the call

".LED:         \n"//Label for LED to be called.
"mov r17, %0   \n"//mov the smallest number into regsiter 17
"out 0x05, r17 \n"//This will turn on pin 53 high which is the red traffic light for traffic light 1.
"ldi r16,255    \n"//Loading register 16 with 2 second 
"rcall delay   \n"// This will call branch delay1 which will dealy the execution of code.
"mov r17, %1   \n"
"out 0x05, r17 \n"//This will turn on pin 53 high which is the red traffic light for traffic light 1.
"ldi r16,255   \n"//Loading register 16 with 2 second 
"rcall delay   \n"// This will call branch delay1 which will dealy the execution of code.
"mov r17, 0    \n"
"ret           \n"

".end:         \n"
::"r" (smallest), "r" (largest):"r16"
  );
  }
}
