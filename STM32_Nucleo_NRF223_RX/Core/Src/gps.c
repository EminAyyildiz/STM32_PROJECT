 #include "gps.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

char rxBuffer[128] = {0};
uint8_t rxIndex = 0;
uint8_t rxDataGPS;
float decimalLat, decimalLong;


float nmeaToDecimal(float coordinate) {
    int degree = (int)(coordinate / 100);
    return degree + (coordinate - degree * 100) / 60;
}


void gpsParse(char *nmeaSentence) {
    if (strncmp(nmeaSentence, "$GPGGA", 6) == 0) {
        float nmeaLat, nmeaLong;
        sscanf(nmeaSentence, "$GPGGA,%*f,%f,%*c,%f", &nmeaLat, &nmeaLong);
        decimalLat = nmeaToDecimal(nmeaLat);
        decimalLong = nmeaToDecimal(nmeaLong);
    }
}


void gpsGetGoogleMapsLink(char *linkBuffer) {
    sprintf(linkBuffer, "%f,%f", decimalLat, decimalLong);

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (rxDataGPS != '\n' && rxIndex < sizeof(rxBuffer)) {
            rxBuffer[rxIndex++] = rxDataGPS;
        } else {
            gpsParse(rxBuffer);
            rxIndex = 0;
            memset(rxBuffer, 0, sizeof(rxBuffer));
        }
        HAL_UART_Receive_IT(&huart1, &rxDataGPS, 1);
    }
}
