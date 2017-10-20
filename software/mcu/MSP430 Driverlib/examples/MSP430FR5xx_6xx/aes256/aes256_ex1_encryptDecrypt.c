/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//!  AES256 Encryption and Decryption Example
//!
//!  Description: This demo shows how to use the AES Encryption and Decryption
//!
//!               MSP430FR5969
//!             -----------------
//!
//!	Key: 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
//!	Plaintext: 00112233445566778899aabbccddeeff
//!	Ciphertext: 8ea2b7ca516745bfeafc49904b496089
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - WDT peripheral
//! - AES256
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//******************************************************************************
#include "driverlib.h"

uint8_t Data[16] =
{0x00, 0x11, 0x22, 0x33,
 0x44, 0x55, 0x66, 0x77,
 0x88, 0x99, 0xaa, 0xbb,
 0xcc, 0xdd, 0xee, 0xff};

uint8_t CipherKey[32] =
{0x00, 0x01, 0x02, 0x03,
 0x04, 0x05, 0x06, 0x07,
 0x08, 0x09, 0x0a, 0x0b,
 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13,
 0x14, 0x15, 0x16, 0x17,
 0x18, 0x19, 0x1a, 0x1b,
 0x1c, 0x1d, 0x1e, 0x1f};

uint8_t DataAESencrypted[16];           // Encrypted data
uint8_t DataAESdecrypted[16];           // Decrypted data
int i = 0;

void main(void){
    // stop watchdog
    WDT_A_hold(WDT_A_BASE);

    // Load a cipher key to module
    AES256_setCipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);

    // Encrypt data with preloaded cipher key
    AES256_encryptData(AES256_BASE, Data, DataAESencrypted);

    // Load a cipher key to module
    AES256_setDecipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);

    // Decrypt data
    AES256_decryptData(AES256_BASE, DataAESencrypted, DataAESdecrypted);

    // Array DataunAES should now contain the same data as array Data

    while(1)
    {
    }
    ;
}
