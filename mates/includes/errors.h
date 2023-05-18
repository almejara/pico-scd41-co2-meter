/* 
 * File:   errors.h
 * Author: cruzj
 *
 * Created on June 18, 2021, 11:23 PM
 */

#ifndef ERRORS_H
#define	ERRORS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
  MATES_ERROR_NONE,
  MATES_ERROR_COMMAND_FAILED,
  MATES_ERROR_COMMAND_TIMEOUT,
  MATES_ERROR_RESPONSE_TIMEOUT
} MatesError;

#ifdef	__cplusplus
}
#endif

#endif	/* ERRORS_H */

