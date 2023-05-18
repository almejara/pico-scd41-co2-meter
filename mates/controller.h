/* 
 * File:   controller.h
 * Author: cruzj
 *
 * Created on June 18, 2021, 11:31 PM
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "includes/commands.h"
#include "includes/widgets.h"
#include "includes/errors.h"

#ifdef	__cplusplus
extern "C" {
#endif
    

#define __MATES_STUDIO_COMPATIBILITY_VERSION__  "0.5.0"
#define __MATES_CONTROLLER_LIBRARY_VERSION__    "0.1.0"

#define __MATES_STRING_BUFFER_SIZE__            50
#define __MATES_BOOT_TIMEOUT__                  5000
#define __MATES_CMD_TIMEOUT__                   500
#define __MATES_RSP_TIMEOUT__                   500
    
// Prerequisite Functions
extern void (* _mates_Reset)(void);
extern uint32_t (* _mates_Millis)(void);
extern uint8_t (* _mates_SerialAvailable)(void);
extern uint8_t (* _mates_SerialRead)(void);
extern void (* _mates_SerialWrite)(uint8_t);
    
// Public Functions
void mates_attachHwResetFnc(void (* resetFnc)(void));
void mates_attachMillisFnc(uint32_t (* millisFnc)(void));
void mates_attachWriteFnc(void (* writeFnc)(uint8_t));
void mates_attachReadFnc(uint8_t(* readFnc)(void));
void mates_attachRxCountFnc(uint8_t(* rxCountFnc)(void));

void mates_setBootTimeout(uint16_t timeout);
void mates_setRspTimeout(uint16_t timeout);
void mates_setCmdTimeout(uint16_t timeout);
bool mates_begin(void);
bool mates_reset(void);
bool mates_softReset(void);

// Non-widget functions
bool mates_setBacklight(uint8_t value);
bool mates_setPage(uint16_t page);
int16_t mates_getPage(void);

// Common widget functions
bool mates_setWidgetValue(MatesWidget type, uint8_t index, int16_t value);
int16_t mates_getWidgetValue(MatesWidget type, uint8_t index);

// Widget-specific functions
bool mates_setLedDigitsShortValue(uint8_t index, int16_t value);
bool mates_setLedDigitsLongValue(uint8_t index, int32_t value);
bool mates_setLedDigitsFloatValue(uint8_t index, float value);
bool mates_setSpectrumValue(MatesWidget type, uint8_t index, uint8_t gaugeIndex, uint8_t value);
bool mates_setLedSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value);
bool mates_setMediaSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value);

// Non-Image (GCI) common widget functions
bool mates_setWidgetParam(MatesWidget type, uint8_t index, int16_t param, int16_t value);
int16_t mates_getWidgetParam(MatesWidget type, uint8_t index, int16_t param);

// TextArea and PrintArea support functions
void mates_setBufferSize(uint16_t size);

// TextArea functions    
bool mates_clearTextArea(uint16_t index);
bool mates_updateTextArea(uint16_t index, const char * str);

// PrintArea functions
bool mates_clearPrintArea(uint16_t index);
bool mates_setPrintAreaColor565(uint16_t index, int16_t rgb565);
bool mates_setPrintAreaColorRGB(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
bool mates_appendArrayToPrintArea(uint16_t index, const int8_t * buf, uint16_t len);
bool mates_appendStringToPrintArea(uint16_t index, const char * str);

// Scope functions
bool mates_appendToScope(uint16_t index, const int16_t * buf, uint16_t len);

// Dot Matrix functions
bool mates_updateDotMatrix(uint16_t index, const char * str);

// Utility functions
char * mates_getVersion(void);
char * mates_getCompatibility(void);
//void mates_printVersion();
MatesError mates_getError(void);

// General Support Functions

void _mates_WriteCommand(MatesCommand value);
void _mates_WriteByte(int8_t value);
void _mates_WriteWord(int16_t value);
void _mates_WriteLong(int32_t value);
void _mates_WriteFloat(float value);
void _mates_WriteString(const char * str);
void _mates_WriteByteArray(const int8_t * buf, uint16_t len);
void _mates_WriteWordArray(const int16_t * buf, uint16_t len);

bool _mates_WaitForACK(uint16_t timeout);
int16_t _mates_ReadWord(void);
int16_t _mates_ReadResponse(uint16_t timeout);

// Widget Support Functions
bool _mates_setWidgetLongValue(int16_t widget, int32_t value);
bool _mates_setWidgetFloatValue(int16_t widget, float value);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_H */

