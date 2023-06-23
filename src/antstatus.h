/*
*
* (c) 2022-2023 Joscha Egloff
*
* File: antstatus.h
* Author: Joscha Egloff(Proton)
* AntOS License: GNU GPL 3.0
* Abstract: Constant Macro Definitions for ANTSTATUS!
*
*/

#ifndef _ANTSTATUS_
#define _ANTSTATUS_

typedef unsigned long ANTSTATUS;

#ifdef __cplusplus
#pragma once
#endif // __cplusplus

#ifndef ANT64_NO_STATUS

// Success values

/**
 * @brief The operation completed successfully.
 */
#define STATUS_SUCCESS ((ANTSTATUS)0x000000L)

 /**
  * @brief The operation has already been completed.
  */
#define STATUS_ALREADY_COMPLETE ((ANTSTATUS)0x000001L)

  // Warning values

  /**
   * @brief The operation timed out.
   */
#define STATUS_TIMEOUT ((ANTSTATUS)0x400000L)

   /**
    * @brief A cleanup operation is required.
    */
#define STATUS_NOTIFY_CLEANUP ((ANTSTATUS)0x400001L)

    /**
     * @brief The operation is pending completion.
     */
#define STATUS_PENDING ((ANTSTATUS)0x400002L)

     /**
      * @brief The thread was alerted.
      */
#define STATUS_ALERTED ((ANTSTATUS)0x400003L)

      // Error values

      /**
       * @brief The operation failed.
       */
#define STATUS_FAILURE ((ANTSTATUS)0x800000L)

       /**
        * @brief The operation failed due to insufficient memory.
        */
#define STATUS_OUT_OF_MEMORY ((ANTSTATUS)0x800001L)

        /**
         * @brief The operation failed because it was called by an invalid caller.
         */
#define STATUS_DRV_INVALID_CALLER ((ANTSTATUS)0x800002L)

         /**
          * @brief The operation was unsuccessful.
          */
#define STATUS_UNSUCCESSFUL ((ANTSTATUS)0x800003L)

          /**
           * @brief The operation is not implemented.
           */
#define STATUS_NOT_IMPLEMENTED ((ANTSTATUS)0x800004L)

           /**
            * @brief An access violation occurred.
            */
#define STATUS_ACCESS_VIOLATION ((ANTSTATUS)0x800005L)

            /**
             * @brief An invalid handle was specified.
             */
#define STATUS_INVALID_HANDLE ((ANTSTATUS)0x800006L)

             /**
              * @brief The end of the file was reached.
              */
#define STATUS_END_OF_FILE ((ANTSTATUS)0x800007L)

              /**
               * @brief The specified file does not exist.
               */
#define STATUS_NO_SUCH_FILE ((ANTSTATUS)0x800008L)

               /**
                * @brief There is no memory available to complete the operation.
                */
#define STATUS_NO_MEMORY ((ANTSTATUS)0x800009L)

                /**
                 * @brief Access to the resource was denied.
                 */
#define STATUS_ACCESS_DENIED ((ANTSTATUS)0x80000AL)

                 /**
                  * @brief The specified buffer is too small to hold the data.
                  */
#define STATUS_BUFFER_TOO_SMALL ((ANTSTATUS)0x80000BL)

                  /**
                   * @brief The operation is not supported.
                   */
#define STATUS_NOT_SUPPORTED ((ANTSTATUS)0x80000CL)

                   /**
                    * @brief An internal error occurred.
                    */
#define STATUS_INTERNAL_ERROR ((ANTSTATUS)0x80000DL)

                    /**
                     * @brief A hard failure occurred in the driver.
                     */
#define STATUS_DRV_HARD_FAILURE ((ANTSTATUS)0x80000EL)

                     /**
                      * @brief The Format is invalid!
                      */
#define STATUS_INVALID_FORMAT ((ANTSTATUS)0x80000FL)

                      /**
                       * @brief DOS initialization failed.
                       */
#define STATUS_DOS_INIT_FAILURE ((ANTSTATUS)0x800010L)

                       /**
                        * @brief Out of handles error occurred
                        */
#define STATUS_OUT_OF_HANDLES ((ANTSTATUS)0x800011L)

                        /**
                         * @brief Invalid parameters were passed to a function
                         */
#define STATUS_INVALID_PARAMETERS ((ANTSTATUS)0x800012L)

                         /**
                          * @brief Driver not initialized error occurred
                          */
#define STATUS_DRV_NOT_INITIALIZED ((ANTSTATUS)0x800013L)

#endif // !ANT64_NO_STATUS

#endif // _ANTSTATUS_
