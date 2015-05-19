//mechanum main
//uses MPU6000,RCoutput,RCinput, blizzard4


#define CH_1 0
#define CH_2 1
#define CH_3 2
#define CH_4 3
#define CH_5 4
#define CH_6 5
#define CH_7 6
#define CH_8 7

#include <stdarg.h>
#include <AP_Common.h>
#include <AP_Progmem.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <AP_HAL_AVR_SITL.h>
#include <AP_HAL_Empty.h>
#include <AP_Param.h>
#include <AP_Math.h>
#include <RC_Channel.h>
#include <AP_ADC.h>
#include <AP_InertialSensor.h>
#include <AP_Notify.h>
#include <AP_GPS.h>
#include <DataFlash.h>
#include <GCS_MAVLink.h>
#include <AP_Declination.h>

#include <AP_BattMonitor.h>//note needs the modified libary to function
#include <AP_Compass.h> // Compass Library

#ifdef DOES_ARDUINO_NOT_SUPPORT_CUSTOM_INCLUDE_DIRECTORIES
#include <AP_ADC_AnalogSource.h>
#endif

const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;

RC_Channel rc_1(CH_1);
RC_Channel rc_2(CH_2);
RC_Channel rc_3(CH_3);
RC_Channel rc_4(CH_4);
RC_Channel rc_5(CH_5);
RC_Channel rc_6(CH_6);
RC_Channel rc_7(CH_7);
RC_Channel rc_8(CH_8);
RC_Channel *rc = &rc_1;

int twist_x=0;
int twist_y=0;
int twist_z=0;

int Otwist_x=0;
int Otwist_y=0;
int Otwist_z=0;

AP_BattMonitor battery_mon1(1,0);//defult pins
AP_BattMonitor battery_mon2(2,3);//TODO select acual pins to uses for second battery moniter

int safety_count=0;

AP_HAL::DigitalSource *a_led;
AP_HAL::DigitalSource *b_led;

const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;//inisilizing the compass
AP_InertialSensor_MPU6000 ins;

#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
AP_Compass_PX4 compass;
#else
AP_Compass_HMC5843 compass;
#endif
uint32_t timer;

void setup()
{
  setup_radio();
  
  for (int i = 0; i < 30; i++) {
    read_radio();
  }

  hal.rcout->set_freq(0xFF, 490);

  hal.rcout->enable_ch(0);
  hal.rcout->enable_ch(1);

  hal.rcout->write(0, 1500);
  hal.rcout->write(1, 1500);

  //battery monitor
  battery_mon1.init();
  battery_mon1.set_monitoring(AP_BATT_MONITOR_VOLTAGE_AND_CURRENT);
  battery_mon2.init();
  battery_mon2.set_monitoring(AP_BATT_MONITOR_VOLTAGE_AND_CURRENT);
  //LED control
  a_led = hal.gpio->channel(54);//A10 output for LEDs
  a_led->mode(GPIO_OUTPUT);
  a_led->write(0);
  b_led = hal.gpio->channel(53);//A9 output for LEDs
  b_led->mode(GPIO_OUTPUT);
  b_led->write(0);
  
  //set up compas and MPU6000
  setup_compass();
}

void loop()
{
  hal.scheduler->delay(10);
  read_radio();
  talk();
  move_pwm();
  run_compass();
}

void read_radio()
{
  rc_1.set_pwm(hal.rcin->read(CH_1));
  rc_2.set_pwm(hal.rcin->read(CH_2));
  rc_3.set_pwm(hal.rcin->read(CH_3));
  rc_4.set_pwm(hal.rcin->read(CH_4));
  rc_5.set_pwm(hal.rcin->read(CH_5));
  rc_6.set_pwm(hal.rcin->read(CH_6));
  rc_7.set_pwm(hal.rcin->read(CH_7));
  rc_8.set_pwm(hal.rcin->read(CH_8));
}

