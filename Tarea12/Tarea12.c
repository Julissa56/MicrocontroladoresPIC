#include <16f877a.h>
#include <stdlib.h>
#fuses hs, nowdt
#use delay (clock=4M)
#use rs232(baud=9600, xmit=PIN_C6, rcv =PIN_C7)
#use fast_io(A)
#use fast_io(E)
#use fast_io(C)

#use standard_io(B)
   #define LCD_ENABLE_PIN  PIN_B2
   #define LCD_RS_PIN      PIN_B0
   #define LCD_RW_PIN      PIN_B1
   #define LCD_DATA4       PIN_B3
   #define LCD_DATA5       PIN_B4
   #define LCD_DATA6       PIN_B6
   #define LCD_DATA7       PIN_B7
   
   #include <lcd.c>
   
 enum funciones {med, cal, in, P4, P5};
 int8 item=0, n_menus=5, flag_P5;
 int1  flag_P4=0, flag_P3=0, flag_P2=0, flag_P1=0; //Al leer en la eeprom, si la eeprom no ha sido escrita se van a inicializar con 0 las banderas
 
 char Kb_Terminal;
 char msg1[10]="Encendido";
 char msg2[10]="Apagado  ";
 
 char msgx[10]="Apagado"; 
 char msgx2[10]="Apagado"; 
 char msgx3[10]="Apagado"; 
 char msgx4[10]="Apagado";
 char msgx5[10]="Apagado";
 
 
 //FUNCIONES
 void Cambio_msg_P1(){
   if (flag_P1==1){    
      strcpy(msgx, msg1);
    }
    else{     
      strcpy(msgx, msg2);
    }
 }
 
  void Cambio_msg_P2(){
  if (flag_P2==1){   
      strcpy(msgx2, msg1);
    }
    else{ 
      strcpy(msgx2, msg2);
    }
  }
  
  void Cambio_msg_P3(){
   if (flag_P3==1){
      strcpy(msgx3, msg1);
    }
    else{
      strcpy(msgx3, msg2);
    }
  }
  
  void Cambio_msg_P4(){
    if (flag_P4==1){
      strcpy(msgx4, msg1);
    }
    else{
      strcpy(msgx4, msg2);
    }
  }
  
  void Cambio_msg_P5(){
      //guardar el valor de flag 5 en la eeprom y leerlo
    if (flag_P5==1){                                                       ////////////
      strcpy(msgx5, msg1);
    }
    else{
      strcpy(msgx5, msg2);
    }
  }

   void Bandera1(){
      if (flag_P1==0){
      flag_P1=1;
      write_eeprom(0,1);
      //delay_ms(300); El delay es para evitar rebote
    }
    else{
      flag_P1=0;
      write_eeprom(0,0);
      //delay_ms(300);
    }
  }
  
  void Bandera2(){
      if (flag_P2==0){
      flag_P2=1;
      write_eeprom(1,1);
      //delay_ms(300); El delay es para evitar rebote
    }
    else{
      flag_P2=0;
      write_eeprom(1,0);
      //delay_ms(300);
    }
  }

  void Bandera3(){
      if (flag_P3==0){
      flag_P3=1;
      write_eeprom(2,1);
      //delay_ms(300); El delay es para evitar rebote
    }
    else{
      flag_P3=0;
      write_eeprom(2,0);
      //delay_ms(300);
    }
  }

  void Bandera4(){
      if (flag_P4==0){
      flag_P4=1;
      write_eeprom(3,1);
      //delay_ms(300); El delay es para evitar rebote
    }
    else{
      flag_P4=0;
      write_eeprom(3,0);
      //delay_ms(300);
    }
  }
 
  void Bandera5(){
      if (flag_P5==0){
      flag_P5=1;
      write_eeprom(4,1);
      //delay_ms(300); El delay es para evitar rebote
    }
    else{
      flag_P5=0;
      write_eeprom(4,0);
      //delay_ms(300);
    }
  }
  
void P1(){
   if (flag_P1==0){
      output_low(PIN_A0);
   }
   else{
      output_high(PIN_A0);
   }
  //Si la bandera P1 marca 0, apagar el pin. Sino encenderlo
 }
 
void P2(){
   if (flag_P2==0){
      output_low(PIN_A1);
   }
   else{
      output_high(PIN_A1);
   }
 }
 
void P3(){
   if (flag_P3==0){
      output_low(PIN_A2);
   }
   else{
      output_high(PIN_A2);
   }
 }
 
void P4(){
   if (flag_P4==0){
      output_low(PIN_A3);
   }
   else{
      output_high(PIN_A3);
   }
 }
 
