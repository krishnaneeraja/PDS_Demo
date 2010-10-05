/*
 *   ThinkFinger - A driver for the UPEK/SGS Thomson Microelectronics
 *   fingerprint reader.
 *
 *   Copyright (C) 2006 Pavel Machek <pavel@suse.cz>
 *                      Timo Hoenig <thoenig@suse.de>
 *
 *   Copyright (C) 2007 Timo Hoenig <thoenig@suse.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef THINKFINGER_H
#define THINKFINGER_H

//typedef unsigned char _Bool;
#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <usb.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup libthinkfinger ThinkFinger - A fingerprint scanner driver for SGS
 *  Thomson Microelectronics fingerprint reader
 * @{ */

#ifndef _STDBOOL_H
/*typedef enum
  {
    false = 0,
    true = 1
  }_Bool;
*/
#endif

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef struct libthinkfinger_s libthinkfinger;

typedef enum {
	TF_INIT_SUCCESS              = 0x00, // initialization succeeded
	TF_INIT_NO_MEMORY            = 0x01, // couldn't allocate memory
	TF_INIT_USB_INIT_SUCCESS     = 0x02, // USB initialization succeeded
	TF_INIT_USB_DEVICE_NOT_FOUND = 0x03, // USB device not found
	TF_INIT_USB_OPEN_FAILED      = 0x04, // USB device could not be opened
	TF_INIT_USB_CLAIM_FAILED     = 0x05, // USB device could not be claimed
	TF_INIT_USB_HELLO_FAILED     = 0x06, // could not send HELLO sequence to USB device
	TF_INIT_UNDEFINED            = 0xff  // undefined
} libthinkfinger_init_status;

typedef enum {
	TF_TASK_IDLE                 = 0x00, // idle
	TF_TASK_INIT                 = 0x01, // initialization
	TF_TASK_ACQUIRE              = 0x02, // acquirement
	TF_TASK_VERIFY               = 0x03, // verification
	TF_TASK_UNDEFINED            = 0xff  // undefined
} libthinkfinger_task;

typedef enum {
	TF_STATE_INITIAL             = 0x00, // initial state
	TF_STATE_UNCHANGED           = 0x01, // no change
	TF_STATE_SWIPE_0             = 0x02, // first swipe
	TF_STATE_SWIPE_1             = 0x03, // second swipe
	TF_STATE_SWIPE_2             = 0x04, // third swipe
	TF_STATE_SWIPE_SUCCESS       = 0x05, // successful swipe
	TF_STATE_SWIPE_FAILED        = 0x06, // bad swipe
	TF_STATE_ENROLL_SUCCESS      = 0x07, // enroll failed
	TF_STATE_ACQUIRE_SUCCESS     = 0x08, // acquirement successful
	TF_STATE_ACQUIRE_FAILED      = 0x09, // acquirement failed
	TF_STATE_VERIFY_SUCCESS      = 0x0a, // verification successful
	TF_STATE_VERIFY_FAILED       = 0x0b, // verification failed
	TF_STATE_OPEN_FAILED         = 0xfb, // open(2) failed
	TF_STATE_SIGINT              = 0xfc, // received sigint
	TF_STATE_USB_ERROR           = 0xfd, // USB error
	TF_STATE_COMM_FAILED         = 0xfe, // communication error
	TF_STATE_UNDEFINED           = 0xff  // undefined
} libthinkfinger_state;

typedef enum {
	TF_RESULT_ACQUIRE_SUCCESS    = TF_STATE_ACQUIRE_SUCCESS, // acquirement successful
	TF_RESULT_ACQUIRE_FAILED     = TF_STATE_ACQUIRE_FAILED,  // acquirement failed
	TF_RESULT_VERIFY_SUCCESS     = TF_STATE_VERIFY_SUCCESS,  // verification successful
	TF_RESULT_VERIFY_FAILED      = TF_STATE_VERIFY_FAILED,   // verification failed
	TF_RESULT_OPEN_FAILED        = TF_STATE_OPEN_FAILED,     // open(2) failed
	TF_RESULT_SIGINT             = TF_STATE_SIGINT,          // received sigint
	TF_RESULT_USB_ERROR          = TF_STATE_USB_ERROR,       // USB error
	TF_RESULT_COMM_FAILED        = TF_STATE_COMM_FAILED,     // communication error
	TF_RESULT_UNDEFINED          = TF_STATE_UNDEFINED        // undefined
} libthinkfinger_result;

/** @brief callback function which the driver invokes to report a new state of
 *         the scanner
 *
 * @param state status libthinkfinger_state
 * @param data void pointer to user data
 *
 * @return void
 */
typedef void (*libthinkfinger_state_cb) (libthinkfinger_state state, void *data);

/** @brief set file the fingerprint should be written to
 *
 * @param tf struct libthinkfinger
 * @param file filename
 *
 * @return 0 on success, else -1
 */
int libthinkfinger_set_file(libthinkfinger *tf, const char *file);

/** @brief set the callback function being invoked to report a new state of the
 *         scanner
 *
 * @param tf struct libthinkfinger
 * @param state status libthinkfinger_state
 * @param data void pointer to user data
 *
 * @return 0 on success, else -1
 */
int libthinkfinger_set_callback(libthinkfinger *tf, libthinkfinger_state_cb state, void *data);

/** @brief acquire fingerprint
 *
 * acquires a fingerprint and stores it to disk on success
 *
 * @param tf struct libthinkfinger
 *
 * @return libthinkfinger_result
 */
libthinkfinger_result libthinkfinger_acquire(libthinkfinger *tf);

/** @brief verify fingerprint
 *
 * verifies a fingerprint
 *
 * @param tf struct libthinkfinger
 *
 * @return libthinkfinger_result
 */
libthinkfinger_result libthinkfinger_verify(libthinkfinger *tf);

/** @brief create a struct libthinkfinger
 *
 * create a struct libthinkfinger and return a pointer to struct libthinkfinger on success.
 *
 * @param reference to libthinkfinger_init_status
 *
 * @return pointer to struct libthinkfinger on success, else NULL
 */
libthinkfinger *libthinkfinger_new(libthinkfinger_init_status* init_status);

/** @brief free an instance of libthinkfinger
 *
 * @param tf pointer to struct libthinkfinger
 *
 * @return void
 */
void libthinkfinger_free(libthinkfinger *tf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* THINKFINGER_H */

/** @} */// end of libthinkfinger group
