/*!
   @file   SmudgeBrush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "QuadraticBrush.h"

//! This brush implements smudging using the mask from the superclass.
class SmudgeBrush : public QuadraticBrush {

    public:

        SmudgeBrush(BGRA color, int flow, int radius);
    virtual ~SmudgeBrush();

        // <!-- begin ta code
    virtual void resetOnMouseUp();
        // end ta code -->

    protected:
        //! Constructs the mask for this brush.
        void makeMask();

    //! Copy paint in the bounding box of the brush from canvas to m_paint
        void pickUpPaint(int x, int y, Canvas2D* canvas);

    //! Apply the brush one time to the given canvas centered at (x,y)
        virtual void paintOnce(int x, int y, Canvas2D* canvas);

        // begin ta code
        bool m_isStartOfStroke;
        BGRA *m_paint;
        // end ta code
};

#endif
