#include <ros.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/String.h>
#include <Stepper.h>

const int TARIK = 1600;

boolean INITIAL_FLAG = 1;
const int S = 23000;
Stepper mStepper(S, 22,23,24,25);


int frendurum=0;
int butondurum=0;
int x = 0;
const int stepsPerRevolution = 2000; //FREN

Stepper myStepper(TARIK,22,23,24,25);
Stepper stepper(stepsPerRevolution,50,51,52,53);

int ileri = 3;
int bos;
int geri = 2;



std_msgs::String button_msg;

 //digitalWrite(ileri,HIGH);
  //digitalWrite(geri,HIGH);

void ilerifonk() {

  digitalWrite(ileri,LOW);
  digitalWrite(geri,HIGH);
}

void gerifonk() {
  
  digitalWrite(geri,LOW);
  digitalWrite(ileri,HIGH);
  
}

void bosfonk() {
  digitalWrite(ileri,LOW);
  digitalWrite(geri,LOW);
}




void callbackFn( const sensor_msgs::Joy& joy_msg){

  int ileri_buton = joy_msg.buttons[3];
  int geri_buton = joy_msg.buttons[0];
  int bos_buton = joy_msg.buttons[1];
   
  float sag = joy_msg.axes[0];
  float sol = joy_msg.axes[0];
 
  float xAxis = joy_msg.axes[4];
  
  int butondurum = joy_msg.buttons[2]; //fren
  
   
 
  
  if (xAxis>=0){
    
    xAxis = (175-xAxis*175);
    analogWrite(10,xAxis);
  }
  else {
    analogWrite(10,175);
  }
  
 if(butondurum==1 && x==0){
    x=1;
   stepper.step(-stepsPerRevolution);
   butondurum=0;
      }
  
  if(butondurum==1 && x==1){
    stepper.step(stepsPerRevolution);
    x=0;
    butondurum=0;
    }
  

 if (ileri_buton == 1){
  ilerifonk();
 }
 if (geri_buton == 1){
  gerifonk();
 }
 if (bos_buton == 1){
  bosfonk();
 }

if (sag >0 && sag <= 1){
  sagadonfonk();
 } 
  if (sol <0 && sol >= -1){
  soladonfonk();
 } 
}

ros::NodeHandle nh;
ros::Subscriber<sensor_msgs::Joy> joy("joy", &callbackFn );
ros::Publisher button_publisher("button_press", &button_msg);


void setup() {
Serial.begin(57600);
 pinMode(ileri,OUTPUT);
 pinMode(geri,OUTPUT);
 myStepper.setSpeed(320);
 stepper.setSpeed(175); //fren
 bosfonk();
 nh.advertise(button_publisher);
 nh.initNode();

 pinMode(7,INPUT);

 while(INITIAL_FLAG) {

  mStepper.setSpeed(60);
  mStepper.step(-10);


 if (digitalRead(7)) {


  Serial.println("BASILDI");

  INITIAL_FLAG=0;

  delay(1109);
   }
  }
  
 

  mStepper.step(S);

 
}

long sagadonfonk() {
  
  myStepper.step(-TARIK);
}

long soladonfonk() {
  
 myStepper.step(TARIK);
 
}

void loop() {
  if (digitalRead(9) == HIGH) {
    button_msg.data = "Pressed";
    } else {
    button_msg.data = "NOT pressed";
    }

 
  button_publisher.publish( &button_msg );
  nh.subscribe(joy);
  nh.spinOnce();

}
