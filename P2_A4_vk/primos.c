#include <math.h>
#include <LPC17xx.h>

uint16_t primos [6541]; //hay 6541 primos hasta el numero 65535
uint8_t esPrimo(uint16_t);
uint16_t k; //variable global que sirve para recorrer el array primos[]
uint16_t z;
uint8_t esPrimo(uint16_t numero){
		//extern int k;
		unsigned int i,prim;
	
		prim=0; //variable prim que nos dira si el numero es primo o no, un "1" si no es primo; un "0" si fuera primo
		z=sqrt(numero);
		for(i=0;i<=k && primos[i]<=(z+1);i++){ //este bucle recorre el array de primos hasta la posicion "k" en la que el numero es menor que la raiz cuadrada del numero a evaluar +1
				if(numero%primos[i]==0){ //si algun numero es divisible y da resto cero el numero no es primo
						prim=1;
						break;
				}
		}
		if(prim==1) 
			return 0; //si no es primo, la funcion devuelve un "0"
		else 
			return 1; //si el nuemro es primo, la funcion devuelve un "1"					
}
int main(){
	unsigned int numero;
	k=1;
	primos[0]=2;
	for(numero=3; numero<=65535;numero+=2){ //en este bucle no evaluamos los numeros pares
			if(esPrimo(numero)==1)//si la funcion devuelve un uno, es que el numero es primo
				primos[k++]=numero;//se asigna ese numero a la posicion correspondiente del array
	}
}
