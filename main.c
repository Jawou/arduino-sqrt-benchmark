#define F_CPU 16000000UL
#include <avr/io.h>
#define BAUD 9600
#define UBRR_VAL ((F_CPU/16/BAUD)-1)


void s_init(){UBRR0H=0;UBRR0L=103;UCSR0B=(1<<TXEN0);UCSR0C=(3<<UCSZ00);}
void s_putc(char c){while(!(UCSR0A&(1<<UDRE0)));UDR0=c;}
void s_puts(const char*s){while(*s)s_putc(*s++);}
void s_nl(){s_putc('\r');s_putc('\n');}
void s_putnum(unsigned long n){char b[12];int8_t i=0;if(n==0){s_putc('0');return;}while(n>0){b[i++]=(n%10)+'0';n/=10;}while(i>0)s_putc(b[--i]);}

#define SQRT_ERROR -1.0f
float naive(float n){
	float t = 0;
	for(float f=0; f<n; f+=0.1){
	    if(f*f>n) return t;
	}
	return SQRT_ERROR;
}
float binary_search(float number){
    float lower = 0;
    float upper = number;
    float mid = (upper + lower)/2;
    while(upper!=lower && upper!=mid && lower!=mid){
        mid = (lower+upper)/2;
        if(mid*mid < number){lower = mid;}
        else{upper = mid;}
    }
    return mid;
}

float newtons(float number){
    float x = number;
    for(short i=0; i<3; i++){
        x = 0.5f * (x+number/x);
    }
    return x;
}

float quake(float number){
    long i;
    float x2;
    float y;

    x2 = number*0.5f;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5F - (x2 * y * y));
    y = y * (1.5F - (x2 * y * y));
    return y;
}
unsigned long benchmark(float(*method)(float), uint16_t seconds){
    unsigned long counter = 2;
    unsigned long target_ms = (unsigned long)seconds * 1000;
    unsigned long start_ms = 0;
    unsigned long elapsed_ms = 0;

    TCNT0 = 0;
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS01)|(1<<CS00);
    OCR0A = 249;
    TIFR0 = (1<<OCF0A);

    while(elapsed_ms < target_ms){
        method((float)counter);
        counter++;

        if(TIFR0 & (1<<OCF0A)){
            TIFR0 = (1<<OCF0A);
            elapsed_ms++;
        }
    }

    return counter - 2;
}







int main(void){
	s_init();
    TCCR1A = 0;
    TCCR1B = (1<<CS11);


    s_puts("serial = work skull emoji");
    unsigned long r1 = benchmark(&naive, 2);
    s_putc('.');

    unsigned long r2 = benchmark(&binary_search, 2);
    s_putc('.');

    unsigned long r3 = benchmark(&newtons, 2);
    s_putc('.');

    unsigned long r4 = benchmark(&quake, 2);
    s_putc('.');

    s_nl();

    s_puts("NAIVE");s_nl();
    s_putnum(r1);s_nl();
    s_nl();
    s_puts("BINARY SEARCH");s_nl();
    s_putnum(r2);s_nl();
    s_nl();
    s_puts("NEWTONS");s_nl();
    s_putnum(r3);s_nl();
    s_nl();
    s_puts("QUAKE III");s_nl();
    s_putnum(r4);s_nl();
}
