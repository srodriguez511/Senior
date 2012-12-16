//USART Program
//The ATMEGA32 will recieve a serial transmission and send it
//Frequency - 8MHZ Operating at 9600BPS
#include <avr/io.h>
void USART_Init(unsigned int baud); //Initlizae Baud
void USART_Transmit(unsigned char data); //Transmission
unsigned char USART_Receive(void); //Receive
int main(){
	unsigned char data2; //Hold the data we send/receive
	USART_Init(51); //Initilize to 9600bps
	for(;;){
		data2 = USART_Receive();
		USART_Transmit(data2);
	}
	return 0;
}
void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (baud >> 8);
	UBRRL = baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)  | (3<<UCSZ0) ;
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}
