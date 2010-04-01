//*****************************************************************************
//*	Calibrate touch screen
//*****************************************************************************
//&	Edit history
//*****************************************************************************
//*	Dec 17,	2009	<MLS> Working on screen rotation
//*****************************************************************************


/*    The functions provided within this file are:
 *
 *          setCalibrationMatrix() - calculates the set of factors
 *                                    in the above equation, given
 *                                    three sets of test points.
 *               getDisplayPoint() - returns the actual display
 *                                    coordinates, given a set of
 *                                    touch screen coordinates.
 * translateRawScreenCoordinates() - helper function to transform
 *                                    raw screen points into values
 *                                    scaled to the desired display
 *                                    resolution.
 *
 *
 */
 

#define _CALIBRATE_C_



/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/

#include "calibrate.h"

#include	"HardwareDef.h"


/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/



/****************************************************/
/*                                                  */
/* Global variables                                 */
/*                                                  */
/****************************************************/



/****************************************************/
/*                                                  */
/* Forward Declaration of local functions           */
/*                                                  */
/****************************************************/






/**********************************************************************
 *
 *     Function: setCalibrationMatrix()
 *
 *  Description: Calling this function with valid input data
 *                in the display and screen input arguments 
 *                causes the calibration factors between the
 *                screen and display points to be calculated,
 *                and the output argument - matrixPtr - to be 
 *                populated.
 *
 *               This function needs to be called only when new
 *                calibration factors are desired.
 *               
 *  
 *  Argument(s): displayPtr (input) - Pointer to an array of three 
 *                                     sample, reference points.
 *               screenPtr (input) - Pointer to the array of touch 
 *                                    screen points corresponding 
 *                                    to the reference display points.
 *               matrixPtr (output) - Pointer to the calibration 
 *                                     matrix computed for the set 
 *                                     of points being provided.
 *
 *
 *  From the article text, recall that the matrix coefficients are
 *   resolved to be the following:
 *
 *
 *      Divider =  (Xs0 - Xs2)*(Ys1 - Ys2) - (Xs1 - Xs2)*(Ys0 - Ys2)
 *
 *
 *
 *                 (Xd0 - Xd2)*(Ys1 - Ys2) - (Xd1 - Xd2)*(Ys0 - Ys2)
 *            A = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Xs0 - Xs2)*(Xd1 - Xd2) - (Xd0 - Xd2)*(Xs1 - Xs2)
 *            B = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 Ys0*(Xs2*Xd1 - Xs1*Xd2) + 
 *                             Ys1*(Xs0*Xd2 - Xs2*Xd0) + 
 *                                           Ys2*(Xs1*Xd0 - Xs0*Xd1)
 *            C = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Yd0 - Yd2)*(Ys1 - Ys2) - (Yd1 - Yd2)*(Ys0 - Ys2)
 *            D = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 (Xs0 - Xs2)*(Yd1 - Yd2) - (Yd0 - Yd2)*(Xs1 - Xs2)
 *            E = ---------------------------------------------------
 *                                   Divider
 *
 *
 *                 Ys0*(Xs2*Yd1 - Xs1*Yd2) + 
 *                             Ys1*(Xs0*Yd2 - Xs2*Yd0) + 
 *                                           Ys2*(Xs1*Yd0 - Xs0*Yd1)
 *            F = ---------------------------------------------------
 *                                   Divider
 *
 *
 *       Return: OK - the calibration matrix was correctly 
 *                     calculated and its value is in the 
 *                     output argument.
 *               NOT_OK - an error was detected and the 
 *                         function failed to return a valid
 *                         set of matrix values.
 *                        The only time this sample code returns
 *                        NOT_OK is when Divider == 0
 *
 *
 *
 *                 NOTE!    NOTE!    NOTE!
 *
 *  setCalibrationMatrix() and getDisplayPoint() will do fine
 *  for you as they are, provided that your digitizer         
 *  resolution does not exceed 10 bits (1024 values).  Higher
 *  resolutions may cause the integer operations to overflow
 *  and return incorrect values.  If you wish to use these   
 *  functions with digitizer resolutions of 12 bits (4096    
 *  values) you will either have to a) use 64-bit signed     
 *  integer variables and math, or b) judiciously modify the 
 *  operations to scale results by a factor of 2 or even 4.  
 *
 *
 */
int setCalibrationMatrix( POINT32 * displayPtr,
                          POINT32 * screenPtr,
                          MATRIX * matrixPtr)
{

    int  retValue = OK ;


    
    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

    if( matrixPtr->Divider == 0 )
    {
        retValue = NOT_OK ;
    }
    else
    {
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;

        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    
        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
    }
 
    return( retValue ) ;

}	/* end of setCalibrationMatrix() */



