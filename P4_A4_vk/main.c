#include "LPC17XX.h"
#include <math.h>
//#include "cmsis_os.h"
//variables globales
uint16_t prim,dato,digito;
//variables para bcd
uint16_t d_m,u_m,cent,dec,unid;
//variables para las interrupciones
uint16_t binbcd,maximo=65535, minimo=2,arr_abaj,num4; 
//variables para el SysTick
uint16_t i=1,j=0,sys=0;

void SysTick_Handler(){ 
		j++;
		if(j==25){
			i++;
			sys=0;
			j=0;
		}	
}
	
void configIRQ(){
	LPC_PINCON->PINSEL4|=1<<22; // P2.11 como EINT1
	LPC_PINCON->PINSEL4|=1<<24;	//P2.12 como EINT2
	LPC_PINCON->PINSEL4|=1<<20;	//P2.10 como EINT0
	
	LPC_SC->EXTMODE|=0x7;
	LPC_SC->EXTPOLAR=0x0;
	
	//Doy la misma prioridad pero diferente subprioridad
	NVIC_SetPriorityGrouping(4);
	NVIC_SetPriority(EINT0_IRQn,5);
	NVIC_SetPriority(EINT1_IRQn,6);
	NVIC_SetPriority(EINT2_IRQn,4);

	//interrupciones habilitadas
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
}	

