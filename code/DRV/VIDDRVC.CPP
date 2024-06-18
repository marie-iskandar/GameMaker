//----------------------------------------------------------------------
// viddrvc.cpp                    Graphics--standard C driver routines
// Copyright 1994 G. Andrew Stone
// Recreational Software Designs
// NOT PUBLIC DOMAIN!
//----------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "SDL.h"

#include "COORD2D.HPP"
#include "GEN.H"
#include "PIXEL.HPP"
#include "VIDDRIVE.HPP"

VideoMode Defaults;

char *FontPtr;

VideoDriver::VideoDriver() {
}
VideoDriver::~VideoDriver() {}

boolean VideoDriver::Validate(int MinVersion)
  {
  return True;
  }

VideoDriver* LoadVideoDriver(char *FileName)
  {
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(SDL_Quit);
  return new VideoDriver();
  }

void TextMode(void) {
  // TODO: uninit
}


/*-----------------------BASIC GRAPHICS FUNCTIONS--------------------*/

void VideoMode::BCurve(Coord2d p1,Coord2d p2,Coord2d VMid,unsigned char col,VideoMode *V)
  {
  int l;
  Coord2d VFrom,VTo,VCur,VBres,VDelta;
  Coord2d Cur,Bres,Old,Delta;
  int Steps,MaxSteps;
  int tf,VMul;

  V->Line(p1,VMid,22,V);
  V->Line(p2,VMid,22,V);

  VFrom = VMid - p1;              // Create the start direction vector.
  VTo   = p2 - VMid;              // Create the end direction vector.

  Delta = p2-p1;

  // Normalize the vectors.
  tf=((Abs(VFrom.x)+Abs(VTo.x)))/2;
  tf=Delta.x/tf;
  tf=Abs(tf);
  VMul    = tf*VTo.x;
  VTo.x   = VMul;
  VMul    = VFrom.x*tf;
  VFrom.x = VMul;

  tf=((Abs(VFrom.y)+Abs(VTo.y)))/2;
  tf=Delta.y/tf;
  tf=Abs(tf);
  VMul    = tf*VTo.y;
  VTo.y   = VMul;
  VMul    = VFrom.y*tf;
  VFrom.y = VMul;

  // Set up Bresenham's to convert 1 vector to the other.
  VDelta = VTo-VFrom;
  VCur   = VFrom;
  Cur    = p1;
  VBres.Set(0,0);
  Bres.Set(0,0);

  MaxSteps= (Abs(Delta.x)+Abs(Delta.y))/30;

  for (Steps=0;Steps<MaxSteps;Steps++)
    {
    //bioskey(0);
    VBres += AbsC(VDelta);
    while (VBres.x>=MaxSteps) { VBres.x -= MaxSteps; VCur.x+=Sign(VDelta.x); }
    while (VBres.y>=MaxSteps) { VBres.y -= MaxSteps; VCur.y+=Sign(VDelta.y); }

    Old=Cur;

    Bres += AbsC(VCur);
    while (Bres.x>=MaxSteps) { Bres.x -= MaxSteps; Cur.x+=Sign(Delta.x)*Sign(VCur.x); }
    while (Bres.y>=MaxSteps) { Bres.y -= MaxSteps; Cur.y+=Sign(Delta.y)*Sign(VCur.y); }

    V->Line(Old,Cur,col,V);
    //V->Line(Old,Old.x+VCur.x/10,Old.y+VCur.y/10,70,V);
    }
  }


void VideoMode::SetMode (VideoMode *v)
  {
  // TODO: implement
  }

void VideoMode::Clear   (Pixel Col,VideoMode *v)
  {
  // TODO: implement
  }

// ROUTINES NEED BOUNDS CHECKING!
// Interdependancy checking required!

void VideoMode::Point   (Coord2d Pos, Pixel col,VideoMode *v)
  {
  v->Rep1Col(Pos,1,col,v);
  }

Pixel VideoMode::GetCol (Coord2d Pos,VideoMode *v)
  {
  Pixel Data;
  v->GetCols (Pos,1,&Data,v);
  return(Data);
  }

void VideoMode::Rep1Col (Coord2d Pos,uint Num,Pixel col,VideoMode *v)
  {
  Num+=Pos.x;
  if (Num>v->Size.x) Num=v->Size.x;
  for (;Pos.x<Num;Pos.x++) v->Point(Pos,col,v);
  }

