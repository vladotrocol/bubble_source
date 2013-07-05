
//=============================================================================
// System Includes
//=============================================================================
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//#define _USE_TRIGGER_EX_MAIN//Do not forget to exclude your main program!!

#include "./ImageProceser/PointGreyImageProcesser/AsyncTriggerEx.h"

//=============================================================================
// Macro Definitions
//=============================================================================
//
// Grab this many images, then quit.
//
#define IMAGES 5000

//
// Set the grab timeout to this many milliseconds.
//
#define TIMEOUT 5000

//
// Software trigger the camera instead of using an external hardware trigger
//
//#define SOFTWARE_TRIGGER_CAMERA

//
// By default, use the PGR-specific SOFT_ASYNC_TRIGGER register 0x102C to
// generate the software trigger. Comment this out to use the DCAM 1.31 
// SOFTWARE_TRIGGER register 0x62C as the software trigger (note: this requires
// a DCAM 1.31-compliant camera that implements this functionality).
//
#define USE_SOFT_ASYNC_TRIGGER

//
// Register defines
// 
#define INITIALIZE         0x000
#define TRIGGER_INQ        0x530
#define CAMERA_POWER       0x610
#define SOFTWARE_TRIGGER   0x62C
#define SOFT_ASYNC_TRIGGER 0x102C


void _HANDLE_ERROR(FlyCaptureError error, std::string function ) {
	   if( error != FLYCAPTURE_OK ) 
	   { 
		  printf( "%s: %s\n", function.c_str(), flycaptureErrorToString( error ) ); 
		  //char c;
		  //scanf("%c",&c);
		  return; 
	   } 
	}

//=============================================================================
// Function Definitions
//=============================================================================
FlyCaptureError
checkSoftwareTriggerPresence( 
                             FlyCaptureContext  context,
                             unsigned int       uiRegister )
{
   FlyCaptureError   error;

   unsigned long     ulValue;
   
   switch( uiRegister )
   {
      case SOFT_ASYNC_TRIGGER:
         error = flycaptureGetCameraRegister( 
            context, SOFT_ASYNC_TRIGGER, &ulValue );
         _HANDLE_ERROR( error,"flycaptureGetCameraRegister()" );

         //
         // Check the Presence_Inq field of the register; bit 0 == 1 indicates
         // presence of this feature.
         //
         if( ( ulValue & 0x80000000 ) == 0x80000000 )
         {
            return FLYCAPTURE_OK;
         }
         else
         {
            return FLYCAPTURE_NOT_IMPLEMENTED;
         }
      case SOFTWARE_TRIGGER:
         error = flycaptureGetCameraRegister( 
            context, TRIGGER_INQ, &ulValue );
         _HANDLE_ERROR(error, "flycaptureGetCameraRegister()"  );

         //
         // Check the Software_Trigger_Inq field of the register; bit 15 == 1 
         // indicates presence of this feature.
         //
         if( ( ulValue & 0x10000 ) == 0x10000 )
         {
            return FLYCAPTURE_OK;
         }
         else
         {
            return FLYCAPTURE_NOT_IMPLEMENTED;
         }
      default:
         return FLYCAPTURE_INVALID_ARGUMENT;
   }
}

FlyCaptureError checkTriggerReady( FlyCaptureContext context )
{
   FlyCaptureError   error;

   unsigned long     ulValue;

   // 
   // Do our check to make sure the camera is ready to be triggered
   // by looking at bits 30-31. Any value other than 1 indicates
   // the camera is not ready to be triggered.
   //
   error = flycaptureGetCameraRegister( 
      context, SOFT_ASYNC_TRIGGER, &ulValue );
   _HANDLE_ERROR( error, "flycaptureGetCameraRegister()"  );

   while( ulValue != 0x80000001 )
   {
      error = flycaptureGetCameraRegister( 
         context, SOFT_ASYNC_TRIGGER, &ulValue );
      _HANDLE_ERROR( error,"flycaptureGetCameraRegister()" );
   }

   return FLYCAPTURE_OK;
}

