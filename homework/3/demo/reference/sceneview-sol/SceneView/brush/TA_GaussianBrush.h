/*!
   @file   TA_GaussianBrush.h
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef GaussianBrush_Header
#define GaussianBrush_Header

#include "Brush.h"

//! This brush should have a mask with gaussian falloff
class GaussianBrush : public Brush {

    public:
        GaussianBrush(BGRA color, int flow, int radius);
    virtual ~GaussianBrush();

    protected:
        void makeMask(); /// Constructs the mask for this brush.
};

#endif