void VideoMode::Rep1ColRev (Coord2d Pos,uint Num,Pixel col,VideoMode *v)
  {
  Pos.x-=Num;
  v->Rep1Col(Pos,Num,col,v);
  }

void VideoMode::RepCols (Coord2d Pos,uint Num,Pixel *cols,VideoMode *v)
  {
  Num+=Pos.x;
  if (Num>v->Size.x) Num=v->Size.x;
  for (;Pos.x<Num;Pos.x++,cols++) v->Point(Pos,*cols,v);
  }

void VideoMode::RepColsRev (Coord2d Pos,uint Num,Pixel *cols,VideoMode *v)
  {
  }

int VideoMode::GetCols (Coord2d Pos,uint Num,Pixel *cols,VideoMode *v)
  {
  Num+=Pos.x;
  if (Num>v->Size.x) Num=v->Size.x;
  for (;Pos.x<Num;Pos.x++,cols++) (*cols) = v->GetCol(Pos,v);
  return 0;
  }

int VideoMode::GetColsRev (Coord2d Pos,uint Num,Pixel *cols,VideoMode *v)
  { return 0; }

void VideoMode::BoxFill (Coord2d Start,Coord2d End,Pixel col,VideoMode *v)
  {
  int Len=(End.x-Start.x)+1;
  for (;Start.y<=End.y;Start.y++) v->Rep1Col(Start,Len,col,v);
  }

void VideoMode::DrawBlock (Coord2d Pos,Block b,VideoMode *v)
  {}

void VideoMode::Box(Coord2d ur,Coord2d ll,Pixel col,VideoMode *v)
  {
  if (ur.x==ll.x) ll.x++;
  if (ur.y==ll.y) ll.y++;

  Coord2d ul = Min(ur,ll);
  Coord2d lr = Max(ur,ll);

  ur = MakeCoord2d(lr.x,ul.y);
  ll = MakeCoord2d(ul.x,lr.y);

  v->Line(ul,ur-MakeCoord2d(1,0),col,v);
  v->Line(ur,lr-MakeCoord2d(0,1),col,v);
  v->Line(lr,ll+MakeCoord2d(1,0),col,v);
  v->Line(ll,ul+MakeCoord2d(0,1),col,v);
  }

void VideoMode::Line(Coord2d Start,Coord2d End,Pixel col,VideoMode *v)
  {
  register int l;
  Coord2d Delta,Sign;
  int absdx=0, absdy=0;
  Coord2d Cnt(0,0);

  Delta.x = End.x-Start.x;
  Delta.y = End.y-Start.y;

  Sign.x=Sign(Delta.x);
  Sign.y=Sign(Delta.y);
  absdx=Sign.x*Delta.x;
  absdy=Sign.y*Delta.y;
  v->Point(Start,col,v);

  if (absdx>=absdy)
    {
    for (l=0; l<absdx; l++)
      {
      Cnt.y += absdy;
      if (Cnt.y>=absdx)
        {
        Cnt.y-=absdx;
        Start.y += Sign.y;
        }
      Start.x += Sign.x;
      v->Point(Start,col,v);
      }
    }
  else
    {
    for (l=0; l<absdy; l++)
      {
      Cnt.x += absdx;
      if (Cnt.x>=absdy)
        {
        Cnt.x -= absdy;
        Start.x += Sign.x;
        }
      Start.y += Sign.y;
      v->Point (Start,col,v);
      }
    }
  }

void VideoMode::SetViewPos(unsigned long int pos,VideoMode *v) {
  // TODO: implement
}

void VideoMode::SetWritePage(unsigned long int pos,VideoMode *v) {
  // TODO: implement
}

void VideoMode::SetAllPal(RGBdata *pal,VideoMode *v) {
  // TODO: implement
}
void VideoMode::GetAllPal(RGBdata *pal,VideoMode *v) {
  // TODO: implement
}
void VideoMode::SetAllPalTo(RGBdata *pal,VideoMode *v) {
  // TODO: implement
}
void VideoMode::SetPal(int num,RGBdata pal,VideoMode *v) {
  // TODO: implement
}


/*----------------------------------------------------------------------*/
