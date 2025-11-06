//Mini Radar System Using Ardiuno UNO
//Register Based Programming



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Ultrasonic pins
#define TRIG_PIN PB2
#define ECHO_PIN PB3

// Buzzer pin
#define BUZZER_PIN PB0

// Servo pin (OC1A -> PB1)
#define SERVO_PIN PB1

void init_pins() {
    // Set TRIG and BUZZER as output, ECHO as input
    DDRB |= (1 << TRIG_PIN) | (1 << BUZZER_PIN);
    DDRB &= ~(1 << ECHO_PIN);
}

void init_servo_pwm() {
    // Fast PWM mode using Timer1 (16-bit)
    // Prescaler = 8 → 50Hz frequency (20ms period)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 39999;  // 20 ms period (16MHz/8/20000Hz)
}

void set_servo_angle(uint8_t angle) {
    // Rotate from 0° to 180°
  for (int pos = 0; pos <= 180; pos++) {
    OCR1A = pos;    // Move servo to position
    _delay_us(15);             // Small delay for smooth motion
  }

  // Rotate back from 180° to 0°
  for (int pos = 180; pos >= 0; pos--) {
    
    OCR1A = pos;
    _delay_us(15);
  }
}

uint16_t get_distance() {
    // Send 10µs trigger pulse
    PORTB &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);

    // Wait for ECHO HIGH
    while (!(PINB & (1 << ECHO_PIN)));
    uint32_t count = 0;
    while (PINB & (1 << ECHO_PIN)) {
        count++;
        _delay_us(1);
        if (count > 30000) break; // timeout
    }

    // Calculate distance (in cm)
    uint16_t distance = count / 58;
    return distance;
}

int main(void) {
    init_pins();
    init_servo_pwm();

    while (1) {
        // Sweep 0 to 180
        for (uint8_t angle = 0; angle <= 180; angle++) {
            set_servo_angle(angle);
            _delay_ms(15);
            uint16_t distance = get_distance();

            if (distance > 0 && distance <= 20) {
                PORTB |= (1 << BUZZER_PIN);
            } else {
                PORTB &= ~(1 << BUZZER_PIN);
            }
        }

        // Sweep 180 back to 0
        for (int8_t angle = 180; angle >= 0; angle--) {
            set_servo_angle(angle);
            _delay_ms(15);
            uint16_t distance = get_distance();

            if (distance > 0 && distance <= 20) {
                PORTB |= (1 << BUZZER_PIN);
            } else {
                PORTB &= ~(1 << BUZZER_PIN);
            }
        }
    }
}
