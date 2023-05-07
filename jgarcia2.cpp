//Jesse Garcia
//walk.cpp

//my libraries
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//#include <GL/glx.h>
//#include "fonts.h"

//asteroids libraries
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


class Global {
public:
     GLuint menu; //my menus texture
     GLuint restartBox;
     GLuint menuBox;
     GLuint mysBox;
}gl;

class Image {
public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
                if (fname[0] == '\0')
                        return;
                int ppmFlag = 0;
                char name[40];
                strcpy(name, fname);
                int slen = strlen(name);
                char ppmname[80];
                if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                        ppmFlag = 1;
                if (ppmFlag) {
                        strcpy(ppmname, name);
                } else {
                        name[slen-4] = '\0';
                        sprintf(ppmname,"%s.ppm", name);
                        char ts[100];
                        sprintf(ts, "convert %s %s", fname, ppmname);
                        system(ts);
                }
                FILE *fpi = fopen(ppmname, "r");
                if (fpi) {
                        char line[200];
                        fgets(line, 200, fpi);
                        fgets(line, 200, fpi);
                        //skip comments and blank lines
                        while (line[0] == '#' || strlen(line) < 2)
                                fgets(line, 200, fpi);
                        sscanf(line, "%i %i", &width, &height);
                        fgets(line, 200, fpi);
                        //get pixel data
                        int n = width * height * 3;
                        data = new unsigned char[n];
                        for (int i=0; i<n; i++)
                                data[i] = fgetc(fpi);
                        fclose(fpi);
                } else {
                        printf("ERROR opening image: %s\n",ppmname);
                        exit(0);
                }
                if (!ppmFlag)
                        unlink(ppmname);
        }
} player1Menu("menuPics/menuP1.png"), player2Menu("menuPics/menuP2.png"),
menuSquare("menuPics/menuWhiteSquare.png"), restartSquare("menuPics/restartWhiteSquare.png"),
mysBoxPic("images/mysteryBox.jpg");


void greenBoxes(int ywin, int xwin)
{
        glPushMatrix();
        glColor3f(0.0, 5.0, 0.0);
        glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(0, ywin);
            glVertex2i(20, ywin);
            glVertex2i(20, 0);
            
            glVertex2i(xwin-20, 0);
            glVertex2i(xwin-20, ywin);
            glVertex2i(xwin, ywin);
            glVertex2i(xwin, 0);
            
            glVertex2i(0, 0);
            glVertex2i(0, 20);
            glVertex2i(xwin, 20);
            glVertex2i(xwin, 0);
            
            glVertex2i(0, ywin-20);
            glVertex2i(0, ywin);
            glVertex2i(xwin, ywin);
            glVertex2i(xwin, ywin-20);
        glEnd();
        glPopMatrix();
}

void playerBlocking(float w, float h, float x, float y, int flipped)
{
    if (flipped == 1) { //fliped = 1 so player is facing right
        //before elbow
        glPushMatrix();
        glTranslatef(x, y+50.0, 0.0);
        glBegin(GL_QUADS);
        //glColor3ub(0, 0, 0);
            glVertex2i(-w, -w);
            glVertex2i(-w, 0);
            glVertex2i(w*2+2, 0); //the w*2+2 dictates how far the 
            glVertex2i(w*2+2, -w); //elbow reach is
        glEnd();
        glPopMatrix();
        
        //after elbow
        glPushMatrix();
        glTranslatef(x+w*2+2, y+50.0, 0.0);
        glBegin(GL_QUADS);
        //glColor3ub(0, 0, 0);
            glVertex2i(-w, 0);
            glVertex2i(-w, h/2);
            glVertex2i(0, h/2);
            glVertex2i(0, 0);
        glEnd();
        glPopMatrix();
    
    } else { //player is facing left
        //before elbow
        glPushMatrix();
        glTranslatef(x, y+50.0, 0.0);
        glBegin(GL_QUADS);
        //glColor3ub(0, 0, 0);
            glVertex2i(-w*2-2, -w);
            glVertex2i(-w*2-2, 0);
            glVertex2i(w, 0);
            glVertex2i(w, -w);
        glEnd();
        glPopMatrix();

        //after elbow
        glPushMatrix();
        glTranslatef(x-w*2-2, y+50.0, 0.0);
        glBegin(GL_QUADS);
        //glColor3ub(0, 0, 0);
            glVertex2i(w, 0);
            glVertex2i(w, h/2);
            glVertex2i(0, h/2);
            glVertex2i(0, 0);
        glEnd();
        glPopMatrix();
    }
}

