#include "LPC17XX.h"
#include <math.h>
#include "cmsis_os.h"
//variables globales
uint16_t prim,dato;
uint16_t d_m,u_m,cent,dec,unid;

void delay(uint32_t n){ 
	int i;
		for(i=0; i<=n; i++){}
}
void configPines(){ 
	//entradas==0
	LPC_GPIO0->FIODIR &=0x808FFFFF; //(1<<23)&(1<<24)&(1<<25)&(1<<26)&(1<<27)&(1<<28)&(1<<29)&(1<<30);
	LPC_GPIO1->FIODIR	&=0x3F03FFFF; //(1<<18)&(1<<19)&(1<<20)&(1<<21)&(1<<22)&(1<<23)&(1<<30)&(1<<31);
	LPC_GPIO2->FIODIR &=0xFFFFFBFF; //1<<10);
	//Salidas==1
	LPC_GPIO0->FIODIR |=0x000001FC; //(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8);
	LPC_GPIO1->FIODIR |=0x0003C713;	//(1<<0)|(1<<1)|(1<<4)|(1<<8)|(1<<9)|(1<<10)|(1<<14)|(1<<15)|(1<<16)|(1<<17);
	LPC_GPIO4->FIODIR	|=0x30000000;	//(1<<28)|(1<<29);
	LPC_GPIO2->FIODIR |=0x000000FF;	//(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7);
	LPC_GPIO3->FIODIR	|=0x06000000;	//(1<<25)|(1<<26);
}


uint16_t leedato(){ //toma de datos de los switches 
	dato=0;
	dato|=(LPC_GPIO0->FIOPIN & (1<<26))>>26; 	//BIT O
	dato|=(LPC_GPIO0->FIOPIN & (1<<25))>>24;	//BIT 1
	dato|=(LPC_GPIO0->FIOPIN & (1<<24))>>22;	//BIT 2
	dato|=(LPC_GPIO0->FIOPIN & (1<<23))>>20;	//BIT 3
	dato|=(LPC_GPIO1->FIOPIN & (1<<31))>>27;	//BIT 4
	dato|=(LPC_GPIO1->FIOPIN & (1<<30))>>25;	//BIT 5
	dato|=(LPC_GPIO0->FIOPIN & (1<<28))>>22;	//BIT 6
	dato|=(LPC_GPIO0->FIOPIN & (1<<27))>>20;	//BIT 7
	dato|=(LPC_GPIO0->FIOPIN & (1<<29))>>21;	//BIT 8
	dato|=(LPC_GPIO0->FIOPIN & (1<<30))>>21;	//BIT 9
	dato|=(LPC_GPIO1->FIOPIN & (1<<18))>>8;		//BIT 10
	dato|=(LPC_GPIO1->FIOPIN & (1<<19))>>8;		//BIT 11
	dato|=(LPC_GPIO1->FIOPIN & (1<<20))>>8;		//BIT 12
	dato|=(LPC_GPIO1->FIOPIN & (1<<21))>>8;		//BIT 13
	dato|=(LPC_GPIO1->FIOPIN & (1<<22))>>8;		//BIT 14
	dato|=(LPC_GPIO1->FIOPIN & (1<<23))>>8;		//BIT 15
	
return dato;
}
void muestrabinario (){ //representacion en binario
	LPC_GPIO0->FIOPIN &=~(1<<3)&(1<<2)&(1<<4)&(1<<5)&(1<<6)&(1<<7)&(1<<8);
	LPC_GPIO1->FIOPIN &=~(1<<0)&(1<<1)&(1<<4)&(1<<8)&(1<<9)&(1<<10)&(1<<14)&(1<<15)&(1<<16)&(1<<17);
	LPC_GPIO4->FIOPIN &=~(1<<28)&(1<<29);
	//LED 1
	if((prim & (1<<0))==0)
	LPC_GPIO0->FIOCLR |=(1<<3); // el led esta apagado
	else
	LPC_GPIO0->FIOSET |=(1<<3);	// el led esta encendido
	
	//LED 2
	if((prim & (1<<1))==0)
	LPC_GPIO0->FIOCLR |=(1<<2);	
	else
	LPC_GPIO0->FIOSET |=(1<<2);		
	
	//LED 3
	if((prim & (1<<2))==0)
	LPC_GPIO1->FIOCLR |=(1<<0);	 
	else
	LPC_GPIO1->FIOSET |=(1<<0);	
	
	//LED 4
	if((prim & (1<<3))==0)
	LPC_GPIO1->FIOCLR |=(1<<1);
	else
	LPC_GPIO1->FIOSET |=(1<<1);	
	
	//LED 5
	if((prim & (1<<4))==0)
	LPC_GPIO1->FIOCLR |=(1<<4);	
	else
	LPC_GPIO1->FIOSET |=(1<<4);	
	
	//LED 6
	if((prim & (1<<5))==0)
	LPC_GPIO1->FIOCLR |=(1<<8);
	else
	LPC_GPIO1->FIOSET |=(1<<8);	
	
	//LED 7
	if((prim & (1<<6))==0)
	LPC_GPIO1->FIOCLR |=(1<<9);
	else
	LPC_GPIO1->FIOSET |=(1<<9);	
	
	//LED 8
	if((prim & (1<<7))==0)
	LPC_GPIO1->FIOCLR |=(1<<10);	
	else
	LPC_GPIO1->FIOSET |=(1<<10);	
	
	//LED 9
	if((prim & (1<<8))==0)
	LPC_GPIO1->FIOCLR |=(1<<14);
	else
	LPC_GPIO1->FIOSET |=(1<<14);	
	
	//LED 10
	if((prim & (1<<9))==0)
	LPC_GPIO1->FIOCLR |=(1<<15);	 
	else
	LPC_GPIO1->FIOSET |=(1<<15);
	
	//LED 11
	if((prim & (1<<10))==0)
	LPC_GPIO1->FIOCLR |=(1<<16);	
	else
	LPC_GPIO1->FIOSET |=(1<<16);	
	
	//LED 12
	if((prim & (1<<11))==0)
	LPC_GPIO1->FIOCLR |=(1<<17);	 
	else
	LPC_GPIO1->FIOSET |=(1<<17);	
	
	//LED 13
	if((prim & (1<<12))==0)
	LPC_GPIO4->FIOCLR |=(1<<29);	
	else
	LPC_GPIO4->FIOSET |=(1<<29);	
	
	//LED 14
	if((prim & (1<<13))==0)
	LPC_GPIO4->FIOCLR |=(1<<28);	
	else
	LPC_GPIO4->FIOSET |=(1<<28);	
	
	//LED 15
	if((prim & (1<<14))==0)
	LPC_GPIO0->FIOCLR |=(1<<4);	 
	else
	LPC_GPIO0->FIOSET |=(1<<4);	
	
	//LED 16
	if((prim & (1<<15))==0)
	LPC_GPIO0->FIOCLR |=(1<<5);	 
	else
	LPC_GPIO0->FIOSET |=(1<<5);		
}

