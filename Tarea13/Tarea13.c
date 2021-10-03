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
 
  int1 Bandera;// InU=0;
  int8 mm=0, ss=0, InU=0;
  int16 Temp=0;
  int16 step=0, M=0;
 #INT_TIMER1
void Tim1(){
   
   if (InU== 1 && Temp<step){
     Temp++;
        if (Temp%300==0){
            mm++;
            ss=0;
        }
        else{
            ss=(Temp%300)/5;
            //lcd_putc("\f");
        }
   }
   else if(InU==1 && Temp==step){
      Temp=0;
      InU=0;
      //mm=0; ss=0;
      output_high(PIN_A0);
      output_high(PIN_A1);
      output_high(PIN_A2);
   }
   
   set_timer1(15536);
}
 
/* void run_func(int8 numf){ //Puede llamar a varias funciones por menu seleccionado
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
 
 */

 
 void main(){
 
   //LCD y configuraciones de pines
   lcd_init();
   set_tris_a(0b01111000);
   output_a(0);
   set_tris_e(0b011);
   
   //Timer
   ENABLE_INTERRUPTS(INT_TIMER1);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
   set_timer1(15536);
   enable_interrupts(GLOBAL);
   
   
   //Eeprom y menus 
   if (read_eeprom(0)==0xff ){ //Revisa lo que hay en la eeprom
      write_eeprom(0,1);
    }
    
    if (input(PIN_E1)== 0){   //Menu configuración      
        Bandera=0;
   }
   else{  //Menu usuario  
        Bandera=1;
   }
   
   M=read_eeprom(0);
   step=M*60*5;
   
   
   //CICLOS
      while(Bandera==1){
         //Si el usuario presiona el boton comienza a contar
         if (input(PIN_A3)== 0 && InU==0){
            mm=0; ss=0;
            InU=1;
            output_low(PIN_A0);
            output_low(PIN_A1);
            output_low(PIN_A2);               
         }
         
      //lcd_putc("\f");
         lcd_gotoxy(1,1);
         printf(lcd_putc, "%02u : %02u", mm,ss);
      }
      while(Bandera==0)
     {
      
         //Para moverse hacia arriba en el menu presionar el boton
         if(input(PIN_A5)== 0){           
            M++;
            delay_ms(300);
            lcd_putc("\f");
         }
         
         //Para moverse hacia abajo en el menu presionar el boton
         if(input(PIN_A4)== 0){           
            //Ciclico de menu 0 a 5
            if( M==0){
               M= 99; 
            }
            else{
            M--;}
            delay_ms(300);
            lcd_putc("\f");
         }
         
         //Ciclico de menu 99 a 0
         if (M>99){          
            M=0;
         }
         
       //Impresión en pantalla del numero a confiigurar
       lcd_gotoxy(1,1);
       printf(lcd_putc, "%Lu :00", M);
       
       if (input(PIN_E1)== 0){         
            delay_ms(200);
            write_eeprom(0,M);
         }

     }
 }