void move_pwm()
{
  uint16_t wheels[2]; 
  //rotate side forward
  if(rc[2].control_in < 300)//manual control
  {
    //hal.console->printf_P(PSTR("twist"));
    wheels[0]=1500+twist_z-twist_y;//+z*(a+b)
    wheels[1]=1500+twist_z+twist_y;
    a_led->write(0);//LED solid
    b_led->write(1);
  }
  else if(rc[2].control_in < 650)//autonomus
  {
    //hal.console->printf_P(PSTR("radio"));
    wheels[0]=1500+rc[3].control_in+rc[1].control_in;//+z*(a+b)
    wheels[1]=1500+rc[3].control_in-rc[1].control_in;
    a_led->write(1);//LED Blinking
    b_led->write(0);
  }
  else//wireless e-stop
  {
    //hal.console->printf_P(PSTR("stop"));
    wheels[0]=1500;
    wheels[1]=1500;
    a_led->write(0);//LED off
    b_led->write(0);
  }
  
  //TODO: add in function to grab rotation of wheels and time information to calculate velocity 
  // this function will be called by talk
  
  //TODO: Add in compass set-up and read functions (can be "copied") from Mecanum 2
  
  //TODO: Add in LED blinking functionality 


  //checks for battery;
  battery_mon1.read();
  //TODO: Add in check for second battery monitor ** done
  //TODO: (nice to have) add in LED blinking pattern when battery is too low ** sure why not
  //TODO: check that the current is ok? 
  if(battery_mon1.voltage()<9 || battery_mon1.current_amps()>19 || battery_mon2.voltage()<9 || battery_mon2.current_amps()>19)
  {
    safety_count++;
    if(safety_count>10)
    {
      wheels[0]=1500;
      wheels[1]=1500;
      //a_led->write(1);//LED Blinking pattern * needs to be implemented on the led mcu
      //b_led->write(0);
    }
  }
  else if(safety_count>0)
  {
    safety_count--;
  }
  hal.rcout->enable_ch(0);
  hal.rcout->write(0, wheels[0]);
  hal.rcout->enable_ch(1);
  hal.rcout->write(1, wheels[1]);
}

void setup_radio(void)
{	
  rc_1.radio_min = 1050;
  rc_2.radio_min = 1076;
  rc_3.radio_min = 1051;
  rc_4.radio_min = 1055;
  rc_5.radio_min = 1085;
  rc_6.radio_min = 1085;
  rc_7.radio_min = 1085;
  rc_8.radio_min = 1085;
  
  rc_1.radio_max = 1888;
  rc_2.radio_max = 1893;
  rc_3.radio_max = 1883;
  rc_4.radio_max = 1886;
  rc_5.radio_max = 1915;
  rc_6.radio_max = 1915;
  rc_7.radio_max = 1915;
  rc_8.radio_max = 1915;

  // 3 is not trimed
  rc_1.radio_trim = 1472;
  rc_2.radio_trim = 1496;
  rc_3.radio_trim = 1500;
  rc_4.radio_trim = 1471;
  rc_5.radio_trim = 1553;
  rc_6.radio_trim = 1499;
  rc_7.radio_trim = 1498;
  rc_8.radio_trim = 1500;

  rc_1.set_range(-500,500);
  rc_1.set_default_dead_zone(50);
  rc_2.set_range(-500,500);
  rc_2.set_default_dead_zone(50);
  rc_3.set_range(0,1000);
  rc_3.set_default_dead_zone(50);
  rc_4.set_range(-500,500);
  rc_4.set_default_dead_zone(50);

  rc_5.set_range(1000,2000);
  rc_5.set_default_dead_zone(20);
  rc_6.set_range(1000,2000);
  rc_6.set_default_dead_zone(20);
  rc_7.set_range(1000,2000);
  rc_7.set_default_dead_zone(20);
  rc_8.set_range(1000,2000);
  rc_8.set_default_dead_zone(20);

  return;
}