void muestrabcd (){//representacion en bcd
	
	uint16_t unid1,dec1,cent1,u_m1,d_m1;
	unid1=unid; dec1=dec; cent1=cent; u_m1=u_m; d_m1=d_m;
	
	//sentencias para limpiar el puerto de la representación anterior;
	LPC_GPIO0->FIOPIN &=~(1<<3)&(1<<2)&(1<<4)&(1<<5)&(1<<6)&(1<<7)&(1<<8);
	LPC_GPIO1->FIOPIN &=~(1<<0)&(1<<1)&(1<<4)&(1<<8)&(1<<9)&(1<<10)&(1<<14)&(1<<15)&(1<<16)&(1<<17);
	LPC_GPIO4->FIOPIN &=~(1<<28)&(1<<29);
	
	//representacion del numero en BCD
	//Primero se representan las unidades
	LPC_GPIO0->FIOSET |= (unid1%2)<<3; //4 sentencias porque el maximo numero (9), hasta llegar a 0, se divide 4 veces
	unid1=unid1/2;
	LPC_GPIO0->FIOSET |= (unid1%2)<<2;
	unid1=unid1/2;
	LPC_GPIO1->FIOSET |= (unid1%2)<<0;
	unid1=unid1/2;
	LPC_GPIO1->FIOSET |= (unid1%2)<<1;
	
	//Se representan las decenas
	LPC_GPIO1->FIOSET |= (dec1%2)<<4;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<8;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<9;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<10;
	
	//Se representan las centenas
	LPC_GPIO1->FIOSET |= (cent1%2)<<14;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<15;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<16;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<17;
	
	//Se representan las unidades de millar
	LPC_GPIO4->FIOSET |= (u_m1%2)<<29;
	u_m1=u_m1/2;
	LPC_GPIO4->FIOSET |= (u_m1%2)<<28;
	u_m1=u_m1/2;
	LPC_GPIO0->FIOSET |= (u_m1%2)<<4;
	u_m1=u_m1/2;
	LPC_GPIO0->FIOSET |= (u_m1%2)<<5;
	
	//Se representan las decenas de millar
	LPC_GPIO0->FIOSET |= (d_m1%2)<<6; //solamente tres veces porque el maximo numero es el 6, por tanto 6/2=3; 3/2=1.5; 1.5/2=0.75;
	d_m1=d_m1/2;
	LPC_GPIO0->FIOSET |= (d_m1%2)<<7;
  d_m1=d_m1/2;
	LPC_GPIO0->FIOSET |= (d_m1%2)<<8;
}
void binario_a_bcd(void){
		uint16_t aux;
	//decenas de millar
	d_m=prim/10000;
	aux=prim-d_m*10000;
	
	//unidades de millar
	u_m=aux/1000;
	aux=aux-u_m*1000;
	
	//centenas
	cent=aux/100;
	aux=aux-cent*100;
	
	//decenas
	dec=aux/10;
	
	//unidades
	unid=aux-dec*10;
}
uint8_t isp(){
if(((LPC_GPIO2->FIOPIN & (1<<10))>>10)==1)
		return 1; // NO esta pulsado ISP
else
		return 0; // ISP esta pulsado

}
void muestrabin_o_bcd(){
	
	if(isp()==1) //no esta pulsado ISP
			muestrabcd();
	else
		muestrabinario();
	
}
void display(uint8_t digito){
	switch(digito)
	{
		case 0:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);
		LPC_GPIO2->FIOSET |= (1<<6);				
		break;
		
		case 1:
		LPC_GPIO2->FIOCLR |= (1<<1)|(1<<2);
		LPC_GPIO2->FIOSET |= (1<<0)|(1<<3)|(1<<4)|(1<<5)|(1<<6);		
		break;

		case 2:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<6)|(1<<4)|(1<<3);
		LPC_GPIO2->FIOSET |= (1<<2)|(1<<5);				
		break;
		
		case 3:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<6)|(1<<2)|(1<<3);
		LPC_GPIO2->FIOSET |= (1<<5)|(1<<4);						
		break;
		
		case 4:
		LPC_GPIO2->FIOCLR |= (1<<1)|(1<<2)|(1<<5)|(1<<6);
		LPC_GPIO2->FIOSET |= (1<<0)|(1<<3)|(1<<4);						
		break;
		
		case 5:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<5)|(1<<6)|(1<<2)|(1<<3);
		LPC_GPIO2->FIOSET |= (1<<1)|(1<<4);			
		break;
		
		case 6:
		LPC_GPIO2->FIOCLR |= (1<<5)|(1<<4)|(1<<6)|(1<<2)|(1<<3);
		LPC_GPIO2->FIOSET |= (1<<0)|(1<<1);					
		break;
		
		case 7:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<2);
		LPC_GPIO2->FIOSET |= (1<<3)|(1<<4)|(1<<5)|(1<<6);						
		break;
		
		case 8:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);				
		break;
		
		case 9:
		LPC_GPIO2->FIOCLR |= (1<<0)|(1<<1)|(1<<2)|(1<<6)|(1<<5);
		LPC_GPIO2->FIOSET |= (1<<3)|(1<<4);					
		break;
	}
}
void displaydigito(uint16_t numero,uint8_t digito){
switch(digito)
							{
								case 1:
								display(unid);								
								break;
								
								case 2:
								display(dec);
								break;
								
								case 3:
								display(cent);								
								break;
								
								case 4:
								display(u_m);							
								break;
								
								case 5:
								display(d_m);							
								break;
							}
	
}	
void ld2(){
			LPC_GPIO3->FIOPIN ^= (1<<25);
}
void ld3(uint8_t bit,uint16_t numero){
	if((numero & (1<<bit))==0)
				LPC_GPIO3->FIOSET |=(1<<26); //apaga el LED
	else
				LPC_GPIO3->FIOCLR |=(1<<26); //enciende el LED
}