void P5(){
   if (flag_P5==0){
      output_low(PIN_E2);
   }
   else{
      output_high(PIN_E2);
   }
 }
 
 
 void run_func(int8 numf){ //Puede llamar a varias funciones por menu seleccionado
   switch (numf){          //Al presionar el select de un led especifico cambia su bandera, su msg y su estado
      case 0: 
         Bandera1();
         Cambio_msg_P1();//Estos dos podrian ser una nueva funcion        
         P1();       
      break;
      
      case 1: 
         Bandera2();
         Cambio_msg_P2();          
         P2();
         break;
      
      case 2: 
         Bandera3();
         Cambio_msg_P3();        
         P3(); 
      break;
      
      case 3:          
         Bandera4();
         Cambio_msg_P4();
         P4(); 
      break;
      
      case 4: 
         Bandera5();                                                 /////////
         Cambio_msg_P5();         
         P5(); 
      break;
   }
 }
 
 
 
 void main(){
 
   lcd_init();
   
   set_tris_a(0b01110000);
   output_a(0);
   set_tris_e(0b011);
   
   if (read_eeprom(0)==0xff || read_eeprom(1)==0xff || read_eeprom(2)==0xff || read_eeprom(3)==0xff || read_eeprom(4)==0xff){ //Revisa lo que hay en la eeprom
      write_eeprom(0,0);
      write_eeprom(1,0); //  Si no, las banderas se asignan a cero
      write_eeprom(2,0);
      write_eeprom(3,0);
      write_eeprom(4,0);
    }
    
      flag_P1=read_eeprom(0); //Si hay algo, el valor de las banderas cambia conforme lo almacenado en la eepprom    
      flag_P2=read_eeprom(1);
      flag_P3=read_eeprom(2);            
      flag_P4=read_eeprom(3);      
      flag_P5=read_eeprom(4);
      
      Cambio_msg_P1();
      Cambio_msg_P2();
      Cambio_msg_P3();
      Cambio_msg_P4();
      Cambio_msg_P5();
      
      P1();
      P2();
      P3();
      P4();
      P5();
      
      printf("Bienvenido! Por favor ingrese un numero \n\r");
    //Bandera= Lo que esta guardado en la eeprom
    //Llamar metodos de led y mensaje
      while(true)
     {
      
         //Para moverse hacia arriba en el menu presionar el boton
         if(input(PIN_A5)== 0){           
            item++;
            delay_ms(300);
            lcd_putc("\f");
         }
         
         //Para moverse hacia abajo en el menu presionar el boton
         if(input(PIN_A4)== 0){           
            //Ciclico de menu 0 a 5
            if( item==0){
               item= n_menus- 1; 
            }
            else{
            item--;}
            delay_ms(300);
            lcd_putc("\f");
         }
         
         //Ciclico de menu 5 a 0
         if (item> (n_menus- 1)){          
            item=0;
         }
         
       
        //Imprime en pantalla el parametro y su estado
         switch(item){                    
            case 0:
               lcd_gotoxy(1,1);
               printf(lcd_putc, "P1:");
               
               lcd_gotoxy(1,2);
               printf(lcd_putc, msgx);
            break;
            
            case 1:
               lcd_gotoxy(1,1);
               printf(lcd_putc, "P2:");
               
               lcd_gotoxy(1,2);
               printf(lcd_putc,  msgx2);   
            break;
            
            case 2:
               lcd_gotoxy(1,1);
               printf(lcd_putc, "P3:");
               
               lcd_gotoxy(1,2);
               printf(lcd_putc,  msgx3);
            break;
            
            case 3:
               lcd_gotoxy(1,1);
               printf(lcd_putc, "P4:");
               
               lcd_gotoxy(1,2);
               printf(lcd_putc,  msgx4);
            break;
            
            case 4:
               lcd_gotoxy(1,1);
               printf(lcd_putc, "P5:");
               
               lcd_gotoxy(1,2);
               printf(lcd_putc,  msgx5);
            break;
         }
         
          //Selecciona el menu que se va a configurar en base al valor item dado por los botones 
         if (input(PIN_E1)== 0){         
            delay_ms(200);
            run_func(item);
         }
         
          //Selecciona el menu que se va a configurar en base al valor item dado por la terminal
         if(kbhit()){
          Kb_Terminal=getc();
          Kb_Terminal=Kb_Terminal-49; //equivalente de char a decimal
          
            if(Kb_Terminal>=0 && Kb_Terminal< n_menus){
               run_func(Kb_Terminal);
               
               printf("\n\r Usted cambio el estado de: LED %c \n\r", Kb_Terminal+49 );
               printf("\n\r\n\r");
               printf("La nueva tabla de estados es: \n\r\n\r");
               
               printf("  LED 1: %s \n\r", msgx);
               printf("  LED 2: %s \n\r", msgx2);
               printf("  LED 3: %s \n\r", msgx3);
               printf("  LED 4: %s \n\r", msgx4);
               printf("  LED 5: %s \n\r", msgx5);
               
               printf("\n\r\n\r");
               printf("Por favor ingrese un numero \n\r");
               //Si el numero que detectó la terminal esta entre el 1 y el  5
               //va a cambiar el estado de la bandera referente al parametro x (lo que va llevar a un cambio en la escritura de la eeprom tambien)
               //va a cambiar el mensaje que se va a mostrar en el LCD
               //y va a cambiar el estado del led
               
               //queremos tambien que imprima en pantalla el nuevo estado en la configuracion de todos los leds
            }
          }
    
     }
 }
