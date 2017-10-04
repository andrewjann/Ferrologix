//Operating under half-step (MS1: H, MS2: L, MS3: L)
//half-step: 0.45deg/pulse, 800 pulses/sec
//full-step: 0.90deg/pulse, 400 pulses/sec
//3:1 gear ratio
//1 Hz: 3750 us between pulses

//motor ratchet variables
int motor_direction = 7;
int motor_step = 5;
int MS1 = 1;
int MS2 = 2;
int MS3 = 3;
int gearRatio = 1;
unsigned long time = micros();
unsigned long ref_time = 0;
unsigned long current_time = time - ref_time;
unsigned long previous_time = 0;
float target_freq = 10;  // Adjust this value CM: change from 5 to 2
float initial_freq = 1;
float initial_pulse = gearRatio/(400*initial_freq); //400 accounts for 400 steps/rotation and 3 accounts for 3:1 gear ratio
float initial_pulseinMicros = initial_pulse*1000000;
float ramp_rate = 5;  //ramp rate is change between target and initial freq over total time to ramp
float prev_pulse=initial_pulse;
unsigned long dt = 0;
float rdt = ramp_rate*dt;
float current_pulse = initial_pulse/(1+(initial_pulse*rdt));
float current_pulseinMicros = current_pulse*1000000;
float current_freq = 1/current_pulse;
float ref_freq;

//state-switch motor variables
int ssmotor_direction = 9;
int ssmotor_step = 8;
int limit_switch_collect = 10;
int limit_switch_ratchet = 11;
unsigned long ssref_time = 0;
unsigned long sscurrent_time = time - ref_time;
unsigned long ssprevious_time = 0;
float sstarget_freq = 5;  // Adjust this value
float ssinitial_freq = 1;
float ssinitial_pulse = gearRatio/(800*initial_freq);
float ssinitial_pulseinMicros = initial_pulse*1000000;
float ssramp_rate = 200;  //ramp rate is change between target and initial freq over total time to ramp
float ssprev_pulse=initial_pulse;
unsigned long ssdt = 0;
float ssrdt = ramp_rate*dt;
float sscurrent_pulse = initial_pulse/(1+(initial_pulse*rdt));
float sscurrent_pulseinMicros = current_pulse*1000000;
float sscurrent_freq = 1/current_pulse;
float ssref_freq;

//pump variables 
int pump_left[] = {12, 13};
int i;
long puprevious_time = 0;
unsigned long puref_time = 0;
long pump_current_time = time - ref_time;
const long on_interval = 10000;
const long off_interval = 490000;
int j = 1;

//GUI variables
int mydata;
int input;
int pump_input;

void setup() {                
  // initialize the digital pin as an output.
  pinMode(motor_direction,OUTPUT);  //motor ratchet direction
  pinMode(motor_step,OUTPUT);  //motor ratchet step
  pinMode(MS1,OUTPUT);  //microsteppers 1,2,3
  pinMode(MS2,OUTPUT);
  pinMode(MS3,OUTPUT);
  pinMode(limit_switch_collect,INPUT);
  pinMode(limit_switch_ratchet,INPUT);

  for(i = 0; i < 2; i++){
  pinMode(pump_left[i], OUTPUT);
  }
  
  pinMode(ssmotor_direction,OUTPUT); //switch state direction
  pinMode(ssmotor_step,OUTPUT);  //switch state step
  
  digitalWrite(MS1,LOW); //operating at full-steps (L,L,L)
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,LOW);
  
  digitalWrite(motor_direction,HIGH);
  
  Serial.begin(9600);
  Serial.setTimeout(100);
}


// the loop routine runs over and over again forever:

void loop() {
  RAMP();
}
//void loop() {
//
//  if (Serial.available()>0)  {  // when data is received through the Serial port, read data
//    mydata = Serial.parseInt();  // reads integers until a non-integer, non-negative sign
//    if(mydata < 300 && mydata > 0)  {  //assigns any number between 0 and 300 to ratcheting code (serial port resets to 0 when no code given)
//      input = mydata; 
//    }
////      Serial.print(input);
//
//    else if (mydata >= 300) {
//      pump_input = mydata; //assigns number >300 to pump code
//    }
//  }
//  
//  //Pump controls 
//  
//  if(pump_input == 300)  {
//    pump_stop(); //pump stops
//  }
//  
//  if(pump_input == 301)  {
//    pump_start();  //pump starts
//  }
// 
//  if(pump_input == 302)  {
//    pump_prime();  //pump reverses to prime
//  }
//  
//  if (input == 101)  {
//    //if mydata receives 101, motor will ramp
//    RAMP();
//  }
//  
//  if (input == 100)  {
//    //if mydata receives 100, motor will reset variables to initial conditions and stop running
//    reset_var();
//    reset_ssvar();
//  }
//  
//  else if(input == 201)  {
//    //if mydata receives a '201', motor rotates clockwise then continues ramp
//    digitalWrite(motor_direction,LOW);
//    reset_var();
//  }
//  else if(input == 200)  {
//    //if mydata receives a '200', motor rotates counter-clockwise then continues ramp
//    digitalWrite(motor_direction,HIGH);
//    reset_var();
//  }
//  else if(input >= 1 && input <= 15)  {
//    //if mydata receives a number between 1-15, adjust target freq accordingly
//    change_freq();
//    reset_var();
//  }
//}