void configureExternalTriggerCamera(FlyCaptureContext  context){
	FlyCaptureError    error;
   FlyCaptureImage      image;

   //printf( "Initializing camera.\n" );

      
   //
   // Initialize the first camera on the bus.
   //
   error = flycaptureInitialize( context, 0 );
   _HANDLE_ERROR(error, "flycaptureInitialize()" );

  /* //
   // Reset the camera to default factory settings by asserting bit 0
   //
   error = flycaptureSetCameraRegister( context, INITIALIZE, 0x80000000 );
   _HANDLE_ERROR(error, "flycaptureSetCameraRegister()" );*/

   //
   // Power-up the camera (for cameras that support this feature)
   //
   error = flycaptureSetCameraRegister( context, CAMERA_POWER, 0x80000000 );
   _HANDLE_ERROR(error, "flycaptureSetCameraRegister()" );

   //
   // Determine whether or not the camera supports external trigger mode.
   // If it does, put the camera into external trigger mode and otherwise 
   // exit.
   //
   bool bTriggerPresent;

   error = flycaptureQueryTrigger( 
      context, &bTriggerPresent, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
   _HANDLE_ERROR(error, "flycaptureQueryTrigger()" );

   if( !bTriggerPresent)
   {
      printf("This camera does not support external trigger... exiting\n");
   }

   int   iPolarity;
   int   iSource;
   int   iRawValue;
   int   iMode;

   error = flycaptureGetTrigger( 
      context, NULL, &iPolarity, &iSource, &iRawValue, &iMode, NULL );
   _HANDLE_ERROR(error, "flycaptureGetTrigger()" );

   //printf( "Going into asynchronous Trigger_Mode_0.\n" );
   //
   // Ensure the camera is in Trigger Mode 0 by explicitly setting it, 
   // as the camera could have a different default trigger mode
   //
#ifdef SOFTWARE_TRIGGER_CAMERA
   //
   // We are triggering the camera using the internal software trigger.
   // If we are using the DCAM SOFTWARE_TRIGGER functionality, we must
   // change the Trigger_Source to reflect the Software Trigger ID = 7.
   //
   error = flycaptureSetTrigger( 
      context, true, iPolarity, 7, 0, 0 );
   _HANDLE_ERROR( "flycaptureSetCameraTrigger()", error );
#else
   //
   // We are triggering the camera using an external hardware trigger.
   //
   error = flycaptureSetTrigger( 
      context, true, iPolarity, iSource, 0, 0 );
   _HANDLE_ERROR(error, "flycaptureSetCameraTrigger()" );

#endif

   // 
   // Poll the camera to make sure the camera is actually in trigger mode
   // before we start it (avoids timeouts due to the trigger not being armed)
   //
   checkTriggerReady( context );

   //
   // Start the camera and grab any excess images that are already in the pipe.
   // Although it is extremely rare for spurious images to occur, it is
   // possible for the grab call to return an image that is not a result of a
   // user-generated trigger. To grab excess images, set a zero-length timeout.
   // A value of zero makes the grab call non-blocking.
   //
   //printf( "Checking for any buffered images..." );
   error = flycaptureSetGrabTimeoutEx( context, 0 );
   _HANDLE_ERROR(error, "flycaptureSetGrabTimeoutEx()" );
      
   error = flycaptureStart(
      context, FLYCAPTURE_VIDEOMODE_ANY, FLYCAPTURE_FRAMERATE_ANY );
   _HANDLE_ERROR(error, "flycaptureStart()" );

   //
   // Grab the image immediately whether or not trigger present
   //
   error = flycaptureGrabImage2( context, &image );
   if( error == FLYCAPTURE_OK )
   {
      ;//printf( "buffered image found. Flush successful.\n" );
   }
   else if( error == FLYCAPTURE_TIMEOUT )
   {
      ;//printf( "no flush required! (normal behaviour)\n" );
   }
   else
   {
      _HANDLE_ERROR(error, "flycaptureGrabImage2()" );
   }

   error = flycaptureStop( context );
   _HANDLE_ERROR(error, "flycaptureStop()" );

}

#ifdef _USE_TRIGGER_EX_MAIN
//=============================================================================
// Main Program
//=============================================================================
int 
main(  )
{
   FlyCaptureContext context;
   FlyCaptureImage fwImage;
   FlyCaptureError error;
   // Create the context.
   error = flycaptureCreateContext( &context );
   _HANDLE_ERROR(error, "flycaptureCreateContext()"  );
   configureExternalTriggerCamera(context);
   //
   // Start camera.  This is done after setting the trigger so that
   // excess images isochronously streamed from the camera don't fill up 
   // the internal buffers.
   //
   //printf( "Starting camera.\n" );
   int width=400;
   int height=300;
   // error = flycaptureStartCustomImage(context,0, (1024-width)/2,(1024-height)/2, width,height,100,FlyCapturePixelFormat::FLYCAPTURE_MONO8 );
	
  error = flycaptureStart( 
      context, 
      FLYCAPTURE_VIDEOMODE_ANY,
      FLYCAPTURE_FRAMERATE_ANY );
   _HANDLE_ERROR(error, "flycaptureStart()" );

   //
   // Set the grab timeout so that calls to flycaptureGrabImage2 will return 
   // after TIMEOUT milliseconds if the trigger hasn't fired.
   //
   error = flycaptureSetGrabTimeoutEx( context, TIMEOUT );
   _HANDLE_ERROR(error, "flycaptureSetGrabTimeoutEx()" );

   printf( "This program will quit after %d images are grabbed.\n", IMAGES );

   for( int iImage = 0; iImage < IMAGES; iImage++ )
   {

#ifdef SOFTWARE_TRIGGER_CAMERA

#ifdef USE_SOFT_ASYNC_TRIGGER
      //
      // Check that the camera actually supports the PGR SOFT_ASYNC_TRIGGER
      // method of software triggering
      //
      error = checkSoftwareTriggerPresence( context, SOFT_ASYNC_TRIGGER );
      if( error == FLYCAPTURE_OK )
      {
         checkTriggerReady( context );
         
         //
         // Camera is now ready to be triggered, so generate software trigger
         // by writing a '0' to bit 31
         //
         printf( "Press the Enter key to initiate a software trigger.\n" );
         getchar();
         error = flycaptureSetCameraRegister( 
            context, SOFT_ASYNC_TRIGGER, 0x80000000 );
         _HANDLE_ERROR( "flycaptureSetCameraRegister()", error );
      }
      else
      {
         printf( "SOFT_ASYNC_TRIGGER not implemented! Grab will timeout.\n" );
      }
#else
      //
      // Check that the camera actually supports the DCAM SOFTWARE_TRIGGER
      // method of software triggering    
      //
      error = checkSoftwareTriggerPresence( context, SOFTWARE_TRIGGER );
      if( error == FLYCAPTURE_OK )
      {
         error = checkTriggerReady( context );
         
         //
         // Camera is now ready to be triggered, so generate software trigger
         // by writing a '1' to bit 0
         //
         printf( "Press the Enter key to initiate a software trigger.\n" );
         getchar();
         error = flycaptureSetCameraRegister( 
            context, SOFTWARE_TRIGGER, 0x80000000 );
         _HANDLE_ERROR( "flycaptureSetCameraRegister()", error );
      }
      else
      {
         printf( "SOFTWARE_TRIGGER not implemented! Grab will timeout.\n" );
      }
#endif

#endif
      //
      // Do an image grab.  This call will block until the camera
      // is externally triggered.
      //
      error = flycaptureGrabImage2( context, &fwImage );
      if( error == FLYCAPTURE_TIMEOUT )
      {
     printf( "Grab #%d timed out after %d milliseconds.\n", iImage, TIMEOUT );
      }
      else if( error != FLYCAPTURE_OK )
      {
     _HANDLE_ERROR(error, "flycaptureGrabImage2()" );
      }      
      else
      {     
		  if(iImage%60==0)printf( "Grab %d !\n", iImage );     
      }
   }   

   //
   // Stop the camera and destroy the context.
   //
   flycaptureStop( context );
   flycaptureDestroyContext( context );

   printf( "Done!  (hit enter)" );
   getchar();

   return 0;
}

#endif