/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuFontHelv18Bold.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : GFont
**     Version     : Component 01.127, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-01-28, 11:55, # CodeGen: 321
**     Abstract    :
**
**     Settings    :
**          Component name                                 : McuFontHelv18Bold
**          SDK                                            : McuLib
**          Overwrite Bounding Box Height                  : Disabled
**          Name                                           : Helv
**          Size                                           : 18
**          Style                                          : bold
**          SourceFolders                                  : 
**            Source Folder                                : fonts
**            Config Folder                                : config/fonts
**     Contents    :
**         GetFontChar           - PGFONT_CharInfo McuFontHelv18Bold_GetFontChar(uint8_t ch);
**         GetBoxHeight          - uint8_t McuFontHelv18Bold_GetBoxHeight(void);
**         GetFont               - PGFONT_Callbacks McuFontHelv18Bold_GetFont(void);
**         GetUnderlineBoxHeight - uint8_t McuFontHelv18Bold_GetUnderlineBoxHeight(void);
**         GetLineSpaceHeight    - uint8_t McuFontHelv18Bold_GetLineSpaceHeight(void);
**
**     * Copyright (c) 2011-2018, Erich Styger
**      * Web:         https://mcuoneclipse.com
**      * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**      * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**      * All rights reserved.
**      *
**      * Redistribution and use in source and binary forms, with or without modification,
**      * are permitted provided that the following conditions are met:
**      *
**      * - Redistributions of source code must retain the above copyright notice, this list
**      *   of conditions and the following disclaimer.
**      *
**      * - Redistributions in binary form must reproduce the above copyright notice, this
**      *   list of conditions and the following disclaimer in the documentation and/or
**      *   other materials provided with the distribution.
**      *
**      * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**      * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**      * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**      * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**      * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**      * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**      * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**      * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**      * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**      * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuFontHelv18Bold.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup McuFontHelv18Bold_module McuFontHelv18Bold module documentation
**  @{
*/         


#ifndef __McuFontHelv18Bold_H
#define __McuFontHelv18Bold_H

/* MODULE McuFontHelv18Bold. */
#include "McuLib.h" /* SDK and API used */
#include "McuFontHelv18Boldconfig.h" /* configuration */

/* Include inherited components */
#include "McuLib.h"


#ifndef __BWUserType_PConstByte
#define __BWUserType_PConstByte
  typedef const uint8_t *PConstByte;   /* Pointer to a constant uint8_t. */
#endif
#ifndef __BWUserType_GFONT_CharInfo
#define __BWUserType_GFONT_CharInfo
  typedef struct {                     /* A descriptor describing the font. */
    int8_t dwidth;                     /* position of next char. relative to the current */
    uint8_t width;                     /* width of character in pixels */
    uint8_t height;                    /* height of character in pixels */
    int8_t offsetX;                    /* horizontal offset to left edge of bounding box */
    int8_t offsetY;                    /* vertical offset to bottom edge of bounding box */
    PConstByte CharBMP;                /* pointer to character bitmap defined above */
  } GFONT_CharInfo;
#endif
#ifndef __BWUserType_PGFONT_CharInfo
#define __BWUserType_PGFONT_CharInfo
  typedef GFONT_CharInfo* PGFONT_CharInfo ; /* A pointer to GFONT_CharInfo */
#endif
#ifndef __BWUserType_CallbackGetFontChar
#define __BWUserType_CallbackGetFontChar
  typedef PGFONT_CharInfo (*CallbackGetFontChar)(uint8_t ch); /* Callback used to get the font information for a single character. */
#endif
#ifndef __BWUserType_GFONT_Callbacks
#define __BWUserType_GFONT_Callbacks
  typedef struct {                     /* Structure with font relevant callbacks. */
    CallbackGetFontChar GetFontChar;   /* Callback for retrieving a font character. */
    uint8_t boundingBoxHeight;         /* Height of the bounding box. This includes the height of the underline box height. */
    uint8_t underlineBoxHeight;        /* Height of the underline box height. */
    uint8_t lineSpaceBoxHeight;        /* Height of the space between lines */
  } GFONT_Callbacks;
#endif
#ifndef __BWUserType_PGFONT_Callbacks
#define __BWUserType_PGFONT_Callbacks
  typedef GFONT_Callbacks* PGFONT_Callbacks ; /* Pointer to struct for font callbacks. */
#endif



PGFONT_CharInfo McuFontHelv18Bold_GetFontChar(uint8_t ch);
/*
** ===================================================================
**     Method      :  McuFontHelv18Bold_GetFontChar (component GFont)
**     Description :
**         Returns for a given character the corresponding font bitmap.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - The character for that a bitmap is required.
**     Returns     :
**         ---             - Character font bitmap
** ===================================================================
*/

uint8_t McuFontHelv18Bold_GetBoxHeight(void);
/*
** ===================================================================
**     Method      :  McuFontHelv18Bold_GetBoxHeight (component GFont)
**     Description :
**         Function to get the height of the bounding box.
**     Parameters  : None
**     Returns     :
**         ---             - Height of bounding box
** ===================================================================
*/

PGFONT_Callbacks McuFontHelv18Bold_GetFont(void);
/*
** ===================================================================
**     Method      :  McuFontHelv18Bold_GetFont (component GFont)
**     Description :
**         Returns callbacks for fonts to be used by the font driver.
**     Parameters  : None
**     Returns     :
**         ---             - Font callbacks.
** ===================================================================
*/

#define McuFontHelv18Bold_GetLineSpaceHeight() \
  3

/*
** ===================================================================
**     Method      :  McuFontHelv18Bold_GetLineSpaceHeight (component GFont)
**     Description :
**         Returns the size of the line space height
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

#define McuFontHelv18Bold_GetUnderlineBoxHeight() \
  5

/*
** ===================================================================
**     Method      :  McuFontHelv18Bold_GetUnderlineBoxHeight (component GFont)
**     Description :
**         Returns the size of the underline box height
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

/* END McuFontHelv18Bold. */

#endif
/* ifndef __McuFontHelv18Bold_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
