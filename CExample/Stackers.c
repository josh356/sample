#include "mbed.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <time.h>

I2C i2c(p28, p27); //inter-integrated circuit, allows data to be sent from the
                   //microcontroller to the LED matrix
 
Adafruit_8x16matrix matrix = Adafruit_8x16matrix(&i2c); //the board,
//most notably has a matrix representing the current state of the board, which
//can be used to toggle the LEDS
//has an array of 8 ints, whose binary denotes which LEDs are ON/OFF

Serial pc(USBTX, USBRX);
InterruptIn button(p8); //Input for the game, as an interupt it can be used at
                        //any time.
DigitalIn reset(p12);   //Used to signal to restart the game
DigitalIn speed1(p14);  //Used in determining the difficulty of the game
DigitalIn speed2(p23);
DigitalIn speed3(p25);
DigitalIn hold(p20);    //determines if the games difficulty holds steady or
                        //increases at the halfway point
int holding = hold;
int mask = 1;
int length = 4;         //how many LEDs scroll across the screen
int height = 0;         //current height
int pos[5] = {0,1,2,3,4}; //positions of the row
int shift = 0;          //will be used as a random offset
int difficulty = (speed3<<2)|(speed2<1)|(speed1);
int delay = 500-difficulty*70; //how long in milliseconds for the line to move
                               //one space
int start_delay = delay;    //holds the value for when the difficulty starts to
                            //increase
int count = 0; //number of games played
clock_t t1;    //used to hold the time

//looks at the board and checks if the given locations LED is on
int getVal(int x, int y){
return(matrix.displaybuffer[x]&(mask<<y))>>y;
}
//sets the designated locations LED to be on or off
void setVal(int x, int y, int val){
 if(val==0)
 {
     matrix.displaybuffer[x]&=(~(mask<<y)); //AND the bit with 0
 }
 else
 {
     matrix.displaybuffer[x] |= (mask<<y); //OR the bit with 1
 }
} 
//swaps the status of the LEDs from two locations
void swap(int x, int y, int x2, int y2){
    int a = getVal(x,y);
    int b = getVal(x2,y2);
    setVal(x,y,b);
    setVal(x2,y2,a);
}
//just the math power function
int power(int x, int y) 
{
    int z = 1;
    for (int i = y; i>0; i--)
    {
        z*=x;
    }
    return z;
}
//simulates an LED "falling" to the bottom
void fall(int x, int y, int z){
while(y!=0 && getVal(x,y-1)==0)
    {
    swap(x,y,x,y-1);
    wait_ms(z);
    matrix.writeDisplay();
    y--;
    }
}
//causes an LED to flash between on and off, ending in the same state it starts
void blink(int x,int y){
    for(int i=0;i<14;i++){
    setVal(x,y,i%2);
    matrix.writeDisplay();
    wait_ms(25);
    }
}