int main(){
	uint16_t numero,i,j,suprimo;
	
	configPines();
	
	while(1){
	uint16_t auxD=0,auxL=0,digito=5,pos=0;
	uint16_t lim;
	leedato(); //lee el dato de los switches

		
	for(numero=dato;numero<=65535;numero++){//recorreremos todos los numeros que nos piden
		suprimo=0;
			if(numero==1) numero++;
			if(numero==0) numero+=2;
			if((numero%2)!=0 || numero==2){ //solo consideramos los numeros impares y el numero 2
				lim=(sqrt(numero)+1);
				for(j=0; j<=lim;j++){
				
					if((numero%j)==0){
							suprimo++;
							
					}
						}
					if(suprimo==1 || numero==2){	//ahora lo representamos, ya que hemos hallado el numero primo
								prim=numero;
								binario_a_bcd();
								muestrabin_o_bcd();
								ld3(pos,numero); // en el instante 0s se considera el bit 0 de la variable numero
								for(i=0; i<=79; i++){	//Bucle de 2 segundos
										delay(625000);	// retardo de 25 ms
										auxD++;
										auxL++;
						
										if(auxL==5){			// han pasado 125ms	
												auxL=0;
												pos++;
														if(pos<=15){
																ld2();		//Conmutamos el LD2
																ld3(pos,prim); //consideramos un bit determinado de un numero primo
														}
														else		// 2 segundos
																pos=0;
										}		
										if(auxD==16){		//han pasado 400ms
												displaydigito(prim,digito);
												auxD=0; //empezamos a contar 400ms de nuevo
												digito--; //representamos el siguiente digito
												}
							
										if(i==79){		// 2 segundos
												auxD=0; //empezamos a contar 400ms de nuevo
												digito=5; //se empieza con el digito decenas de millar
												break;
										}
								}
					}		
					else
				suprimo=0;
			}
			if(numero==65535) 
				break;
		
	}
}
}
