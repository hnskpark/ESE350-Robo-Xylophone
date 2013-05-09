#include "mbed.h"
#include "Loop.h"

Loop* setloop(int repeats)  {
    Loop* temp = (Loop*)malloc(sizeof(Loop));
    temp->repeat = repeats;
    temp->head = NULL;
    temp->current = NULL;
    return temp;
}

void addnote(Loop* loop, int n, int notes[], int num, int denom)  {
    if (loop->head)
        setnote(loop->head, n, notes, num, denom);
    else
        loop->head = setnote(n, notes, num, denom);
}

void addnote(Loop* loop, int note, int num, int denom)  {
    if (loop->head)
        setnote(loop->head, note, num, denom);
    else  {
        int noteArray[] = {note};
        loop->head = setnote(1, noteArray, num, denom);
    }
}

void addnote(Loop* loop, Note* note)  {
    if (loop->head)
        setnote(loop->head, note);
    else
        loop->head = note;
}
