#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"
#include "lcd/display.h"

#include "usetable.h"

/**************************************************************************/

// Improved version by Ikarus:
// Added graphical battery.

// hLine and vLine code from Juna, nougad and fu86
// (camp 2011, CTHN Village)

#define BOX_FILLED 1
#define BOX_OUTLINE 2
#define LINE_SOLID 1
#define LINE_DOTTED 2
#define LINE_DASHED 3
#define FRAME_2D 1
#define FRAME_3D 2

#define max(a,b) ({ __typeof__ (a) _a = (a);    \
      __typeof__ (b) _b = (b);                  \
      _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a);    \
      __typeof__ (b) _b = (b);                  \
      _a < _b ? _a : _b; })
#define sign(x) ((x) == 0 ? 0 : ((x) > 0 ? 1 : -1 ))

void drawBox(int x, int y, int width, int height, int style);
void drawLineX(int x, int y, int length, int style);
void drawLineY(int x, int y, int length, int style);
void drawMeterX(int x, int y, int length, int value);
void drawLine(int x0, int y0, int x1, int y1, int style);
void projectIso(int x, int y, int z, int *isox, int *isoy);
void drawIsoGrid(int posx, int posy, int size);
void drawIsoLine(int posx, int posy, int x0, int y0, int z0, int x1, int y1, int z1, int style);
void drawIsoFrame(int posx, int posy, int x, int y, int z, int type);

void ram(void) {
  // int v,mv,c;
  // char old_state=-1;
  // int i=0;
  int exit = 0;
  
  int button;
  int length = 7;
  int step = 1;
  lcdClear();
  //lcdFill(0);
  //for (int x=3; x<RESX; x+=5) {
  //  for (int y=3; y<RESY; y+=5) {
  //    lcdSetPixel(x, y, 1);
  //  }
  //}
    
  lcdSetPixel(0,0,1);
  lcdSetPixel(0,RESY-1,1);
  lcdSetPixel(RESX-1,0,1);
  lcdSetPixel(RESX-1,RESY-1,1);


  //drawBox(10, 10, 10, 10, BOX_OUTLINE);
  //drawBox(22, 22, 1, 10, BOX_OUTLINE);
  //drawBox(40, 40, 3, 10, BOX_OUTLINE);
  //drawBox(100, 100, 3, 10, BOX_OUTLINE);
  //drawBox(4, 4, RESX, RESY, BOX_OUTLINE);
   

  // lcdSetCrsr( 10, 10);
  // 
  // lcdPrintln('painted');
  do {
    ////c = gpioGetValue(RB_PWR_CHRG);
    //// mv = GetVoltage();
    //  
    //  i=++i%(RESX*RESY); 
    //  //lcdSetPixel(i%RESX, i/RESX, 1);
    //  lcdSetPixel(i/RESY, i%RESY, 1);
    //
    //  lcdRefresh();
    //  //delayms(10);
    lcdFill(0);

    for (int len = 1; len <= step; len++) {
      drawLineX(30, 10+len, len, LINE_SOLID);
    }
    
    for (int len = 1; len <= step; len++) {
      drawLineY(45+len, 10, len, LINE_SOLID);
    }
    
    drawBox(70, 10, step, step, BOX_FILLED);
    drawBox(90, 10, step, step, BOX_OUTLINE);
    
    drawMeterX(1, 30, length+2, step);

    drawLineX(1, 35, step, LINE_SOLID);
    drawLineX(1, 37, step, LINE_DOTTED);
    drawLineX(1, 39, step, LINE_DASHED);
    
    drawLine (20, 35, 20+step, 35+length, LINE_SOLID);
    drawLine (24, 35, 24+step, 35+length, LINE_DOTTED);
    drawLine (28, 35, 28+step, 35+length, LINE_DASHED);

    drawLine (1,  41, 1,   40,   LINE_SOLID);
    drawLine (4,  41, 4,   40+1, LINE_SOLID);
    drawLine (7,  41, 7+1, 40,   LINE_SOLID);
    drawLine (10, 41, 10+1,40+1, LINE_SOLID);

    drawLine (1,  45, 1,   45,   LINE_DASHED);
    drawLine (4,  45, 4,   45+1, LINE_DASHED);
    drawLine (7,  45, 7+1, 45,   LINE_DASHED);
    drawLine (10, 45, 10+1,45+1, LINE_DASHED);

    drawIsoGrid (60, 40, 20);
    drawIsoFrame(60, 40, step, length, 5, FRAME_2D);
    drawIsoLine (60, 40, 0,    0,      0, step, length, 5, LINE_SOLID); 
    
    lcdSetCrsr(1,1);
    lcdPrintln(IntToStr(length, 3, 0));
    lcdPrintln(IntToStr(step, 3, 0));
    lcdRefresh();


#if 0    
    int x0 = 30;
    int y0 = 30;
    int x1 = 31;
    int y1 = 31;
    lcdSetPixel(x0, y0, 1);
    switch (step) {
    case 1: 
      x1 += 5;
      y1 += 5;
      break;
    case 2: 
      x1 += 0;
      y1 += 5;
      break;
    case 3: 
      x1 +=-5;
      y1 += 5;
      break;
    case 4: 
      x1 +=-5;
      y1 += 0;
      break;
    case 5: 
      x1 +=-5;
      y1 +=-5;
      break;
    case 6: 
      x1 += 0;
      y1 +=-5;
      break;
    case 7: 
      x1 += 5;
      y1 +=-5;
      break;
    case 8:
    default:
      x1 += 5;
      y1 += 0;
      break;
    }
  

    lcdSetCrsr(1,1);
    lcdPrintln(IntToStr(step, 3, 0));
    lcdPrintln(IntToStr(x1, 3, 0));
    lcdPrintln(IntToStr(y1, 3, 0));
    lcdRefresh();
    drawLine (x0, y0, x1, y1);
    lcdRefresh();
#endif
    //lcdPrint("test");
    button = getInputWaitTimeout(100);
    //lcdPrintInt(button);
    delayms(100);
    switch (button) {
    case BTN_UP:
      lcdPrintln("+");
      step = (step+1)%(length+1);
      break;
    case BTN_DOWN:
      lcdPrintln("-");
      step = (step-1+(length+1))%(length+1);
      break;
    case BTN_RIGHT:
      lcdPrintln(">");
      length += 1;
      break;
    case BTN_LEFT:
      lcdPrintln("<");
      length -= length < 2 ? 0: 1;
      step = min(step, length);
      break;
    case BTN_ENTER:
      lcdPrintln("enter");
      exit = 0;
      return;
      break;
    }
    
    
  } while  (!exit);


}