/**********************************************************************
*
*     Function: getDisplayPoint()
*
*  Description: Given a valid set of calibration factors and a point
*                value reported by the touch screen, this function
*                calculates and returns the true (or closest to true)
*                display point below the spot where the touch screen 
*                was touched.
* 
*
* 
*  Argument(s): displayPtr (output) - Pointer to the calculated
*                                      (true) display point.
*               screenPtr (input) - Pointer to the reported touch
*                                    screen point.
*               matrixPtr (input) - Pointer to calibration factors
*                                    matrix previously calculated
*                                    from a call to 
*                                    setCalibrationMatrix()
* 
*
*  The function simply solves for Xd and Yd by implementing the 
*   computations required by the translation matrix.  
* 
*                                              /-     -\
*              /-    -\     /-            -\   |       |
*              |      |     |              |   |   Xs  |
*              |  Xd  |     | A    B    C  |   |       |
*              |      |  =  |              |* |   Ys  |
*              |  Yd  |     | D    E    F  |   |       |
*              |      |     |              |   |   1   |
*              \-    -/     \-            -/   |       |
*                                              \-     -/
* 
*  It must be kept brief to avoid consuming CPU cycles.
* 
*
*       Return: OK - the display point was correctly calculated 
*                     and its value is in the output argument.
*               NOT_OK - an error was detected and the function
*                         failed to return a valid point.
*
*
*
*                 NOTE!    NOTE!    NOTE!
*
*  setCalibrationMatrix() and getDisplayPoint() will do fine
*  for you as they are, provided that your digitizer 
*  resolution does not exceed 10 bits (1024 values).  Higher
*  resolutions may cause the integer operations to overflow
*  and return incorrect values.  If you wish to use these   
*  functions with digitizer resolutions of 12 bits (4096
*  values) you will either have to a) use 64-bit signed 
*  integer variables and math, or b) judiciously modify the 
*  operations to scale results by a factor of 2 or even 4.  
*
*
*/
 
//
 #ifdef _ENABLE_SCREEN_ROTATION_
//*****************************************************************************
unsigned char	getDisplayPoint(	POINT32	*displayPtr,
									POINT32	*screenPtr,
									MATRIX	*matrixPtr )
{
unsigned char	retValue = OK ;
INT32			myXXvalue;
INT32			myYYvalue;

	if( matrixPtr->Divider != 0 )
	{

		/* Operation order is important since we are doing integer	*/
		/*	math. Make sure you add all terms together before		*/
		/*	dividing, so that the remainder is not rounded off		*/
		/*	prematurely.											*/

		myXXvalue	=	( (matrixPtr->An * screenPtr->x) + 
							(matrixPtr->Bn * screenPtr->y) +
							matrixPtr->Cn 
						) / matrixPtr->Divider ;

		myYYvalue	=	( (matrixPtr->Dn * screenPtr->x) + 
							(matrixPtr->En * screenPtr->y) + 
							matrixPtr->Fn 
						) / matrixPtr->Divider ;
	
		//*	this was added by <MLS> Dec 2009 to allow for screen orientation
		switch(gScreenOrientation)
		{
			case kScreenOrientation_Portrait:
				displayPtr->x	=	kSCREEN_Y_size - myYYvalue;;
				displayPtr->y	=	myXXvalue;
				break;
		
			case kScreenOrientation_Portrait180:
				displayPtr->x	=	myYYvalue;
				displayPtr->y	=	kSCREEN_X_size - myXXvalue;
				break;
		
			case kScreenOrientation_Landscape180:
				displayPtr->x	=	kSCREEN_X_size - myXXvalue;
				displayPtr->y	=	kSCREEN_Y_size - myYYvalue;
				break;

			default:
				displayPtr->x	=	myXXvalue;
				displayPtr->y	=	myYYvalue;
				break;
		}
	
	}
	else
	{
		retValue = NOT_OK ;
	}

	return( retValue ) ;

} /* end of getDisplayPoint() */

 #else
//*****************************************************************************
unsigned char getDisplayPoint( POINT32 * displayPtr,
					POINT32 * screenPtr,
					MATRIX * matrixPtr )
{
	unsigned char	retValue = OK ;


	if( matrixPtr->Divider != 0 )
	{

			/* Operation order is important since we are doing integer 	*/
			/*	math. Make sure you add all terms together before		*/
			/*	dividing, so that the remainder is not rounded off		*/
			/*	prematurely.											*/

		displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
							(matrixPtr->Bn * screenPtr->y) + 
							matrixPtr->Cn 
						) / matrixPtr->Divider ;

		displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
						(matrixPtr->En * screenPtr->y) + 
							matrixPtr->Fn 
						) / matrixPtr->Divider ;
	}
	else
	{
		retValue = NOT_OK ;
	}

	return( retValue ) ;

} /* end of getDisplayPoint() */

#endif

