#pragma once
#ifndef __OGL_UTILS_H__
#define __OGL_UTILS_H__

//const int lineHeight = 16;				// The default height of a line of text (do not modify!)
#define charWidth (8u)		// TODO: Get rid of this #define
#define lineHeight (16u)		// TODO: Get rid of this #define

enum PrintAlignment { LEFT, CENTER, RIGHT };

bool OglUtilsInitFont();

void OglUtilsKillFont();

void OglUtilsPrint(double X, double Y, int iFont, PrintAlignment nAlignment, const char *chText);
//void OglUtilsPrint(int iX, int iY, int iFont, bool bCentered, const char *chFmt, ...);

#endif // __OGL_UTILS_H__