void drawBox(int x, int y, int width, int height, int style) {
  // checks
  if (width < 0) {
    width *= -1;
    x -= width;
  }  
  if (height < 0) {
    height *= -1;
    y -= height;
  }  
  x = max(0, min(x, RESX-1));
  y = max(0, min(y, RESY-1));
  width  = x + width  > RESX ? RESX - x : width;
  height = y + height > RESY ? RESY - y : height;
  width  = x + width  < 0 ? -x : width;
  height = y + height < 0 ? -y : height;

  // draw
  switch (style) {
  case BOX_OUTLINE: 
    drawLineX(x, y, width, LINE_SOLID);
    drawLineX(x, y+height-1, width, LINE_SOLID);
    drawLineY(x, y, height, LINE_SOLID);
    drawLineY(x+width-1, y, height, LINE_SOLID);
    break;
  case BOX_FILLED:
  default:  
    for (int posx = x; posx < x+width; posx++) {
      for (int posy = y; posy < y+height; posy++) {
        lcdSetPixel(posx, posy, 1);
      }
    }  
    break;
  }
}

void drawLineX(int x, int y, int length, int style) {
  int inc=sign(length);
  for (int i=0; i<length; i+=inc) {
    if (((i+1)%style) || (style==LINE_SOLID) || (i+1==length)) {
      lcdSetPixel(x+i, y, 1);
    }
  }
}

void drawLineY(int x, int y, int length, int style) {
  int inc=sign(length);
  for (int i=0; i<length; i+=inc) {
    if (((i+1)%style) || (style==LINE_SOLID) || (i+1==length)) {
      lcdSetPixel(x, y+i, 1);
    }
  }
}

/**
 * length is overall length, maximal value thus length-2
 * for painting, at pos 0 and length a tick is made
 * if possible, a midpoint is displayed.
 **/