//when pressed, the button checks if the blocks have a place to stand (if the
//LED below is on) and then falls if it does not
void buttonPress(){
wait_ms(4); //to prevent accidental presses
if(button && height<16)
    {               
        for(int j=0;j<8;j++)
            {      
                if(height !=0 &&getVal(j,height-1)==0)
                    {
                        if(getVal(j,height)!=0)
                        {
                            length--;
                            blink(j,height);
                            fall(j,height,100);
                        }
                        setVal(j,height,0);
                     }
            }
        shift=rand(); //a psuedo random offset so the LEDs dont start in the 
                      //same position each time
        height++;     //move to the next level
        if(height>=8 && holding == 0) //upon reaching the halfway point, the speed                        
        delay=delay*4/5;           //increases every level
        for (int k = length-1; k>=0;k--){
        setVal((pos[k]+shift)%8,height,1); //sets up the next level
        }
        matrix.writeDisplay();
    }
}
//at the start of a new game, all settings are reset
void newGame(){ 
    
    matrix.clear();
    length=4;
    height=0;
    holding = hold;
    difficulty = (speed3<<2)|(speed2<1)|(speed1);
    delay = 500-difficulty*70;
    for (int k = length-1; k>=0;k--){
        //wait_ms(100);
        setVal((pos[k]+shift)%8,height,1);
        matrix.writeDisplay();
    }
    
    while(length>0 && height<16) { //keep moving the lights along the line if
                                   //the top isnt reached and there are still 
                                   //LEDs that havnt fallen
        wait_ms(delay);            //the wait to move the LEDs
        if(reset)                  //break if a new game is to be started
        break;
        if(length>0 && height<16) //checked again in case the button was pressed
        {
            swap((pos[0]+shift)%8,height,(pos[length]+shift)%8,height); //moves the line
            for(int a = 0; a<6;a++)                              
            {
                pos[a]=(pos[a]+1)%8; //updates the position of the line
            }  
         }
         else
         {
            break;   
         }
         matrix.writeDisplay(); //displays the board
    }
    if(!reset){ //if we arent trying to restart the game,
    wait_ms(1000);
    if(length>0 && height == 16){ //and the game has been won, display the win
    height =4;
    matrix.clear();
    setVal(0,11,1);
    setVal(1,11,1);
    setVal(2,11,1);
    
    setVal(5,11,1);
    setVal(6,11,1);
    setVal(7,11,1);
    for (int l = 0;l<8;l++)
    setVal(l,6,1);
    matrix.writeDisplay();
    wait_ms(100);
    swap(0,6,0,8);
    matrix.writeDisplay();
    wait_ms(100);
    swap(1,6,1,7);
    matrix.writeDisplay();
    wait_ms(100);
    
    
    swap(6,6,6,7);
    matrix.writeDisplay();
    wait_ms(100);
    swap(7,6,7,8);
    matrix.writeDisplay();
    wait_ms(100);
    
    swap(1,11,1,12);
    matrix.writeDisplay();
    wait_ms(100);
    
    swap(6,11,6,12);
    matrix.writeDisplay();
    wait_ms(1000);
    for (int x=0;x<8;x++)
        for(int y=0;y<16;y++){
            if(reset)
            newGame();
            fall(x,y,10);
            }
    }
    else                    //display the loss
    {
     matrix.clear();
    setVal(0,11,1);
    setVal(1,11,1);
    setVal(2,11,1);
    
    setVal(5,11,1);
    setVal(6,11,1);
    setVal(7,11,1);
    for (int l = 0;l<8;l++)
    setVal(l,6,1); 
    matrix.writeDisplay();
    wait_ms(100);
    swap(0,6,0,4);
    matrix.writeDisplay();
    wait_ms(100);
    swap(1,6,1,5);
    matrix.writeDisplay();
    wait_ms(100);
    
    
    swap(6,6,6,5);
    matrix.writeDisplay();
    wait_ms(100);
    swap(7,6,7,4);
    matrix.writeDisplay();
    wait_ms(100);
    
    swap(1,11,1,10);
    matrix.writeDisplay();
    wait_ms(100);
    
    swap(6,11,6,10);
    matrix.writeDisplay();
    wait_ms(1000);
    for (int x=0;x<8;x++)
        for(int y=0;y<16;y++){
            if(reset)
            newGame();
            fall(x,y,10);
            }
    }
   }
   else{
    wait_ms(10);
   newGame();       //starts a new game
   }
}

int main() {
    button.rise(&buttonPress);
   // reset.fall(&newGame);
   matrix.begin(0x70); //allows the matrix to be used
   matrix.clear();     //turns off all LEDs
   matrix.writeDisplay();
    
    t1=clock();
    srand(t1);          //psuedo random using the time as a seed
    while(1){           //continuously loops the game
    if (reset||count ==0){
    count++;
    wait_ms(4);
    shift = rand();
    
    newGame();
    }
    }
}

