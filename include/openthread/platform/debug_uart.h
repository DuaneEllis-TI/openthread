/*
 *  Copyright (c) 2017, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined( PLATFORM_DEBUG_UART_H )
#define PLATFORM_DEBUG_UART_H

#include <openthread/platform/logging.h>

/**
 * @file
 *
 * Note: This is NOT a public thread API, and this header file is *NOT*
 * installed as part of openthread, this is an pseudo-internal
 * debug feature usable by developers during the course of development.
 *
 * This implements a very basic debug uart useful for logging
 * messages when the primary uart is already engaged (example the NCP)
 * and encapsulating logs is not practical.
 *
 * Implimentation Notes:
 *
 * Only 3 functions are required to be implimented by the platform.
 * see/search for the word MUST in the comments below.
 *
 * The other functions (without MUST in the comment) are WEAK symbols
 * that can optionally be overridden by the platform or the application.
 *
 * many of the other functions are simply convience functions to
 * aid a developer during their normal course of work and are not
 * intended to be present, or used in production system.
 */

#if __cplusplus
extern "C" {
#endif

/**
 * Standard printf() to the debug uart with no log decoration.
 *
 * @param fmt[in] printf formatter text
 *
 * This is a debug convience function that is not intended to be
 * used in anything other then "debug scenarios" by a developer.
 *
 * lf -> cr/lf mapping is automatically handled via otPlatDebugUart_putchar()
 *
 * @sa otPlatDebugUart_vprintf() for limitations
 *
 * This is a WEAK symbol that can easily be overridden as needed.
 */
void otPlatDebugUart_printf(const char *fmt, ...);

/**
 * Stanard vprintf() to the debug uart, with no log decoration.
 *
 * @param fmt[in] printf formatter text
 * @param ap[in]  va_list value for print parameters.
 *
 * Implimentation limitation: this formats the text into
 * a purposely small text buffer on the stack, thus long
 * messages may be truncated.
 *
 * This is a WEAK symbol that can easily be overridden as needed.
 *
 * For example, some platforms might override this via a non-WEAK
 * symbol because the platform provides a UART_vprintf() like
 * function that can handle an arbitrary length output.
 */
void otPlatDebugUart_vprintf(const char *fmt, va_list ap);

/**
 * Platform specific write single byte to Debug Uart
 * This should not perform CR/LF mapping.
 *
 * This function MUST be implimented by the platform
 *
 * @param the_byte[in] what to transmit
 */
void otPlatDebugUart_putchar_raw(int the_byte);

/**
 * Poll/test debug uart if a key has been pressed.
 * It would be common to a stub function that returns 0.
 *
 * This function MUST be implimented by the platform
 *
 * @retval zero - nothing ready
 * @retval nonzero - otPlatDebugUart_getc() will succeed.
 */
int otPlatDebugUart_kbhit(void);

/**
 * Poll/Read a byte from the debug uart
 *
 * This function MUST be implimented by the platform
 *
 * @retval (negative) no data available, @sa otPlatDebugUart_kbhit()
 * @retval (0x00..0x0ff) data byte value
 *
 */
int otPlatDebugUart_getc(void);

/**
 * Write byte to the uart, expand cr/lf as need.
 *
 * A WEAK default implimentation is provided
 * that can be overridden as needed.
 *
 * @param the_byte[in] the byte to transmit
 */
void otPlatDebugUart_putchar(int the_byte);

/**
 * identical to "man 3 puts" - terminates with lf
 * Which is then mapped to cr/lf as required
 *
 * A WEAK default implimentation is provided
 * that can be overridden as needed.
 *
 * @param s[in] the string to print with a lf at the end
 */
void otPlatDebugUart_puts(const char *s);

/**
 * Write N bytes to the UART, mapping cr/lf
 *
 * @param pBytes[in] pointer to bytes to transmit.
 * @param nBytes[in] how many bytes to transmit.
 */
void otPlatDebugUart_write_bytes(const uint8_t *pBytes, int nBytes);

/**
 * puts() without a terminal newline.
 * see: "man 3 puts", without a adding a terminal lf
 *
 * @param s[in] the string to print without a lf at the end
 *
 * Note, the terminal "lf" mapped to cr/lf via
 * the function otPlatDebugUart_putchar()
 */
void otPlatDebugUart_puts_no_nl(const char *s);

/**
 * Logging function when the DEBUG_UART logs are enabled.
 *
 * This function can be used via a compiler command line define
 * in this form:
 *
 *-DOPENTHREAD_CONFIG_PLAT_LOG_FUNCTION=otPlatDebugUart_Log
 */
void otPlatDebugUart_Log(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...);

/**
 * Some platforms (posix) can log to a file.
 *
 * @returns OT_ERROR_NONE
 * @returns OT_ERROR_FAILED
 *
 * Platforms that desire this MUST provide an implimentation.
 *
 */
otError otPlatDebugUart_logfile(const char *filename);

#if __cplusplus
}
#endif

#endif

