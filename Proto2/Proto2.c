//USART Program
//The ATMEGA32 will receive a serial transmission and send it
//Frequency - 8MHZ Operating at 9600BPS
//PB0 - VIBRATOR
//PB1 - INDICATOR LED
//PB2 - POWER LED
//PB3 - S0 - MUX (PIN 10)
//PB4 - S1 - MUX (PIN 11)
//PB5 - S2 - MUX (PIN 14)
//PB6 - S3 - MUX (PIN 13)
//FRONT SENSOR - I1 (PIN 8) of MUX
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

void USART_Init(unsigned int baud); //Init Baud
void USART_Transmit(unsigned char data); //Transmission
unsigned char USART_Receive(void); //Receive
int convert(char);

int main(){
	unsigned char data2; //Hold the data we send/receive
	unsigned char storage[3];
	int hun = 0, ten = 0, one = 0, final = 0;
	int base = 48;
	DDRB = 0b11111111;

	//PORTB = (1 << PB2);

	USART_Init(51); //Initilize to 9600bps
	for(;;){

        //I1 is 1000 (S0 S1 S2 S3) - PB3 is high
		PORTB = (1 << PB2) | (1 << PB3) | (0 << PB4) | (0 << PB5) | (0 << PB6);			

		data2 = USART_Receive();
		USART_Transmit(data2);

		
		if(data2 == 0x52){ //if data = R	
			//save the distanc
			data2 = USART_Receive();
			storage[0] = data2;
			data2 = USART_Receive();
			storage[1] = data2;
			data2 = USART_Receive();
			storage[2] = data2;

			USART_Transmit(storage[0]); //output the distance
			USART_Transmit(storage[1]);
			USART_Transmit(storage[2]);

			//convert to a number, makes it easier to use
			hun = convert(storage[0]);
  			ten = convert(storage[1]);
  			one = convert(storage[2]);
	
			//make it integer total value
			hun = (hun - base) * 100;
  			ten = (ten - base) * 10;
  			one = one - base;
  
 			final = hun + ten + one; //final value should match storage

			//test cases
			if(final >= 40 && final <= 80){ //vibrate if
				if(final >= 70 && final < 80){
					for(int i = 0; i < 1; i++){
						PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
						_delay_ms(150);
						PORTB = (0 << PB0) | (0 << PB1) | (1 << PB2);
						_delay_ms(230);
					}
				}
				else if(final >= 60 && final < 70){
					for(int i = 0; i < 1; i++){	
						PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
						_delay_ms(180);
						PORTB = (0 << PB0) | (0 << PB1) | (1 << PB2);
						_delay_ms(150);
					} 
				}
				else if(final >= 50 && final < 60){
					for(int i = 0; i < 1; i++){
						PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
						_delay_ms(180);
						PORTB = (0 << PB0) | (0 << PB1) | (1 << PB2);
						_delay_ms(125);
					}
				}
				else{
					for(int i = 0; i < 1; i++){
						PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2);
						_delay_ms(150);
						PORTB = (0 << PB0) | (0 << PB1) | (1 << PB2);
						_delay_ms(100);
					}
				}					
			}	   
		}
		
	}
	return 0;
}
void USART_Init( unsigned int baud )
{
	UBRRH = (baud >> 8); ///Set baud rate
	UBRRL = baud;
	UCSRB = (1<<RXEN)|(1<<TXEN); //Enable RX and TX
	UCSRC = (1<<URSEL)  | (3<<UCSZ0) ; //8 data bits . 1 stop bit.
}
void USART_Transmit( unsigned char data )
{
	while ( !( UCSRA & (1<<UDRE)) ); // Wait for empty transmit buffer 
	UDR = data; //Put data into buffer, sends the data 
 }
unsigned char USART_Receive( void )
{
	while ( !(UCSRA & (1<<RXC)) ); //Wait for data to be received 
	return UDR; //Get and return received data from buffer 
}

//convert the ascii chars to dec 
int convert(char achar){
  if(achar == '0')  	 
  	return 48;
  else if(achar == '1')  
  	return 49;
  else if(achar == '2')  
  	return 50;
  else if(achar == '3')  
  	return 51;
  else if(achar == '4')  
  	return 52;
  else if(achar == '5')  
  	return 53;
  else if(achar == '6')  
  	return 54;
  else if(achar == '7')  
  	return 55;
  else if(achar == '8')  
  	return 56;
  else					 
  	return 57;
}


