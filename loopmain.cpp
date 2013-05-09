#include "mbed.h"

BusIn in(p5, p6, p7, p8, p11, p12, p29, p30, p16, p17);

Serial xylo(p9, p10);  // send on 9, receive on 10
Serial pc(USBTX, USBRX);

int main() {
    xylo.baud(9600); // make sure this is the same for receiver

    while(1)  {
        int temp = in;
        wait_ms(25);
        if (temp != in)  {
            // do a bitwise XOR to see what changed
            int diff = temp ^ in;
            // for those bit(s), only take ones that are now 1 (in "in")
            int pusheddown = diff & in;
            // for each button that was pressed, send a message to xylophone
            // message simply is the number of loop (1-10)
            if (pusheddown)  {
                //pc.printf("pushed down: 0x%03x\r\n", pusheddown);
                // get each bit in pusheddown
                for (int i=1; i<=10; ++i)  {
                    if (pusheddown & 0x1)  {
                        //pc.printf("%d\r\n", i);
                        xylo.printf("%d\n", i);
                    }
                    pusheddown = pusheddown >> 1;
                }
                
            }
            
            
        }
    
    }
}
