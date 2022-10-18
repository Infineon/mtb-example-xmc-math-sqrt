/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: MATH SQRT Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "xmc_common.h"
#include "xmc_math.h"
#include "math.h"
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

#include <stdio.h>
#include <string.h>

/*******************************************************************************
* Macros
*******************************************************************************/

/* Input number for which sqrt is calculated */
#define INPUT_NUM           (0.25F)

/* Size of the string buffer which stores float values */
#define STRING_BUF_SIZE     (32)

/* Convert Q15 number to float format */
__STATIC_INLINE float q15_to_float(int16_t a)
{ 
    return ( (float) a / 0x8000 );
}

/* Convert Q31 number to float format */
__STATIC_INLINE float q31_to_float(int32_t a)
{ 
    return ( (float) a / 0x80000000 );
}

/* Convert float to Q15 number */
__STATIC_INLINE int32_t float_to_q15(float a)
{ 
    return (int16_t)( (a * 0x8000) + 0.5F );
}

/* Convert float to Q31 number */
__STATIC_INLINE int32_t float_to_q31(float a)
{ 
    return (int32_t)( (a * 0x80000000) + 0.5F );
}

/*******************************************************************************
* Function Name: float_to_string
********************************************************************************
* Summary:
*  Function to convert a float number to a string
*
* Parameters:
*  char* string_buffer - Pointer to the string
*  uint8_t buf_size    - Size of the string buffer
*  float number        - Float number that needs to be converted to string
*
* Return:
*  void
*
*******************************************************************************/
void float_to_string(char* string_buffer, uint8_t buf_size, float number)
{
    /* Check if number is negative */
    uint8_t is_negative;
    if(number < 0)
    {
        is_negative = 1;
        number = -number;
    }
    else
    {
        is_negative = 0;
    }

    int integral_part = (int)number;
    int decimal_part = (number - integral_part) * 1000;
    if(is_negative)
    {
        snprintf(string_buffer, buf_size, "-%d.%03d", integral_part, decimal_part);
    }
    else
    {
        snprintf(string_buffer, buf_size, "%d.%03d", integral_part, decimal_part);
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It calculates the square root of the given input.
* The following operations are executed in this function:
*  1. Initialize the device and peripherals.
*  2. Convert the input number to Q31 format.
*  3. Calculate the square root using MATH-CORDIC block using the number
*     obtained in step 2 and print using UART.
*  4. Obtain the square root using software implemented function sqrtf() and
*     print using UART. 
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Variables Initialization */ 
    int32_t sqr_root_cordic_q31; 
    float sqr_root_cordic_float, sqr_root_software_float;
    char string_buff[STRING_BUF_SIZE];

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    printf("\x1b[2J\x1b[;H");
    printf("===============================================================\r\n");
    printf("XMC MCU: MATH SQRT example\r\n");
    printf("===============================================================\r\n\n");
    
    /* Calculating Square root of the input using CORDIC block */
    sqr_root_cordic_q31 = XMC_MATH_CORDIC_Q31_Sqrt(float_to_q31( INPUT_NUM ));
   
    /* Print the result on the UART */
    printf("Sqr_root_CORDIC_Q31 = %ld \r\n", sqr_root_cordic_q31);
   
    /* Convert obtained Q31 square root value to float number */
    sqr_root_cordic_float = q31_to_float( sqr_root_cordic_q31 );
    float_to_string(string_buff, STRING_BUF_SIZE, sqr_root_cordic_float);

    /* Print the obtained square root on UART */
    printf("Sqr_root_CORDIC_float = %s\r\n", string_buff);

    /* Calculating Square root of the input using MATH.h function */
    sqr_root_software_float = sqrtf( INPUT_NUM );
    float_to_string(string_buff, STRING_BUF_SIZE, sqr_root_software_float);
   
    /* Print the obtained square root on UART */
    printf("Sqr_root_Software_float = %s\r\n", string_buff);

    while(1);
}

/* [] END OF FILE */