void drawMeterX(int x, int y, int length, int value) {
  int midPoint = 0;
  value = min (value, length-2);
  drawLineX(x, y, length, LINE_SOLID);
  lcdSetPixel(x, y+1, 1);
  lcdSetPixel(x+length-1, y+1, 1);
  if (length-2 <= 0) {
    return;
  } else if (length-2 > 2) {
    midPoint = (length-2+1)/2; // +1 to round up
  }
  for (int i = 1; i <= value; i++) {
    lcdSetPixel(x+i, y+1, 1);
  }
  if (midPoint) {
    /*int pix = (midPoint-1==value) || (midPoint<value) ?
      0 : 1;
      lcdSetPixel(x+midPoint, y+1, pix);*/
    lcdSetPixel(x+midPoint, y, 0);
  }
}

#define abs(x) (x)>0?(x):((x)*(-1));
void drawLine(int x0, int y0, int x1, int y1, int style) {
  // Bresenham-Algorithm from http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int dx = x1-x0;
  int dy = y1-y0;
  int step = 0;
  /* if (dx == 0) { */
  /*   drawLineY(x0, y0, dy+1, style); */
  /*   return; */
  /* } */
  /* if (dy == 0) { */
  /*   drawLineX(x0, y0, dx+1, style); */
  /*   return; */
  /* } */
  dx = abs(dx);
  dy = abs(dy);
  int stepx = x0 < x1 ? 1 : -1;
  int stepy = y0 < y1 ? 1 : -1;
  int err = dx-dy;
  
  int e2;

  while (1) {
    if (((step+1)%style) || (style==LINE_SOLID)) {
      lcdSetPixel(x0, y0, 1);
    }
    if ((x0 == x1) && (y0 == y1)) {
      lcdSetPixel(x0, y0, 1);
      break;
    }
    e2 = 2*err;
    if (e2 > -dy) { 
      err -= dy;
      x0  += stepx;
    }
    if (e2 <  dx) { 
      err += dx;
      y0  += stepy;
    }
    step++;
  }
}
#undef abs

void projectIso(int x, int y, int z, int *isox, int *isoy) {
  *isox  =  y - x/2;
  *isoy  = -z + x/2;
} 

void drawIsoGrid(int posx, int posy, int size) {
  int x, y;
  projectIso(size, 0, 0, &x, &y);
  drawLine(posx-x, posy-y, posx+x, posy+y, LINE_DASHED);
  projectIso(0, size, 0, &x, &y);
  drawLine(posx-x, posy-y, posx+x, posy+y, LINE_DASHED);
  projectIso(0, 0, size, &x, &y);
  drawLine(posx-x, posy-y, posx+x, posy+y, LINE_DASHED);
}

void drawIsoLine(int posx, int posy, int x0, int y0, int z0, int x1, int y1, int z1, int style) {
  int isox0, isoy0, isox1, isoy1;
  projectIso(x0, y0, z0, &isox0, &isoy0);
  projectIso(x1, y1, z1, &isox1, &isoy1);
  drawLine(posx+isox0, posy+isoy0, posx+isox1, posy+isoy1, style);
}

void drawIsoFrame(int posx, int posy, int x, int y, int z, int type) {
    drawIsoLine(posx, posy, x, 0, 0, x, y, 0, LINE_DOTTED); 
    drawIsoLine(posx, posy, 0, y, 0, x, y, 0, LINE_DOTTED); 
                                      
    drawIsoLine(posx, posy, 0, y, 0, 0, y, z, LINE_DOTTED); 
    drawIsoLine(posx, posy, 0, 0, z, 0, y, z, LINE_DOTTED); 

    drawIsoLine(posx, posy, 0, 0, z, x, 0, z, LINE_DOTTED); 
    drawIsoLine(posx, posy, x, 0, 0, x, 0, z, LINE_DOTTED); 

    if (type == FRAME_3D) {
      drawIsoLine(posx, posy, x, y, 0, x, y, z, LINE_DOTTED);
      drawIsoLine(posx, posy, 0, y, z, x, y, z, LINE_DOTTED);
      drawIsoLine(posx, posy, x, 0, z, x, y, z, LINE_DOTTED);
    }
} 