void talk()
{
  //uint8_t Byte[6];
  char Byte[10];
  int Bints[3];
  uint8_t bytes[20];
  unsigned test=0;

  if(hal.console->available() >=10)
  { 
    //hal.console->println("talked");
    while(hal.console->available() >=10)
    {
      Byte[0]= hal.console->read();
      //hal.console->println(Byte[0]);
      if(Byte[0]=='B')// make sure all data begains with a zero
      {
        Byte[4]= hal.console->read();//swaped to try and fix wrong incorect input
        Byte[5]= hal.console->read();
        Byte[6]= hal.console->read();
        Byte[1]= hal.console->read();
        Byte[2]= hal.console->read();
        Byte[3]= hal.console->read();
        Byte[7]= hal.console->read();
        Byte[8]= hal.console->read();
        Byte[9]= hal.console->read();
        hal.console->flush();
        //Bints[0]=100*(int)(Byte[1]-'0');
        Bints[0]=100*(int)(Byte[1]-'0')+10*(int)(Byte[2]-'0')+(int)(Byte[3]-'0');
        //twist_x=Bints[0];
        Bints[1]=100*(int)(Byte[4]-'0')+10*(int)(Byte[5]-'0')+(int)(Byte[6]-'0');
        Bints[2]=100*(int)(Byte[7]-'0')+10*(int)(Byte[8]-'0')+(int)(Byte[9]-'0');
        twist_x=4*Bints[0]-500;
        twist_y=4*Bints[1]-500;
        twist_z=4*Bints[2]-500;

        if(Otwist_x-twist_x>100)// limits rapidthrottle value changes
          twist_x=Otwist_x-100;
        else if(twist_x-Otwist_x>100)
          twist_x=Otwist_x+100;
        
        if(Otwist_y-twist_y>100)
          twist_y=Otwist_y-100;
        else if(twist_y-Otwist_y>100)
          twist_y=Otwist_y+100;
          
        if(Otwist_z-twist_z>100)
          twist_z=Otwist_z-100;
        else if(twist_z-Otwist_z>100)
          twist_z=Otwist_z+100;
          
        Otwist_x=twist_x;
        Otwist_y=twist_y;
        Otwist_z=twist_z;
        //TODO: send compass information to the laptop
        //TODO: send velocity information to the laptop
        
        uint8_t = Comp;//placeholder, compass and odometry need to be implemented first, and SB_driver needs to be modified to read the output as well
        uint8_t = Velo;
        hal.console->printf("%u,%u", (unsigned)Comp,(unsigned)Velo);
      }
    }
  }
}

void setup_compass()
{
  compass.set_offsets(0,0,0); // set offsets to account for surrounding interference
  compass.set_declination(ToRad(0.0)); // set local difference between magnetic north and true north

  hal.scheduler->delay(1000);
  timer = hal.scheduler->micros();

#if CONFIG_HAL_BOARD == HAL_BOARD_APM2
    // we need to stop the barometer from holding the SPI bus
    hal.gpio->pinMode(40, GPIO_OUTPUT);
    hal.gpio->write(40, 1);
#endif

    ins.init(AP_InertialSensor::COLD_START, 
			 AP_InertialSensor::RATE_100HZ);

    if (!compass.init()) {
        hal.console->println("compass initialisation failed!");
        while (1) ;
    }

    compass.set_offsets(0,0,0); // set offsets to account for surrounding interference
    compass.set_declination(ToRad(0.0)); // set local difference between magnetic north and true north
}

void run_compass()//compass function, remove prints and consol reads
{
    Vector3f accel;
    Vector3f gyro;
    float length;
	uint8_t counter = 0;
    static float min[3], max[3], offset[3];

    compass.accumulate();

    // flush any user input
    while( hal.console->available() ) {
        hal.console->read();
    }

    // clear out any existing samples from ins
    ins.update();

    // loop as long as user does not press a key
    while( !hal.console->available() ) {

      

 timer = hal.scheduler->micros();
        compass.read();
        unsigned long read_time = hal.scheduler->micros() - timer;
        float heading;

        if (!compass.healthy()) {
            hal.console->println("not healthy");
            return;
        }
	Matrix3f dcm_matrix;
	// use roll = 0, pitch = 0 for this example
	dcm_matrix.from_euler(0, 0, 0);
        heading = compass.calculate_heading(dcm_matrix);
        compass.null_offsets();

        // capture min
        const Vector3f &mag = compass.get_field();
        if( mag.x < min[0] )
            min[0] = mag.x;
        if( mag.y < min[1] )
            min[1] = mag.y;
        if( mag.z < min[2] )
            min[2] = mag.z;

        // capture max
        if( mag.x > max[0] )
            max[0] = mag.x;
        if( mag.y > max[1] )
            max[1] = mag.y;
        if( mag.z > max[2] )
            max[2] = mag.z;

        // calculate offsets
        offset[0] = -(max[0]+min[0])/2;
        offset[1] = -(max[1]+min[1])/2;
        offset[2] = -(max[2]+min[2])/2;

        // display all to user


        hal.console->println();
    


        // wait until we have a sample
        ins.wait_for_sample(read_time);

        // read samples from ins
        ins.update();
        accel = ins.get_accel();
        gyro = ins.get_gyro();

        length = accel.length();

hal.console->printf_P(PSTR("%.2f\t\t\t\t%u \t\t  %4.2f  %4.2f  %4.2f \t \t %4.2f \t\t\t%4.2f %4.2f %4.2f\n"), 
								ToDeg(heading), (unsigned)read_time, accel.x, accel.y, accel.z, length, gyro.x, gyro.y, gyro.z);


}
}

AP_HAL_MAIN();