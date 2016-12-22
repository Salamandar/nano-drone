#include "music.h"
#include "hardware/motors.h"
#include "hardware/hardware.h"

#define MOTOR Mot_Avant_gauche
// Temps d'une double croche (base de temps)
#define TEMPO 140

void beep(unsigned int note, unsigned int duration) {
    int i;
    long delay = (long)(10000/note);  //This is the semiperiod of each note.
    long time = (long)((duration*100)/(delay*2));  //This is how much time we need to spend on the note.
    motor_set_speed(MOTOR, note/2);
    delay_ms(duration-40);
    motor_set_speed(MOTOR, 0);
    delay_ms(40); //Add a little delay to separate the single notes
}


//Definition of the notes' frequecies in Hertz.;
enum notes {
    none  = 0,
    do1   = 241,
    do1d  = 260,
    re1b  = do1d,
    re1   = 286,
    re1d  = 315,
    mi1b  = re1d,
    mi1   = 340,
    fa1   = 368,
    fa1d  = 442,
    sol1b = fa1d,
    sol1  = 442,
    sol1d = 500,
    la1b  = sol1d,
    la1   = 550,
    la1d  = 600,
    si1b  = la1d,
    si1   = 685,
    do2   = 770,
    do2d  = 850,
    re2b  = do2d,
    re2   = 980,
    re2d  = 1023,
    mi2b  = re2d,
    mi2   = 1023,
    fa2   = 1023,
    fa2d  = 1023,
    sol2b = fa2d,
    sol2  = 1023,
};

void imperial_march() {
    // beep(re2d, 100000);

    beep(sol1,  TEMPO*4);
    beep(sol1,  TEMPO*4);
    beep(sol1,  TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1,  TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1,  TEMPO*8);

    beep(re2,   TEMPO*4);
    beep(re2,   TEMPO*4);
    beep(re2,   TEMPO*4);
    beep(mi2b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1b, TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1,  TEMPO*8);

    beep(sol2,  TEMPO*4);
    beep(sol1,  TEMPO*3);
    beep(sol1,  TEMPO*1);
    beep(sol2,  TEMPO*4);
    beep(sol2b, TEMPO*3);
    beep(fa2,   TEMPO*1);
    beep(mi2,   TEMPO*1);
    beep(re2d,  TEMPO*1);
    beep(mi2,   TEMPO*2);

    beep(none,  TEMPO*2);
    beep(sol1d, TEMPO*2);
    beep(do2d,  TEMPO*4);
    beep(si1b,  TEMPO*3);
    beep(si1,   TEMPO*1);
    beep(si1b,  TEMPO*1);
    beep(la1,   TEMPO*1);
    beep(si1b,  TEMPO*2);

    beep(none,  TEMPO*2);
    beep(mi1b,  TEMPO*2);
    beep(sol1b, TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(sol1b, TEMPO*1);
    beep(si1b,  TEMPO*4);
    beep(sol1,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(re2,   TEMPO*8);

    beep(sol2,  TEMPO*4);
    beep(sol1,  TEMPO*3);
    beep(sol1,  TEMPO*1);
    beep(sol2,  TEMPO*4);
    beep(sol2b, TEMPO*3);
    beep(fa2,   TEMPO*1);
    beep(mi2,   TEMPO*1);
    beep(re2d,  TEMPO*1);
    beep(mi2,   TEMPO*2);

    beep(none,  TEMPO*2);
    beep(sol1d, TEMPO*2);
    beep(do2d,  TEMPO*4);
    beep(do2,   TEMPO*3);
    beep(si1,   TEMPO*1);
    beep(si1b,  TEMPO*1);
    beep(la1,   TEMPO*1);
    beep(si1b,  TEMPO*2);

    beep(none,  TEMPO*2);
    beep(mi1b,  TEMPO*2);
    beep(sol1b, TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1,  TEMPO*4);
    beep(mi1b,  TEMPO*3);
    beep(si1b,  TEMPO*1);
    beep(sol1,  TEMPO*8);

    // //end of third bit... (Though it doesn't play well)
    // //let's repeat it

    // beep(aH, TEMPO*4);
    // beep(a, 300);
    // beep(a, TEMPO*1);
    // beep(aH, 400);
    // beep(gSH, 200);
    // beep(gH, 200);
    // beep(fSH, 125);
    // beep(fH, 125);
    // beep(fSH, 250);

    // delay_ms(250);

    // beep(aS, 250);
    // beep(dSH, 400);
    // beep(dH, 200);
    // beep(cSH, 200);
    // beep(cH, 125);
    // beep(b, 125);
    // beep(cH, 250);

    // delay_ms(250);

    // beep(f, 250);
    // beep(gS, TEMPO*4);
    // beep(f, 375);
    // beep(cH, 125);
    // beep(a, TEMPO*4);
    // beep(f, 375);
    // beep(cH, 125);
    // beep(a, TEMPO*8);
    //end of the song}
}
