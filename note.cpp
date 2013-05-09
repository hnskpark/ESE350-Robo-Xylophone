#include "mbed.h"
#include "Note.h"

// initializer for Note struct
Note* setnote(int numnotes, int notes[], int lengthn, int lengthd)  {
    Note* temp = (Note*)malloc(sizeof(Note));
    temp->n = numnotes;
    temp->value = (int*)malloc(sizeof(int) * numnotes);
    for (int i=0; i<numnotes; ++i)
            temp->value[i] = notes[i];
    temp->length = (float)lengthn/lengthd / BPM * 60;
    temp->next = NULL;
    
    return temp;
}

// attaches a note to tail of head*
Note* setnote(Note* head, Note* newnote)  {
    while (head->next)  {
        head = head->next;
    }
    head->next = newnote;
    
    return newnote;
}

// creates a new note
Note* setnote(Note* head, int numnotes, int notes[], int lengthn, int lengthd)  {
    Note* temp = setnote(numnotes, notes, lengthn, lengthd);
    
    return setnote(head, temp);
}

Note* setnote(Note* head, int note, int lengthn, int lengthd)  {
    int noteArray[] = {note};
    
    return setnote(head, 1, noteArray, lengthn, lengthd);
}    

Note* setrest(Note* head, int lengthn, int lengthd)  {
    int noteArray[] = {0};
    return setnote(head, 1, noteArray, lengthn, lengthd);
}
