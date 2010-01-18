#ifndef _CALIBRATE_H_

#define _CALIBRATE_H_

#ifdef __cplusplus
extern "C"{
#endif

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/

#include <math.h>


/****************************************************/
/*                                                  */
/* Definitions                                      */
/*                                                  */
/****************************************************/

#ifndef		_CALIBRATE_C_
	#define		EXTERN         extern
#else
	#define		EXTERN
#endif



#ifndef		OK
	#define		OK		        0
	#define		NOT_OK		   -1
#endif



#define			INT32				long




/****************************************************/
/*                                                  */
/* Structures                                       */
/*                                                  */
/****************************************************/


typedef struct Point32 {
                        INT32	x; /*!< x coordinate (32-bit, signed) */ 
						INT32	y; /*!< y coordinate (32-bit, signed) */
                     } POINT32 ;



typedef struct Matrix {
							/* This arrangement of values facilitates 
							 *  calculations within getDisplayPoint() 
							 */
                        INT32    An,     /*!< A = An/Divider */
                                 Bn,     /*!< B = Bn/Divider */
                                 Cn,     /*!< C = Cn/Divider */
                                 Dn,     /*!< D = Dn/Divider */
                                 En,     /*!< E = En/Divider */
                                 Fn,     /*!< F = Fn/Divider */
                                 Divider ; /*!< The divider */
                     } MATRIX ;




/****************************************************/
/*                                                  */
/* Function declarations                            */
/*                                                  */
/****************************************************/



EXTERN int setCalibrationMatrix( POINT32 * display,
                                 POINT32 * screen,
                                 MATRIX * matrix) ;


EXTERN unsigned char getDisplayPoint( POINT32 * display,
                            			POINT32 * screen,
                            			MATRIX * matrix ) ;

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* _CALIBRATE_H_ */