void RAMP()  {  // motor ramp function
    time = micros();
    current_time = time - ref_time;
    if (current_time >= previous_time + current_pulseinMicros)  {
      digitalWrite(5,HIGH);
      delayMicroseconds(50);
      digitalWrite(5,LOW);
      previous_time = current_time;
    }
    if ((initial_freq+rdt) >= target_freq)  {  // If current frequency greater than or equal to target frequency, just set to target frequency
      //potential change: abs(target_freq - current_freq) < 1
      current_freq = target_freq;
      current_pulse = gearRatio/(400*current_freq);
      current_pulseinMicros = current_pulse*1000000;
      }
    else {
      dt = current_time/10000;
      rdt = ramp_rate*dt/100;
      current_freq = initial_freq + rdt;
      current_pulse = gearRatio/(400*current_freq);
      current_pulseinMicros = current_pulse*1000000;
     }
}

void state_switch()  {  // motor ramp for state-switching
    time = micros();
    sscurrent_time = time - ssref_time;
    if (sscurrent_time >= ssprevious_time + sscurrent_pulseinMicros)  {
      digitalWrite(ssmotor_step,HIGH);
      delayMicroseconds(50);
      digitalWrite(ssmotor_step,LOW);
      ssprevious_time = sscurrent_time;
    }
    if ((ssinitial_freq+ssrdt) >= sstarget_freq)  {  // If current frequency greater than or equal to target frequency, just set to target frequency
      //potential change: abs(target_freq - current_freq) < 1
      sscurrent_freq = sstarget_freq;
      sscurrent_pulse = gearRatio/(800*sscurrent_freq);
      sscurrent_pulseinMicros = sscurrent_pulse*1000000;
      }
    else {
      ssdt = sscurrent_time/10000;
      ssrdt = ssramp_rate*ssdt/100;
      sscurrent_freq = ssinitial_freq + ssrdt;
      sscurrent_pulse = gearRatio/(800*sscurrent_freq);
      sscurrent_pulseinMicros = sscurrent_pulse*1000000;
     }
}

void moveTo_collect_state()  {  // move to collect position
  digitalWrite(ssmotor_direction,LOW);
  if(digitalRead(limit_switch_collect)==HIGH)  {
    state_switch();
  }
}

void moveTo_ratchet_state()  {  // move to ratchet position
  digitalWrite(ssmotor_direction,HIGH);
  if(digitalRead(limit_switch_ratchet)==HIGH)  {
    state_switch();
  }
}
void change_freq()  {  // adjust frequency of ratcheting
  target_freq = input;
}

void reset_var()  {  // reset variables for ratchet
  time = micros();
  ref_time = time;
  current_time = time - ref_time;
  previous_time = 0;
  initial_freq = 1;
  initial_pulse = gearRatio/(400*initial_freq);
  initial_pulseinMicros = initial_pulse*1000000;
  ramp_rate = 5;
  prev_pulse=initial_pulse;
  dt = 0;
  rdt = ramp_rate*dt;
  current_pulse = initial_pulse/(1+(initial_pulse*rdt));
  current_pulseinMicros = current_pulse*1000000;
  current_freq = 1/current_pulse;
}

void reset_ssvar()  { // reset variables for state-switching
  time = micros();
  ssref_time = 0;
  sscurrent_time = time - ssref_time;
  ssprevious_time = 0;
  ssinitial_freq = 1;
  ssinitial_pulse = gearRatio/(800*ssinitial_freq);
  ssinitial_pulseinMicros = ssinitial_pulse*1000000;
  ssramp_rate = 50;  //ramp rate is change between target and initial freq over total time to ramp
  ssprev_pulse=ssinitial_pulse;
  ssdt = 0;
  ssrdt = ssramp_rate*ssdt;
  sscurrent_pulse = ssinitial_pulse/(1+(ssinitial_pulse*ssrdt));
  sscurrent_pulseinMicros = sscurrent_pulse*1000000;
  sscurrent_freq = 1/sscurrent_pulse;
}

void pump_stop(){
  digitalWrite(pump_left[0], LOW); 
  digitalWrite(pump_left[1], LOW); 
  ref_time=0;
}

void pump_start(){
  time = micros();
//  ref_time = time;
    pump_current_time = time - puref_time; // resets counter to 0 (could potentially reset ratchet counter too. need to test to make sure. Might need to make ref_time and current_time pump specific a different variable)
    if (pump_current_time - puprevious_time >= 0 && pump_current_time - puprevious_time < on_interval) { // counter will start at 0 and increase until 1 second
      digitalWrite(pump_left[0], HIGH); 
      digitalWrite(pump_left[1], LOW);
    }
    if (pump_current_time - puprevious_time >= on_interval) { // at 1 second the following will occur:
      puprevious_time = pump_current_time + off_interval; // Previous time will set to current time plus 30 sec. This will make current_time - puprevious time negative 
      digitalWrite(pump_left[0], LOW); 
      digitalWrite(pump_left[1], LOW);      
    }

    
}
   

void pump_prime(){
  digitalWrite(pump_left[0], HIGH);
  digitalWrite(pump_left[1], LOW);
}