void holdingWeapon(float w, float h, float x, float y, int flipped, int weapon) //fix the colors when done
{
    if (weapon == 1) {
        if (flipped == 1) { //fliped = 1 so player is facing right holding weapon
            //arm
            glPushMatrix();
            glTranslatef(x, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(0, 0, 0);
                glVertex2i(-w, -w);
                glVertex2i(-w, 0);
                glVertex2i(h, 0);
                glVertex2i(h, -w);
            glEnd();
            glPopMatrix();

            //handle
            glPushMatrix();
            glTranslatef(x+h, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(-10, 0);
                glVertex2i(-10, 10);
                glVertex2i(0, 10);
                glVertex2i(0, 0);
            glEnd();
            glPopMatrix();

            //handle protector
            glPushMatrix();
            glTranslatef(x+h, y+60.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(200, 1, 1);
                glVertex2i(-20, 0);
                glVertex2i(-20, 5);
                glVertex2i(10, 5);
                glVertex2i(10, 0);
            glEnd();
            glPopMatrix();

            //blade
            glPushMatrix();
            glTranslatef(x+h, y+65.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(-10, 0);
                glVertex2i(-10, h);
                glVertex2i(0, h);
                glVertex2i(0, 0);
            glEnd();
            glPopMatrix();
        } else { //player is facing left holding weapon
            //arm           
            glPushMatrix();
            glTranslatef(x, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(0, 0, 0);
                glVertex2i(w, -w);
                glVertex2i(w, 0);
                glVertex2i(-h, 0);
                glVertex2i(-h, -w);
            glEnd();
            glPopMatrix();

            //handle
            glPushMatrix();
            glTranslatef(x-h, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(10, 0);
                glVertex2i(10, 10);
                glVertex2i(0, 10);
                glVertex2i(0, 0);
            glEnd();
            glPopMatrix();

            //handle protector
            glPushMatrix();
            glTranslatef(x-h, y+60.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(200, 1, 1);
                glVertex2i(20, 0);
                glVertex2i(20, 5);
                glVertex2i(-10, 5);
                glVertex2i(-10, 0);
            glEnd();
            glPopMatrix();

            //blade
            glPushMatrix();
            glTranslatef(x-h, y+65.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(10, 0);
                glVertex2i(10, h);
                glVertex2i(0, h);
                glVertex2i(0, 0);
            glEnd();
            glPopMatrix();
        }
    }
}

void useWeapon(float w, float h, float x, float y, int flipped, int weapon) 
{
    //code here
    if (weapon == 1) {
        if (flipped == 1) { //fliped = 1 so player is facing right using weapon
            //arm
            glPushMatrix();
            glTranslatef(x, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(0, 0, 0);
                glVertex2i(-w, -w);
                glVertex2i(-w, 0);
                glVertex2i(h, 0);
                glVertex2i(h, -w);
            glEnd();
            glPopMatrix();

            //handle
            glPushMatrix();
            glTranslatef(x+h, y+40.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(0, 0);
                glVertex2i(0, 10);
                glVertex2i(10, 10);
                glVertex2i(10, 0);
            glEnd();
            glPopMatrix();

            //handle protector
            glPushMatrix();
            glTranslatef(x+h+10, y+40.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(200, 1, 1);
                glVertex2i(0, -10);
                glVertex2i(0, 20);
                glVertex2i(5, 20);
                glVertex2i(5, -10);
            glEnd();
            glPopMatrix();

            //blade
            glPushMatrix();
            glTranslatef(x+h+15, y+40.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(0, 0);
                glVertex2i(0, 10);
                glVertex2i(h, 10);
                glVertex2i(h, 0);
            glEnd();
            glPopMatrix();
        } else { //player facing left using weapon
            //arm
            glPushMatrix();
            glTranslatef(x, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(0, 0, 0);
                glVertex2i(w, -w);
                glVertex2i(w, 0);
                glVertex2i(-h, 0);
                glVertex2i(-h, -w);
            glEnd();
            glPopMatrix();

            //handle
            glPushMatrix();
            glTranslatef(x-h, y+50.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(0, 0);
                glVertex2i(0, -10);
                glVertex2i(-10, -10);
                glVertex2i(-10, 0);
            glEnd();
            glPopMatrix();

            //handle protector
            glPushMatrix();
            glTranslatef(x-h-10, y+40.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(200, 1, 1);
                glVertex2i(0, -10);
                glVertex2i(0, 20);
                glVertex2i(-5, 20);
                glVertex2i(-5, -10);
            glEnd();
            glPopMatrix();

            //blade
            glPushMatrix();
            glTranslatef(x-h-15, y+40.0, 0.0);
            glBegin(GL_QUADS);
            glColor3ub(1, 200, 1);
                glVertex2i(0, 0);
                glVertex2i(0, 10);
                glVertex2i(-h, 10);
                glVertex2i(-h, 0);
            glEnd();
            glPopMatrix();
        }
    }
}

int randLimit = 0;
int randX = 0;
int mysteryBox(int x, int mbn)
{
    //spawn a mystery box randomly with a fixed y. 
    //x is the only number that will be random

    x -= 50;
    /* initialize random seed: */
    srand (time(NULL));
    if (randLimit == mbn) {
        randX = (rand() % x);
        randLimit += 1;
        if (randLimit >= 3) { //was 2 adding one for health at 100
            randLimit = 0;
        }
    }
    
    //for mystery box
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &gl.mysBox);

    int w = mysBoxPic.width;
    int h = mysBoxPic.height;

    glBindTexture(GL_TEXTURE_2D, gl.mysBox);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, mysBoxPic.data);
    

    glBindTexture(GL_TEXTURE_2D, gl.mysBox);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(randX, 100);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(randX, 150);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(randX+50, 150);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(randX+50, 100);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    return randX;
}

int flasher = 0; //variable to flash menu options in menu
void restartScreen(int player, int ywin, int xwin)
{
    /* //stuff for ggprint8b
    Rect r;
    unsigned int c = 0x00ffff44;
	r.bot = ywin - 20;
	r.left = 10;
	r.center = 0;
    */
    
    //for player menu///////////////////////////////////////////////////////////////
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &gl.menu);

    int w;
    int h;

    if (player != 1) {
        w = player1Menu.width;
        h = player1Menu.height;
    } else {
        w = player2Menu.width;
        h = player2Menu.height;
    }

    //Consider changing the look of the menu? add a flashing wins in the middle of the menu?
    //add a temp image of what power up the player got
    //for player menu
    glBindTexture(GL_TEXTURE_2D, gl.menu);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        if (player != 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, player1Menu.data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, player2Menu.data);
    }

    glBindTexture(GL_TEXTURE_2D, gl.menu);
    glColor3f(1.0, 1.0, 1.0);

    int width = 500;
    int height = 500;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(xwin/3, ywin/2);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(xwin/3, ywin/2+height);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xwin/3+width, ywin/2+height);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xwin/3+width, ywin/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    if (flasher < 20) {
    //for white restart box//////////////////////////////////////////////////////
        //printf("flasher: %i\n", flasher);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &gl.restartBox);
    
        int rbW = restartSquare.width;
        int rbH = restartSquare.height;

        glBindTexture(GL_TEXTURE_2D, gl.restartBox);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, rbW, rbH, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, restartSquare.data);

        glBindTexture(GL_TEXTURE_2D, gl.restartBox);
        glColor3f(1.0, 1.0, 1.0);

        int sizeRestart = 200;
        int adjustYBox = 36;
        int adjustXBox = 285; //the x seems to be a little off for player 2 it looks like it needs to be 
                              //pushed to the right a little bit.
                              
    
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(xwin/3+adjustXBox, ywin/2+adjustYBox);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(xwin/3+adjustXBox, ywin/2+sizeRestart+adjustYBox);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xwin/3+sizeRestart+adjustXBox, ywin/2+sizeRestart+adjustYBox);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xwin/3+sizeRestart+adjustXBox, ywin/2+adjustYBox);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        
        //for white menu box//////////////////////////////////////////////////////////////////
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &gl.menuBox);
    
        int mbW = menuSquare.width;
        int mbH = menuSquare.height;

        glBindTexture(GL_TEXTURE_2D, gl.menuBox);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, mbW, mbH, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, menuSquare.data);

        glBindTexture(GL_TEXTURE_2D, gl.menuBox);
        glColor3f(1.0, 1.0, 1.0);

        int sizeMenu = 120;
        int adjustYMenu = 76;
        int adjustXMenu = 40;
    
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(xwin/3+adjustXMenu, ywin/2+adjustYMenu);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(xwin/3+adjustXMenu, ywin/2+sizeMenu+adjustYMenu);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xwin/3+sizeMenu+adjustXMenu, ywin/2+sizeMenu+adjustYMenu);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xwin/3+sizeMenu+adjustXMenu, ywin/2+adjustYMenu);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
        flasher += 1;

    if (flasher > 40) {
        printf("flasher: %i\n", flasher);
        flasher = 0;
    }

    //box for menu -> I want to put a texture over this
    /*
    glPushMatrix();
    glTranslatef(xwin/2, ywin/2, 0.0);
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
        glVertex2i(-xwin/4, -ywin/5);
        glVertex2i(-xwin/4, ywin/5);
        glVertex2i(xwin/4, ywin/5);
        glVertex2i(xwin/4, -ywin/5);
    glEnd();
    glPopMatrix();
    */

    //testing small box on menu
    /*
    glPushMatrix();
    glTranslatef(xwin/2, ywin/2, 0.0);
    glBegin(GL_QUADS);
    glColor3ub(100,1,1);
        glVertex2i(-100,0);
        glVertex2i(-100,100);
        glVertex2i(100,100);
        glVertex2i(100,0);
    glEnd();
    glPopMatrix();
    */

    //words in menu
    /*
    r.left += xwin/2;
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 16, c, "");
    ggprint8b(&r, 50, c, "PLAYER %i WINS", player);
    ggprint8b(&r, 50, c, "PRESS r TO RESTART");
    r.left -= xwin/2;
    */
}

