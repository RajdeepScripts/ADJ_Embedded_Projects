#include "stm32f4xx.h"
#include "gps.h"
#include "usart_debug.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>



/* Extract a specific comma-separated field from an NMEA sentence
 *
 * Parameters:
 *   line   -> Pointer to full NMEA sentence (e.g. "$GPRMC,...")
 *   field  -> Field number to extract (0-based, excluding '$')
 *   out    -> Output buffer where the extracted field is stored
 *   maxlen -> Size of the output buffer
 */
static void get_nmea_field(const char *line, int field, char *out, int maxlen)
{
    /* Tracks which field number we are currently parsing */
    int current = 0;

    /* Index into the output buffer */
    int i = 0;

    /* If the sentence starts with '$', skip it
       (NMEA sentences always begin with '$') */
    if (*line == '$')
        line++;

    /* Move through the string until we reach the desired field */
    while (*line && current < field)
    {
        /* Every comma means we move to the next field */
        if (*line == ',')
            current++;

        /* Advance to next character */
        line++;
    }

    /* Copy characters of the desired field into output buffer */
    while (*line &&            // Stop if end of string
           *line != ',' &&     // Stop at next comma (end of field)
           i < maxlen - 1)     // Leave space for null terminator
    {
        out[i++] = *line++;   // Copy character and advance pointers
    }

    /* Null-terminate the output string */
    out[i] = '\0';
}




/* Convert NMEA latitude (ddmm.mmmm) to decimal degrees
 *
 * Example:
 *   NMEA latitude = "2835.09364"
 *   dd = 28 degrees
 *   mm.mmmm = 35.09364 minutes
 *
 * Decimal degrees = degrees + (minutes / 60)
 *                 = 28 + (35.09364 / 60)
 */
static double nmea_lat_to_decimal(const char *lat)
{
    /* Extract the degree portion (first two characters)
       lat[0] and lat[1] are ASCII digits */
    int deg = (lat[0] - '0') * 10 +
              (lat[1] - '0');

    /* Convert the minutes portion starting from lat[2]
       This converts "35.09364" to a floating-point value */
    double min = atof(&lat[2]);

    /* Convert minutes to fractional degrees and add to degrees */
    return deg + (min / 60.0);
}



/* Convert NMEA longitude (dddmm.mmmm) to decimal degrees
 *
 * Example:
 *   NMEA longitude = "07718.95917"
 *   ddd = 77 degrees
 *   mm.mmmm = 18.95917 minutes
 *
 * Decimal degrees = degrees + (minutes / 60)
 *                 = 77 + (18.95917 / 60)
 */
static double nmea_lon_to_decimal(const char *lon)
{
    /* Extract the degree portion (first three characters)
       lon[0], lon[1], lon[2] are ASCII digits */
    int deg = (lon[0] - '0') * 100 +
              (lon[1] - '0') * 10 +
              (lon[2] - '0');

    /* Convert the minutes portion starting from lon[3]
       This converts "18.95917" to a floating-point value */
    double min = atof(&lon[3]);

    /* Convert minutes to fractional degrees and add to degrees */
    return deg + (min / 60.0);
}







void gps_usart1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* PA9, PA10 AF7 */
    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->MODER |=  (2U << (9 * 2)) | (2U << (10 * 2));

    GPIOA->AFR[1] &= ~((0xF << 4) | (0xF << 8));
    GPIOA->AFR[1] |=  (7U << 4) | (7U << 8);

    /* 9600 baud @ 16 MHz APB2 */
    USART1->BRR = 0x0683;

    USART1->CR1 = USART_CR1_RE | USART_CR1_UE;
}




/* =========================================================
   GPS POLLING (call continuously)
   ========================================================= */



void gps_poll(void)
{
    /* Buffer to store one complete NMEA sentence
       static -> retains value across function calls */
    static char line[128];

    /* Index to track current position in the buffer */
    static uint8_t idx = 0;

    /* Check if USART1 has received a byte
       RXNE = Receive Data Register Not Empty */
    if (USART1->SR & USART_SR_RXNE)
    {
        /* Read one character from USART1
           Reading DR also clears the RXNE flag */
        char c = USART1->DR;

        /* Check if this character marks end of NMEA line */
        if (c == '\n')
        {
            /* Null-terminate the string to make it a valid C string */
            line[idx] = '\0';

            /* Reset index for the next incoming sentence */
            idx = 0;

            /* Check if this line is an RMC sentence
               Works for GPRMC, GNRMC, GLRMC */
            if (strstr(line, "RMC,") != NULL)
            {
                /* Temporary buffers to hold parsed NMEA fields */

                /* Fix status: 'A' = valid, 'V' = invalid */
                char status[2] = {0};

                /* Latitude in ddmm.mmmm format */
                char lat[16] = {0};

                /* Longitude in dddmm.mmmm format */
                char lon[16] = {0};

                /* Latitude hemisphere: N or S */
                char ns[2]  = {0};

                /* Longitude hemisphere: E or W */
                char ew[2]  = {0};

                /* Extract fields from the RMC sentence */
                get_nmea_field(line, 2, status, sizeof(status)); // Fix status
                get_nmea_field(line, 3, lat, sizeof(lat));       // Latitude
                get_nmea_field(line, 4, ns, sizeof(ns));         // N/S
                get_nmea_field(line, 5, lon, sizeof(lon));       // Longitude
                get_nmea_field(line, 6, ew, sizeof(ew));         // E/W

                /* Proceed only if:
                   - GPS fix is valid ('A')
                   - Latitude and longitude are not empty */
                if (status[0] == 'A' && lat[0] && lon[0])
                {
                    /* Convert NMEA latitude to decimal degrees */
                    double dlat = nmea_lat_to_decimal(lat);

                    /* Convert NMEA longitude to decimal degrees */
                    double dlon = nmea_lon_to_decimal(lon);

                    /* Convert decimal degrees to fixed-point
                       (6 digits after decimal point) */
                    int lat_i = (int)(dlat * 1000000);
                    int lon_i = (int)(dlon * 1000000);

                    /* Print latitude in human-readable form */
                    usart_debug("LAT: %d.%06d %s\r\n",
                                 lat_i / 1000000,      // Integer part
                                 lat_i % 1000000,      // Fractional part
                                 ns);                  // N or S

                    /* Print longitude in human-readable form */
                    usart_debug("LON: %d.%06d %s\r\n\r\n",
                                 lon_i / 1000000,      // Integer part
                                 lon_i % 1000000,      // Fractional part
                                 ew);                  // E or W
                }
            }
        }
        else if (idx < sizeof(line) - 1)
        {
            /* If character is not newline,
               store it in the buffer and move index */
            line[idx++] = c;
        }
        /* If buffer is full, extra characters are ignored
           to prevent overflow */
    }
}


