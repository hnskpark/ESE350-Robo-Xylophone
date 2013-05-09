#include "mbed.h"
#include "rtos.h"
#include "Loop.h"


DigitalOut keys[] = { (p5), (p6), (p7), (p8), (p9), (p10), (p11), (p12), (p13), (p14), (p15), (p18), (p17) };  // output pins, 1 for each key
int BPM;            // global BPM (speed)

Serial controller(p28, p27);
Serial pc(USBTX, USBRX);
Loop* loops[10];     // const index of loops
Thread* loopthreads[10];  // which loops are on

void play(Note* note)  {
    //printf("play\r\n");
    // turn on keys
    for (int i=0; i < note->n; ++i)
        if ((note->value[i] >= 1)&&(note->value[i] <= 13))
            keys[note->value[i] - 1] = 1;
    Thread::wait(20);
    // turn off
    for (int i=0; i < note->n; ++i)
        if ((note->value[i] >= 1)&&(note->value[i] <= 13))
            keys[note->value[i] - 1] = 0;
}


// thread to play a single loop indefinitely
void threadedloop(void const *args)  {
    Loop* myloop = (Loop*)args;
    //pc.printf("Started thread\r\n");
    while (1)  {
    //pc.printf("looping %p... %d\r\n", myloop->head, myloop->repeat);
    
    if (!myloop->current)  {  // initial/looparound case
        //pc.printf("looping around\r\n");
        myloop->current = myloop->head;
        myloop->repeat--;
        if (myloop->repeat == 0)  return;
    }
    //printf("about to play %p\r\n", myloop->current);
    play(myloop->current);
    //printf("successfully played\r\n");
    Thread::wait(myloop->current->length * 1000);
   
    myloop->current = myloop->current->next;
 
    }
}

void freenote(void const *args)  {
    while(1)  {
        Thread::signal_wait(0x1);
        printf("play free note\r\n");
        play((Note*)args);
    }
}


// create a continuous sequence of notes w/ specified duration, returning a pointer to initial note
// TODO: revise to use Loop format?
Note* scale(int start, int end, int lengthn, int lengthd)  {
    int ntuple; // number of notes to play
    float length;  // length of each individual note
    int arr[] = {start};
    
    if (start == end)  {
        return NULL;
    }
    else if (start > end)  {
        ntuple = start - end + 1;
    }
    else  {
        ntuple = end - start + 1; 
    }
    
    length = (float)lengthn/lengthd / ntuple / BPM*60;
    Note* head = setnote(1, arr, 0, 0); // length dummied out for now
    head->length = length;
    
    for (int i=1; i<ntuple; ++i)  {
        int value;
        if (start > end)  value = start - i;
        else  value = start + i;
        
        int arr2[] = {value};
        Note* temp = setnote(head, 1, arr2, 0, 0);
        temp->length = length;
    }
    
    return head;
}


int main() {
    // turn all keys off
    for (int i=0; i<13; ++i)
        keys[i] = 0;

    // reset loops[]
    for (int i=0; i<10; ++i)
        loops[i] = NULL;
   
    BPM = 500;  // default tempo
    controller.baud(9600);
     
    pc.printf("\r\ninit\r\n");
    
    // load demo tune
  
    //Demo Song Hardcode

    Loop* demo = setloop(2);
    addnote(demo, currentnote);
    
    threadedloop(demo);
    //free all the notes??
    
    Note* temp;
    while (currentnote->next)  {
        temp = currentnote;
        currentnote = currentnote->next;
        delete temp;
    }
    
    // interactive section
    BPM = 150;  // default tempo
    
    int looper[] = {A, C};
    Loop* loop1 = setloop(-1);
    loops[0] = loop1;
    addnote(loop1, hi(G), 1, 1);
    addnote(loop1, F, 1, 1);
    addnote(loop1, E, 1, 1);
    addnote(loop1, F, 1, 2);
    addnote(loop1, hi(A), 1, 1);
    addnote(loop1, REST, 4, 1);
    
    //Additional Loops
    
    
    //int highnote[] = {hi(E)};
    //int lownote[] = {G};
    
    //Note* freehi = setnote(1, highnote, 1, 1);
    //Note* freelo = setnote(1, lownote, 1, 1);
   
    
    printf("allocated all loops!\r\n");
    
    // ready to play
    // everything has to be part of a Loop in order to play
    

    //Thread t9(freenote, freehi);
    //Thread t10(freenote, freelo);
    
    
    //t1.terminate();  // test to terminate a loop from main thread
    while(1)  {
        //if (controller.readable())  {
            int loopnum;
            
            if (controller.scanf("\n%d", &loopnum))  {
                // code to turn on/off the loop OR play the free note
                pc.printf("received %d\r\n", loopnum);
                
                        loopnum--;
                        if (loopthreads[loopnum])  {
                            pc.printf("end loop %d\r\n", loopnum);
                            loopthreads[loopnum]->terminate();
                            delete loopthreads[loopnum];  // helps with memory
                            loopthreads[loopnum] = NULL;
                        }
                        else  {
                            pc.printf("start loop %d\r\n", loopnum);
                            loopthreads[loopnum] = new Thread(threadedloop, loops[loopnum]);
                        }
                    
            }
        //}
    
    }
}