void configPines(){ 
	//entradas==0
	LPC_GPIO0->FIODIR &=0x808FFFFF; //(1<<23)&(1<<24)&(1<<25)&(1<<26)&(1<<27)&(1<<28)&(1<<29)&(1<<30);
	LPC_GPIO1->FIODIR	&=0x3F03FFFF; //(1<<18)&(1<<19)&(1<<20)&(1<<21)&(1<<22)&(1<<23)&(1<<30)&(1<<31);
	LPC_GPIO2->FIODIR &=0xFFFFE3FF; //(1<<11);(1<<12);(1<<10)
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
	
	LPC_GPIO0->FIOPIN &=~(1<<3)&(1<<2)&(1<<4)&(1<<5)&(1<<6)&(1<<7)&(1<<8);
	LPC_GPIO1->FIOPIN &=~(1<<0)&(1<<1)&(1<<4)&(1<<8)&(1<<9)&(1<<10)&(1<<14)&(1<<15)&(1<<16)&(1<<17);
	LPC_GPIO4->FIOPIN &=~(1<<28)&(1<<29);
	
	LPC_GPIO0->FIOSET |= (unid1%2)<<3;
	unid1=unid1/2;
	LPC_GPIO0->FIOSET |= (unid1%2)<<2;
	unid1=unid1/2;
	LPC_GPIO1->FIOSET |= (unid1%2)<<0;
	unid1=unid1/2;
	LPC_GPIO1->FIOSET |= (unid1%2)<<1;
	
	LPC_GPIO1->FIOSET |= (dec1%2)<<4;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<8;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<9;
	dec1=dec1/2;
	LPC_GPIO1->FIOSET |= (dec1%2)<<10;
	
	LPC_GPIO1->FIOSET |= (cent1%2)<<14;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<15;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<16;
	cent1=cent1/2;
	LPC_GPIO1->FIOSET |= (cent1%2)<<17;
	
	LPC_GPIO4->FIOSET |= (u_m1%2)<<29;
	u_m1=u_m1/2;
	LPC_GPIO4->FIOSET |= (u_m1%2)<<28;
	u_m1=u_m1/2;
	LPC_GPIO0->FIOSET |= (u_m1%2)<<4;
	u_m1=u_m1/2;
	LPC_GPIO0->FIOSET |= (u_m1%2)<<5;
	
	LPC_GPIO0->FIOSET |= (d_m1%2)<<6;
	d_m1=d_m1/2;
	LPC_GPIO0->FIOSET |= (d_m1%2)<<7;
  d_m1=d_m1/2;
	LPC_GPIO0->FIOSET |= (d_m1%2)<<8;
}
void binario_a_bcd(){
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
	unid=aux-dec*10;
	
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

void representa(){
	uint16_t digito=5,pos=0;
	
	binario_a_bcd();
	
	if(!binbcd)
		muestrabinario();
	else
		muestrabcd();
	
			for(i=1; i<=80;){	//Bucle de 2 segundos
			if(sys==0){
				if(i==1){
						displaydigito(prim,digito);
						digito--;
						ld3(pos,prim); // en el instante 0s se considera el bit 0 de la variable numero
						
					}
					if(i%5==0){// han pasado 125ms
							pos++;
						if(pos<=15){
									ld2();		//Conmutamos el LD2
									ld3(pos,prim); //consideramos un bit determinado de un numero primo
									}
									}		
						if(i==16){		//han pasado 400ms
								displaydigito(prim,digito);
								digito--; //representamos el siguiente digito
							}
						if(i==32){		//han pasado otros 400ms
								displaydigito(prim,digito);
								digito--; //representamos el siguiente digito
							}
						if(i==48){		//han pasado otros 400ms
								displaydigito(prim,digito);
								digito--; //representamos el siguiente digito
							}
						if(i==64){		//han pasado otros 400ms
								displaydigito(prim,digito);
								digito--; //representamos el siguiente digito
						}				
			sys=1;}
			
				}
			if(i==81){	
			i=1;
			sys=0;
			}
}

void calculaprimo(){
	uint16_t b; 
	uint16_t lim,a,suprimo,num,num2;
	for(b=0;b<1000;b++){ //por si se cambia el valor de arr_abaj muchas veces
	if(arr_abaj==0){ // Va del minimo al maximo
		for(num=minimo;num<=maximo;num++){
			if(minimo>num)//si el numero es menor que el minimo se empieza desde el minimo
			{num=minimo;}
			if(maximo<num)// si el numero es menor que el maximo se empieza desde el minimo
			{num=minimo;}
				suprimo=0;
				if(num==1) num++;
				if(num==0) num+=2;
			
				if(num%2!=0||num==2){
					lim=(sqrt(num)+1);
					for(a=0; a<=lim;a++) {
						if(num%a==0)
								suprimo++;
						if(suprimo==2)
								break;
}	
				if(suprimo==1|| num==2){
						prim=num;
						representa();
					}
	
		}
	if(arr_abaj==1){
		for(num2=num;num2>=minimo;num2--){
			if(minimo>num2)//si el numero es menor que el minimo, se empieza desde el maximo
			{num2=maximo;}
			if(maximo<num2)//si el numero es mayor que el maximo, se empieza desde el maximo
			{num2=maximo;}
			suprimo=0;
			if(num==1) num2=maximo;
			if(num==0) num2=maximo;
		
		if(num2%2!=0||num2==2){
				lim=(sqrt(num2)+1);
		for(a=0; a<=lim; a++) {
				if(num2%a==0)
						suprimo++;
				if(suprimo==2)
						break;
		}	
		if(suprimo==1|| num2==2){				
				prim=num2;
				representa();
			}
				}	
		if(arr_abaj==0){
			num=num2;
				break;
			}
		if(num2==minimo)
			num2=maximo;
			}
		}
	if(num==maximo)
		num=minimo-1;
	}
}	

	}
}

int main(){
	
	configPines();
	configIRQ();
	
	SysTick->LOAD=100000; //1ms	
	NVIC_SetPriority(SysTick_IRQn,0); //le damos la mayor prioridad al SysTick
  SysTick->VAL= 0;           
  SysTick->CTRL=0x7; //habilitamos
	
	calculaprimo();
	
	while(1);
}
			
void EINT0_IRQHandler(){
LPC_SC->EXTINT|=0x00000001; //limpiar el flag
			binbcd^=1; //cambia el valor cada vez que se produce la interrupcion
}	
void EINT1_IRQHandler(){
	LPC_SC->EXTINT|=0x00000002; //limpiar el flag
	
	if((LPC_SC->EXTPOLAR &(1<<1))==0)	{
		minimo=leedato();	
		LPC_SC->EXTPOLAR |=(1<<1);			//flanco de bajada = numero minimo
		
	}
	else{
		maximo=leedato();	
	LPC_SC->EXTPOLAR &=(~(1<<1));			// flanco de subidad = numero maximo
	}
}
void EINT2_IRQHandler(){
	LPC_SC->EXTINT|=0x00000004; //limpiar el flag
	arr_abaj^=1; // cambia el valor cada vez que se produce la interrupcion
}
