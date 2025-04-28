#include <main.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// Dinh nghia cac chan dieu khien
#define LCD_RS_PIN     PIN_D0
#define LCD_RW_PIN     PIN_D1
#define LCD_ENABLE_PIN PIN_D2
#define LCD_DATA4      PIN_D4
#define LCD_DATA5      PIN_D5
#define LCD_DATA6      PIN_D6
#define LCD_DATA7      PIN_D7
#define relay          PIN_B0   // �i?u khien relay
#define buzzer         PIN_B1   // Dieu khien buzzer
#define SET            input(PIN_C0)  // N�t nhan chuyen che �o
#define UP             input(PIN_C1)  // N�t nhan t�ng nguong nhiet �o
#define DOWN           input(PIN_C2) // N�t nh?n giam nguong nhiet �o

#include <lcd.c> 
#include <internal_eeprom.c> 


int mode = 0;                  
int threshold = 50;            
float BCOEFFICIENT = 3380.0;   
float THERMISTORNOMINAL = 10000; 
float TEMPERATURENOMINAL = 25;  
float SERIESRESISTOR = 10000;   
unsigned int16 sample[5];       
int fanstate = 0;              
int overheat = 0;              

// H�m luu nguong nhiet do v�o EEPROM
void save_threshold_to_eeprom(int value) {
    write_eeprom(0, value); // L�u t?i �?a ch? 0
}

// H�m doc nguong nhiet do tu EEPROM
int read_threshold_from_eeprom() {
    return read_eeprom(0); // �?c gi� tr? t? �?a ch? 0
}

void main() {
    // Cau hinh ADC v� LCD
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(A0_A3_A1_A3);
    set_adc_channel(0);  // �?c ADC t? ch�n RA0
    lcd_init();

    // �oc nguong tu EEPROM khi khoi �ong
    threshold = read_threshold_from_eeprom();
    if (threshold < 10 || threshold > 100) {
        threshold = 50; 
    }



    while (TRUE) {
        int i;
        float average;

        // Doc nhiet do cam bien
        for (i = 0; i < 5; i++) {
            sample[i] = read_adc();
            delay_ms(10);
        }

        // T�nh trung binh gia tri ADC
        average = 0;
        for (i = 0; i < 5; i++) {
            average += sample[i];
        }
        average /= 5;

        // T�nh dien tro gia tri ADC
        average = 1023 / average - 1;
        average = SERIESRESISTOR / average;

        // Chuyen doi sang nhiet do (�C) bang cong thuc Steinhart-Hart
        float steinhart;
        steinhart = average / THERMISTORNOMINAL;
        steinhart = log(steinhart);
        steinhart /= BCOEFFICIENT;
        steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);
        steinhart = 1.0 / steinhart;
        steinhart -= 273.15;
        int temp = (int)steinhart;

        // Chuyen che do hien thi (nhiet do/nguong) neu nhan n�t SET
        if (!SET) {
            delay_ms(100); // Xu ly chong doi n�t
            if (!SET)
                mode = 1 - mode; // Chuyen doi giua che do 0 v� 1
        }

        if (mode == 0) {
            // Che do hien thi nhiet do?
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "TEMP: %d C       ", temp);

            // �ieu khien quat va buzzer
            if (temp >= threshold) {
    if (fanstate == 0) {
        fanstate = 1;
        output_high(relay);
        output_high(buzzer);
        delay_ms(150);
        output_low(buzzer);
        delay_ms(100);
        output_high(buzzer);
        delay_ms(150);
        output_low(buzzer);
    }
    output_high(relay);
    lcd_gotoxy(1, 2);
    lcd_putc("FAN: ON ");
    }
    else {
    // Tat quat khi nhiet do duoi nguong
    if (fanstate == 1) {
        fanstate = 0; // 
    }
    output_low(relay); 
    output_low(buzzer); 
    lcd_gotoxy(1, 2);
    lcd_putc("FAN: OFF");
}
            
// Canh b�o qu� nhiet
if (abs(temp - threshold) >= 30) {
    if (!overheat) {
        overheat = 1;
        output_high(buzzer); // K�ch hoat buzzer li�n tuc
    }
} else {
    if (overheat) {
        overheat = 0;
        output_low(buzzer); // Tat buzzer khi nhiet �o an to�n
    }
}

        } else {
            // Che �o �ieu chinh nguong
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "THRESHOLD: %d    ", threshold);

            // Dieu chinh nguong bang n�t UP/DOWN
            if (!UP) {
                delay_ms(100);
                if (!UP) {
                    threshold++;
                    save_threshold_to_eeprom(threshold); // Luu v�o EEPROM
                }
            }
            if (!DOWN) {
                delay_ms(100);
                if (!DOWN) {
                    threshold--;
                    save_threshold_to_eeprom(threshold); // Luu v�o EEPROM
                }
            }
        }
    }
}

