//USART Program
//The ATMEGA32 will receive a serial transmission and send it
//Frequency - 8MHZ Operating at 9600BPS
#include <avr/io.h>
#include <stdlib.h>

void USART_Init(unsigned int baud); //Initlizae Baud
void USART_Transmit(unsigned char data); //Transmission
unsigned char USART_Receive(void); //Receive
int convert(char);
void delay_ms(uint16_t x);

int main(){
	int count = 0;
	unsigned char data2; //Hold the data we send/receive
	unsigned char storage[3];
	int hun = 0, ten = 0, one = 0, final = 0;
	int base = 48;
	int sensor1 = 0;
	int sensor2 = 0;
	DDRB = 0b11111111;
	DDRD = 0b01011000;
		
	USART_Init(51); //Initilize to 9600bps
	for(;;){

		count++; // for switching between the sensors
		PORTD = (0 << PD3); //TRI COLOR LED SENSOR I3
		PORTD = (0 << PD7); //TRI COLOR LED SENSOR I3
		
		if(count < 10){ // first sensor I1
			PORTB = (1 << PB2) | (1 << PB3) | 
			(0 << PB4) | (0 << PB5) | (0 << PB6) | (0 << PB7);
			sensor1 = 1;
			sensor2 = 0;
		}

		if(count > 10 && count <= 20){ // second sensor I3
			PORTB = (0 << PB2) | (1 << PB3) | 
			(1 << PB4) | (0 << PB5) | (0 << PB6) | (1 << PB7);
			sensor2 = 1;
			sensor1 = 0;
		} 
		
		if(count >= 21) 
			count = 0;


		data2 = USART_Receive();
		USART_Transmit(data2);

		
		if(data2 == 0x52){ //if data = R
	
			//save the distance
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
			if(final <= 25){ //vibrate if less than 24 inches away
				if(sensor2 == 1){
					PORTB = (0 << PB7);
					PORTD = (1 << PD4);				

					PORTB = (1 << PB1);
					delay_ms(1000);
					PORTB = (0 << PB1);
					
					PORTD = (0 << PD4);
					PORTB = (1 << PB7);
				}
				if(sensor1 == 1){
					PORTB = (0 << PB2);
					PORTD = (1 << PD3); //TRI COLOR LED SENSOR I3
					
					PORTD = (1 << PD6);
					delay_ms(1000);
					PORTD = (0 << PD6);
				
					PORTD = (0 << PD3); //TRI COLOR LED SENSOR I3	
					PORTB = (1 << PB2);
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


//General short delays
void delay_ms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 90 ; y++){
      for ( z = 0 ; z < 6 ; z++){
        asm volatile ("nop");
      }
    }
  }
}
