#include "stm32f10x.h"

/*****************************************************************
Preambule : indiquez ici les periopheriques que vous avez utilisez
*****************************************************************/

// GPIOA   : broche 6 pour controler la LED verte
// GPIOC : broche 13 pour detecter l'appui du bouton


/*****************************************************************
Declaration des fonctions²²
*****************************************************************/
int rand(void);
void configure_gpio_pa5(void) ;
void configure_gpio_pc13(void) ;
void set_gpio(GPIO_TypeDef *GPIO, int n) ;
void reset_gpio(GPIO_TypeDef *GPIO, int n) ;
void configure_timer(TIM_TypeDef *TIM, int psc, int arr) ;
void configure_it(void) ;
void start_timer(TIM_TypeDef *TIM) ;
void stop_timer(TIM_TypeDef *TIM) ;
void clignotLed();
/*****************************************************************
Varibales globales
 *****************************************************************/
 static int buttonState = 0x0;
/*****************************************************************
MAIN
*****************************************************************/

int main(void){
	
    // Configuration des ports d'entree/sortie


	configure_gpio_pa5();
	configure_gpio_pc13();
    // Ecrire la suite du code
	// Activation de l'horloge dans TIM
	   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
	   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    configure_timer(TIM3,2000,64799);
		configure_timer(TIM2,1999,10799);
		configure_it();
		start_timer(TIM3);
    // Boucle d'attente du processeur
	while (1)
		
   /*buttonState = GPIOC -> IDR & ( 0x1 << 13);*/
	return 0;
}

/*****************************************************************
Corps des fonctions
*****************************************************************/

/**
Configure la broche 5 du port A (led verte)
*/
void configure_gpio_pa5(void){
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0xF << 20);
	GPIOA->CRL |= (0x1 << 20);
}

/**
Configure la broche 13 du port C (bouton USER) 
*/
void configure_gpio_pc13(void) {
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOC -> CRH &= ~(0xF << 20);
	GPIOC -> CRH |= (0x1 << 22); // le bit n°22 de CRH prend "1"
}

/**
Met a 1 la broche n du port GPIO
*/
void set_gpio(GPIO_TypeDef *GPIO, int n) {
GPIO-> ODR |= (0x1 << n);
}

/**
Met a 0 la broche n du port GPIO
*/
void reset_gpio(GPIO_TypeDef *GPIO, int n) {	
GPIO-> ODR &= ~(0x01 << n);
}

/**
Configure la periode du timer TIM en fonction des parametres
psc (prescaler) et arr (autoreload) sans lancer le timer
*/
void configure_timer(TIM_TypeDef *TIM, int psc, int arr) {
	
	TIM-> ARR = arr;
	TIM-> PSC = psc;

}

/**
Demarre le timer TIM
*/
void start_timer(TIM_TypeDef *TIM) {
	TIM->CR1 = TIM->CR1 | TIM_CR1_CEN;
}

/**
Arrete le timer TIM
*/
void stop_timer(TIM_TypeDef *TIM) {
TIM->CR1 = TIM->CR1 & ~(0x01<< 0);
}

/**
Configure toutes les interruptions du systeme
*/
void configure_it(void) {
	TIM2-> DIER |= TIM_DIER_UIE; // validation de l'interruption pour timer2
	  NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
	
		TIM3-> DIER |= TIM_DIER_UIE; // validation de l'interruption pour timer3
	  NVIC->ISER[0] |= NVIC_ISER_SETENA_29;
}

/*****************************************************************
Fonctions d'interruption
*****************************************************************/
void TIM3_IRQhandler (void){
reset_gpio(GPIOA,5);
	/*stop_timer(TIM3);
	start_timer(TIM2);*/
}
void TIM2_IRQhandler (void){
set_gpio(GPIOA,5);
/*	stop_timer(TIM2);
	start_timer(TIM3);*/
	if (buttonState & ( 0x1 << 13) ){
		stop_timer(TIM3);
		stop_timer(TIM2);
		clignotLed();
	}
}

/*****************************************************************
Fonctions pre-definies
*****************************************************************/
void clignotLed (void){
		set_gpio(GPIOA,5);
}
	
/**
Retourne une valeur entiere aleatoire comprise entre 800 et 1800
*/
int rand(){
	static int randomseed = 0;
	randomseed = (randomseed * 9301 + 49297) % 233280;
	return 800 + (randomseed % 1000);
}

