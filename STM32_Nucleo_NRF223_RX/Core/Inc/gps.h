/*
 * gps.h
 *
 *  Created on: Sep 27, 2024
 *      Author: root
 */

#ifndef GPS_H
#define GPS_H

#include "stm32f1xx_hal.h"

// Global değişkenler
extern float decimalLong;
extern float decimalLat;

// Fonksiyon prototipleri

float nmeaToDecimal(float coordinate);

void gpsParse(char *nmeaSentence);
void gpsGetGoogleMapsLink(char *linkBuffer);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif // GPS_H
