#include <avr/io.h>
#include <util/delay.h>

void myDecoder(int num){
		switch (num){
		case 0:
			PORTA = 0x7E; //7-segment 0111 1110
			PORTC &= 0xF8; //LED 1111 1000
			break;
		case 1:
			PORTA = 0x0C; //7-segment 0000 1100
			PORTC |= (1 << 0);
			PORTC &= ~(1 << 1);
			PORTC &= ~(1 << 2);
			break;
		case 2:
			PORTA = 0xB6; //7-segment 1011 0110
			PORTC &= ~(1 << 0);
			PORTC |= (1 << 1);
			PORTC &= ~(1 << 2);
			break;
		case 3: // 1001 1110
			PORTA = 0x9E;
			PORTC |= (1 << 0);
			PORTC |= (1 << 1);
			PORTC &= ~(1 << 2);
			break;
		case 4: // 1100 1100
			PORTA = 0xCC;
			PORTC &= ~(1 << 0);
			PORTC &= ~(1 << 1);
			PORTC |= (1 << 2);
			break;
		case 5: // 1101 1010
			PORTA = 0xDA;
			PORTC |= (1 << 0);
			PORTC &= ~(1 << 1);
			PORTC |= (1 << 2);
			break;
		case 6: // 1111 1010
			PORTA = 0xFA;
			PORTC &= ~(1 << 0);
			PORTC |= (1 << 1);
			PORTC |= (1 << 2);
			break;
		case 7: // 0000 1110
			PORTA = 0x0E;
			PORTC |= (1 << 0);
			PORTC |= (1 << 1);
			PORTC |= (1 << 2);
			break;
		}
	}

int main(){

	DDRA |= 0xFE; // 7-SEGMENT - port - output // 1111 1110 ///
	DDRC |= (1 << PC6); // common cathode ///
	PORTC |= (1 << PC6); //enable of cathode ///
	DDRD &= 0xC3; //PUSH BUTTONS - pin - input // 1100 0011 ///
	DDRC |= 0x07; //LEDs - port - output // 0000 0111 ///
	DDRC |= (1 << PC5); // BUZZER - port - output ///


	unsigned int num = 0;
	// setting zero as initial value
	myDecoder(num);
	//for resume/pause
	char state = 1;

	while(1){
		// push button D2 - increment
		if(!(PIND & (1 << PD2)) & state){
			_delay_ms(30);
			if(!(PIND & (1 << PD2)) & state){
				if(num != 7){
					//INCREMENT
					num++;
				}
				else if (num == 7){
					// BUZZER
					for(int i = 0; i < 3; i++){
					PORTC |= (1 << PC5);
					_delay_ms(500);
					PORTC &= ~(1 << PC5);
					_delay_ms(500);
				}

				}
				myDecoder(num);
			}
			while(!(PIND & (1 << PD2)) & state){}
		}

		// push button D3 - decrement
		else if(!(PIND & (1 << PD3)) & state){
			_delay_ms(30);
			if(!(PIND & (1 << PD3)) & state){
				if(num != 0){
					//decrement
					num--;
				}
				else if (num == 0){
					// BUZZER
					for(int i = 0; i < 3; i++){
						PORTC |= (1 << PC5);
						_delay_ms(500);
						PORTC &= ~(1 << PC5);
						_delay_ms(500);
					}

				}
				myDecoder(num);
			}
			while(!(PIND & (1 << PD3)) & state){}
		}

		// push button D4 - pause/resume
		if(!(PIND & (1 << PD4))){
			_delay_ms(30);
			if(!(PIND & (1 << PD4))){
				if (state) state = 0;
				else if (!state) state = 1;
			}
			while(!(PIND & (1 << PD4))){}
		}

		//push button D5 - reset
		if(!(PIND & (1 << PD5))){
			_delay_ms(30);
			if(!(PIND & (1 << PD5))){
				state = 1;
				// reset the 7-segment
				num = 0; myDecoder(num);

				//flash LEDs
				for(int i = 0; i < 3; i++){ // 0000 0111
					PORTC |= 0x07;
					_delay_ms(700);
					PORTC &= ~(0x07);
					_delay_ms(700);
				}
			}
			while(!(PIND & (1 << PD5))){}
		}



	}

}
