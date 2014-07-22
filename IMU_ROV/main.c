#include "Gyro.h"
#include "Accelerometer.h"
#include "Magneto.h"
#include "My_Timer.h"
#include "My_UART.h"
#include "My_DMA.h"
#include "My_I2C.h"

#include "Kalman_Sim.h"



#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

int main(void)
{
  My_Init();
  Init_DMA();
  Init_Timer();
  Init_I2C();
  Init_Sensors();
  Init_UART();


  Kalman_Sim_initialize();

  while(1)
  {
    Read_Accelerometer();
    Calculate_Acc();
    Read_Compass();
    Compass_Heading();
    Calculate_Compass();
    Read_Gyro();
    Calculate_Gyro();

    fgyro[0]    = sen_data.gyro_x;
    fgyro[1]	= sen_data.gyro_y;
    fgyro[2]	= sen_data.gyro_z;

    facc[0]     = sen_data.accel_x;
    facc[1]	= sen_data.accel_y;
    facc[2]	= sen_data.accel_z;

    fmag[0]     = sen_data.magnetom_x;
    fmag[1]	= sen_data.magnetom_y;
    fmag[2]	= sen_data.magnetom_z;


    Kalman_Sim_step();


    if(time != 1)
    {
      if(  (Out1[2] < -1700) && (temp2 == 0) && (temp6 != 2)  )
      {
        temp2 = 1;
        temp6 = 1;
      }

      if( temp2 ==1 )
      {

        if( (Out1[2] > -1700) && (Out1[2] < 0) )
        {
          temp2 = 0;
        }

        if( Out1[2] > 0 )
        {
          temp2 = 3;
        }
      }

      if( temp2 == 3 )
      {
        if( Out1[2] < 0 )
        {
          temp2 = 0;
        }
      }




      if(  (Out1[2] > 1700) && (temp5 == 0) && (temp6 != 1)   )
      {
        temp5 = 1;
        temp6 = 2;
      }

      if( temp5 ==1 )
      {

        if( (Out1[2] < 1700) && (Out1[2] > 0) )
        {
          temp5 = 0;
        }

        if( Out1[2] < 0 )
        {
          temp5 = 3;
        }
      }

      if( temp5 == 3 )
      {
        if( Out1[2] > 0 )
        {
          temp5 = 0;
        }
      }

    }


    if( (temp3 == 2)&&(temp4 < 35) )
    {
      sen_offset.out_offset[0] = Out1[0];
      sen_offset.out_offset[1] = Out1[1];
      sen_offset.out_offset[2] = Out1[2];


      temp4 ++;
    }
    if( temp4 == 35 )
    {
      sen_offset.out_offset[0] = offset[0];
      sen_offset.out_offset[1] = offset[1];
      sen_offset.out_offset[2] = offset[2];

      temp4 = 0;
    }

    Uart_Array[0] = (int)(Out1[0] - sen_offset.out_offset[0]);
    Uart_Array[1] = (int)(Out1[1] - sen_offset.out_offset[1]);
    Uart_Array[2] = (int)(Out1[2] - sen_offset.out_offset[2]);


    if( (temp2 == 3) && (temp5 != 3) )
    {

      Uart_Array[2] = -(3600 - Uart_Array[2]) ;

    }

    if( (temp5 == 3) && (temp2 != 3) )
    {

      Uart_Array[2] = 3600 + Uart_Array[2] ;

    }

    Display_Uart();


    if(time != 1)
    {
      // YAW
      // turn left
      if(Uart_Array[2] <= -150)
      {
        if( (flag != 's') && (flag != 'l') )
        {
          flag = 's';
          temp3 = 0;
        }
        else
        {
          flag = 'l';
        }
      }

      // turn right
      if(Uart_Array[2] >= 150)
      {
        if( (flag != 's') && (flag != 'r') )
        {
          flag = 's';
          temp3 = 0;
        }
        else
        {
          flag = 'r';
        }
      }


      if( (-150<Uart_Array[2])&&(Uart_Array[2]<150) )
      {

        //PITCH
        // Go down
        if(Uart_Array[1] <= -100)
        {
          if( (flag != 's') && (flag != 'd')  )
          {
            flag = 's';
            temp3 = 0;
          }
          else
          {
            flag = 'd';
          }
        }

        // Go up
        if(Uart_Array[1] >= 150)
        {
          if((flag != 's') && (flag != 'u') )
          {
            flag = 's';
            temp3 = 0;
          }
          else
          {
            flag = 'u';
          }
        }

        if( (-100<Uart_Array[1])&&(Uart_Array[1]<150) )
        {

          //ROLL
          // rotate left
          if(Uart_Array[0] <= -150)
          {
            if( (flag != 's') && (flag != 'a')  )
            {
              flag = 's';
              temp3 = 0;
            }
            else
            {
              flag = 'a';
            }
          }

          // rotate right
          if(Uart_Array[0] >= 150)
          {
            if( (flag != 's') && (flag != 'b')  )
            {
              flag = 's';
              temp3 = 0;
            }
            else
            {
              flag = 'b';
            }
          }


          if( (-150<Uart_Array[0])&&(Uart_Array[0]<150) )
          {
            //flag = 's';
          }

        }
      }

      if( flag == 'l' )
      {

        K = 60;//-Uart_Temp[0]/10;
        v[0] = 0.667*K;
        v[1] = (K/3)+128;
        v[2] = (K/3)+128;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 'r' )
      {

        K = 60;// Uart_Temp[1]/10;
        v[0] = (0.667*K)+128;
        v[1] = K/3;
        v[2] = K/3;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 'u' )
      {

        K = 60;//Uart_Temp[2]/10;
        v[0] = 0;
        v[1] = K*0.577;
        v[2] = (K*0.577)+128;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 'd' )
      {

        K = 60;//-Uart_Temp[3]/10;
        v[0] = 0;
        v[1] = (K*0.577)+128;
        v[2] = K*0.577;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 'a' )
      {
        v[0] = 50;
        v[1] = 50;
        v[2] = 50;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 'b' )
      {
        v[0] = 178;
        v[1] = 178;
        v[2] = 178;
        Uart_Send_0[0] = (char)v[0];
        Uart_Send_0[1] = (char)v[1];
        Uart_Send_0[2] = (char)v[2];
        Uart_Send_0[3] = 122;
      }

      if( flag == 's' )
      {
        Uart_Send_0[0] = 0;
        Uart_Send_0[1] = 0;
        Uart_Send_0[2] = 0;
        Uart_Send_0[3] = 122;
      }

      if( (flag == 's') && (temp3 == 0) )
      {
         temp3 = 2;
      }

      DMA_Re_Enable_0();

    }
    else
    {
      DMA_Re_Enable_0();
    }
    DMA_Re_Enable_1();
    Timer_CyRun();

  }
}
