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
#define relay          PIN_B0   // Ði?u khien relay
#define buzzer         PIN_B1   // Dieu khien buzzer
#define SET            input(PIN_C0)  // Nút nhan chuyen che ðo
#define UP             input(PIN_C1)  // Nút nhan tãng nguong nhiet ðo
#define DOWN           input(PIN_C2) // Nút nh?n giam nguong nhiet ðo

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

// Hàm luu nguong nhiet do vào EEPROM
void save_threshold_to_eeprom(int value) {
    write_eeprom(0, value); // Lýu t?i ð?a ch? 0
}

// Hàm doc nguong nhiet do tu EEPROM
int read_threshold_from_eeprom() {
    return read_eeprom(0); // Ð?c giá tr? t? ð?a ch? 0
}

void main() {
    // Cau hinh ADC và LCD
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(A0_A3_A1_A3);
    set_adc_channel(0);  // Ð?c ADC t? chân RA0
    lcd_init();

    // Ðoc nguong tu EEPROM khi khoi ðong
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

        // Tính trung binh gia tri ADC
        average = 0;
        for (i = 0; i < 5; i++) {
            average += sample[i];
        }
        average /= 5;

        // Tính dien tro gia tri ADC
        average = 1023 / average - 1;
        average = SERIESRESISTOR / average;

        // Chuyen doi sang nhiet do (°C) bang cong thuc Steinhart-Hart
        float steinhart;
        steinhart = average / THERMISTORNOMINAL;
        steinhart = log(steinhart);
        steinhart /= BCOEFFICIENT;
        steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15);
        steinhart = 1.0 / steinhart;
        steinhart -= 273.15;
        int temp = (int)steinhart;

        // Chuyen che do hien thi (nhiet do/nguong) neu nhan nút SET
        if (!SET) {
            delay_ms(100); // Xu ly chong doi nút
            if (!SET)
                mode = 1 - mode; // Chuyen doi giua che do 0 và 1
        }

        if (mode == 0) {
            // Che do hien thi nhiet do?
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "TEMP: %d C       ", temp);

            // Ðieu khien quat va buzzer
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
            
// Canh báo quá nhiet
if (abs(temp - threshold) >= 30) {
    if (!overheat) {
        overheat = 1;
        output_high(buzzer); // Kích hoat buzzer liên tuc
    }
} else {
    if (overheat) {
        overheat = 0;
        output_low(buzzer); // Tat buzzer khi nhiet ðo an toàn
    }
}

        } else {
            // Che ðo ðieu chinh nguong
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "THRESHOLD: %d    ", threshold);

            // Dieu chinh nguong bang nút UP/DOWN
            if (!UP) {
                delay_ms(100);
                if (!UP) {
                    threshold++;
                    save_threshold_to_eeprom(threshold); // Luu vào EEPROM
                }
            }
            if (!DOWN) {
                delay_ms(100);
                if (!DOWN) {
                    threshold--;
                    save_threshold_to_eeprom(threshold); // Luu vào EEPROM
                }
            }
        }
    }
}

