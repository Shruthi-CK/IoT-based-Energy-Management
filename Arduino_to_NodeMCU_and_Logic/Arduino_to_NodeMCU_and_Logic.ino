#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "MicrogridHelper.h" 

// Change these pins to ones that are not used for hardware serial
SoftwareSerial s(10, 11); // RX, TX

// Sensor pin definitions
// MG is short for microgrid

int sensor1 = A0; //Input pin for Critical load of MG 1
int sensor2 = A1; //Input pin for Non-Critical load of MG 1
int sensor3 = A2; //Input pin for Solar of MG 1
int sensor4 = A3; //Input pin for Wind of MG 1
int sensor5 = A4; //Input pin for Battery of MG 1

int sensor6 = A5; //Input pin for Critical load of MG 2
int sensor7 = A6; //Input pin for Non-Critical load of MG 2
int sensor8 = A7; //Input pin for Solar of MG 2
int sensor9 = A8; //Input pin for Wind of MG 2
int sensor10 = A9; //Input pin for Battery of MG 2

int sensor11 = A10; //Input pin for Critical load of MG 3
int sensor12 = A11; //Input pin for Non-Critical load of MG 3
int sensor13 = A12; //Input pin for Solar of MG 3
int sensor14 = A13; //Input pin for Wind of MG 3
int sensor15 = A14; //Input pin for Battery of MG 3

void setup() { 
  Serial.begin(9600); // Enable hardware serial for debugging
  s.begin(9600);      // Start software serial
  
  pinMode(2, OUTPUT); // Green LED light for MG 1
  pinMode(3, OUTPUT); // White LED light for MG 1
  pinMode(4, OUTPUT); // Red LED light for MG 1
  
  pinMode(5, OUTPUT); // Green LED light for MG 2
  pinMode(6, OUTPUT); // White LED light for MG 2
  pinMode(7, OUTPUT); // Red LED light for MG 2
  
  pinMode(8, OUTPUT); // Green LED light for MG 3
  pinMode(9, OUTPUT); // White LED light for MG 3
  pinMode(10, OUTPUT); // Red LED light for MG 3
}

void loop() {
  // Read potentiometer values
  
  float load1m1 = analogRead(sensor1)*3000/1024; // Critical load for MG 1(range 0-3000kWh)
  float load2m1 = analogRead(sensor2)*3000/1024; // Non-Critical load for MG 1 (range 0-3000kWh)
  float s1 = analogRead(sensor3)*2000/1024; // Solar generation for MG 1 (range 0-2000kWh)
  float w1 = analogRead(sensor4)*2000/1024; // Wind generation for MG 1 (range 0-2000kWh)
  float bt1 = analogRead(sensor5)*2000/1024; // Battery power for MG 1 (range 0-2000kWh)
  
  float load1m2 = analogRead(sensor1)*3000/1024; // Critical load for MG 2 (range 0-3000kWh)
  float load2m2 = analogRead(sensor2)*3000/1024; // Non-Critical load for MG 2 (range 0-3000kWh)
  float s2 = analogRead(sensor3)*2000/1024; // Solar generation for MG 2 (range 0-2000kWh)
  float w2 = analogRead(sensor4)*2000/1024; // Wind generation for MG 2 (range 0-2000kWh)
  float bt2 = analogRead(sensor5)*2000/1024; // Battery power for MG 2 (range 0-2000kWh)
  
  float load1m3 = analogRead(sensor1)*3000/1024; // Critical load for MG 3 (range 0-3000kWh)
  float load2m3 = analogRead(sensor2)*3000/1024; // Non-Critical load for MG 3 (range 0-3000kWh)
  float s3 = analogRead(sensor3)*2000/1024; // Solar generation for MG 3 (range 0-2000kWh)
  float w3 = analogRead(sensor4)*2000/1024; // Wind generation for MG 3 (range 0-2000kWh)
  float bt3 = analogRead(sensor5)*2000/1024; // Battery power for MG 3 (range 0-2000kWh)

    
  // Create JSON object to parse all the potentiometer data to the NodeMCU module 
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  
  object["data1"] = load1m1;
  object["data2"] = load2m1;
  object["data3"] = gen1;
  object["data4"] = grid1;
  object["data5"] = price1;
  
  object["data6"] = load1m2;
  object["data7"] = load2m2;
  object["data8"] = gen2;
  object["data9"] = grid2;
  object["data10"] = price2;
  
  object["data11"] = load1m3;
  object["data12"] = load2m3;
  object["data13"] = grid3;
  object["data14"] = excess3;
  object["data15"] = price3;

  // Send JSON data over software serial
  object.printTo(s);

  // Debugging: print JSON data to hardware serial
  object.printTo(Serial);

  
  // Perform calculations
  
  float load1 = load1m1 + load2m1; // Total load of MG1
  float gen1 = s1 + w1 + bt1; // Total generation of MG1
  float grid1 = gen1-load1; // Excess in MG1
  float load2 = load1m2 + load2m2; // Total load of MG2
  float gen2 = s2 + w2 + bt2; // Total generation of MG2
  float grid2 = gen2-load2; // Excess in MG2
  float load3 = load1m3 + load2m3; // Total load of MG3
  float gen3 = s3 + w3 + bt3; // Total generation of MG3
  float grid3 = gen3-load3; // Excess in MG3

  //Printing basic details of the microgrids

  Serial.print("Demand of microgrid1 = ");
  Serial.print(load1);
  Serial.print(" kwh");
  Serial.print("\nDemand of microgrid2 = ");
  Serial.print(load2);
  Serial.print(" kwh");
  Serial.print("\nDemand of microgrid3 = ");
  Serial.print(load3);
  Serial.print(" kwh");
  
  Serial.print("\n\nSolar generation of microgrid1 = ");
  Serial.print(s1);
  Serial.print(" kwh");
  Serial.print("\nWind generation of microgrid1 = ");
  Serial.print(w1);
  Serial.print(" kwh");
  Serial.print("\nBattery power of microgrid1 = ");
  Serial.print(bt1);
  Serial.print(" kwh");
  
  Serial.print("\n\nSolar generation of microgrid2 = ");
  Serial.print(s2);
  Serial.print(" kwh");
  Serial.print("\nWind generation of microgrid2 = ");
  Serial.print(w2);
  Serial.print(" kwh");
  Serial.print("\nBattery power of microgrid2 = ");
  Serial.print(bt2);
  Serial.print(" kwh");
  
  Serial.print("\n\nSolar generation of microgrid3 = ");
  Serial.print(s3);
  Serial.print(" kwh");
  Serial.print("\nWind generation of microgrid3 = ");
  Serial.print(w3);
  Serial.print(" kwh");
  Serial.print("\nBattery power of microgrid3 = ");
  Serial.print(bt3);
  Serial.print(" kwh");

  float gen = gen1+gen2+gen3; // Total generation of the microgrid setup
  float load = load1+load2+load3; // Total load of the microgrid setup

  // Assign multiplication factor based on the Peak hour

  float mf=0; // Multiplication factor
  int flag1=0; // To indicate the load level in MG 1 through LED light
  int flag2=0; // To indicate the load level in MG 2 through LED light
  int flag3=0; // To indicate the load level in MG 3 through LED light

  if ((load)< 0.75*(gen))
    mf=1;
  else if ((load)< 1.25*(gen)){
    mf=1.33;
    flag1++;
    flag2++;
    flag3++;
  }
  else {
    mf=2;
    flag1+=2;
    flag2+=2;
    flag3+=2;
  }  


    // LOAD SHARING
 
    float total=grid1+grid2+grid3; 
    float share1=0;
    float share2=0;
    
    // Scenario 1: All MG produce excess energy
    
    if(grid1>0 && grid2>0 && grid3>0) 
    { Serial.print(total);
      Serial.print(" kwh of energy is excess in the grid. This energy can be supplied back to the main grid.\n");
      if(load1<=0.6*gen1)
        price1= grid_ops(load1,s1,ps_low,w1,pw_low,bt1,pbt_low,pgrid_low,0,1);  
      else if(load1<1.2*gen1) 
        { price1= grid_ops(load1,s1,ps_med,w1,pw_med,bt1,pbt_med,pgrid_med,0,1); 
          flag1++;
        }
      else 
        { price1= grid_ops(load1,s1,ps_high,w1,pw_high,bt1,pbt_high,pgrid_high,0,1);
          flag1+=2;
        }
        
      if(load2<=0.6*gen2)
        price2= grid_ops(load2,s2,ps_low,w2,pw_low,bt2,pbt_low,pgrid_low,0,2);
      else if(load2<1.2*gen2) 
        { price2= grid_ops(load2,s2,ps_med,w2,pw_med,bt2,pbt_med,pgrid_med,0,2);
          flag2++;
        }
      else 
        { price2= grid_ops(load2,s2,ps_high,w2,pw_high,bt2,pbt_high,pgrid_high,0,2);
          flag2+=2;
        }
          
      if(load3<=0.6*gen3)
        price3= grid_ops(load3,s3,ps_low,w3,pw_low,bt3,pbt_low,pgrid_low,0,3);
      else if(load3<1.2*gen3) 
        { price3= grid_ops(load3,s3,ps_med,w3,pw_med,bt3,pbt_med,pgrid_med,0,3); 
          flag3++;
        }
      else 
        { price3= grid_ops(load3,s3,ps_high,w3,pw_high,bt3,pbt_high,pgrid_high,0,3);
          flag3+=2;
        }
      Serial.println("\nTotal Price = ");
      Serial.print(price1+price2+price3);
    }

    // Scenario 2: Only one MG produces excess energy
    
    else if (grid1>0 && grid2<0 && grid3<0 || grid1<0 && grid2>0 && grid3<0 || grid1<0 && grid2<0 && grid3>0) 
    { if (grid1>0)
       float totprice = gridcheck1(grid1,grid2,grid3,total,share1,share2,load1,1,2,3,flag1,flag2,flag3);     
      else if (grid2>0)
       float totprice = gridcheck1(grid2,grid1,grid3,total,share1,share2,load2,2,1,3,flag1,flag2,flag3);
      else
       float totprice = gridcheck1(grid3,grid1,grid2,total,share1,share2,load3,3,1,2,flag1,flag2,flag3); 
     }
     
    // Scenario 3: Two MG produces excess energy
    
    else if ( grid1>0 && grid2>0 &&grid3<0 || grid1>0 && grid2<0 && grid3>0 || grid1<0 && grid2>0 && grid3>0 )
    { if (grid3<0)
       float totprice = gridcheck2(grid1,grid2,grid3,total,share1,share2,1,2,3,flag1,flag2,flag3);
      else if (grid2<0)
       float totprice = gridcheck2(grid1,grid3,grid2,total,share1,share2,1,3,2,flag1,flag2,flag3);
      else
       float totprice = gridcheck2(grid2,grid3,grid1,total,share1,share2,2,3,1,flag1,flag2,flag3);  
    }
    
    // Scenario 4: All MGs are energy are energy-deficient  
    else {
      Serial.print("\n");
      Serial.print(-total);
      Serial.print(" kwh of energy is required from main grid. NCL must take place."); 
      if(load1<=0.6*gen1)
        price1= grid_ops(load1,s1,ps_low,w1,pw_low,bt1,pbt_low,pgrid_low,0,1);  
      else if(load1<1.2*gen1) 
        { price1= grid_ops(load1,s1,ps_med,w1,pw_med,bt1,pbt_med,pgrid_med,0,1); 
          flag1++;
        }
      else 
        { price1= grid_ops(load1,s1,ps_high,w1,pw_high,bt1,pbt_high,pgrid_high,0,1);
          flag1+=2;
        }
        
      if(load2<=0.6*gen2)
        price2= grid_ops(load2,s2,ps_low,w2,pw_low,bt2,pbt_low,pgrid_low,0,2);
      else if(load2<1.2*gen2) 
        { price2= grid_ops(load2,s2,ps_med,w2,pw_med,bt2,pbt_med,pgrid_med,0,2);
          flag2++;
        }
      else 
        { price2= grid_ops(load2,s2,ps_high,w2,pw_high,bt2,pbt_high,pgrid_high,0,2);
          flag2+=2;
        }
          
      if(load3<=0.6*gen3)
        price3= grid_ops(load3,s3,ps_low,w3,pw_low,bt3,pbt_low,pgrid_low,0,3);
      else if(load3<1.2*gen3) 
        { price3= grid_ops(load3,s3,ps_med,w3,pw_med,bt3,pbt_med,pgrid_med,0,3); 
          flag3++;
        }
      else 
        { price3= grid_ops(load3,s3,ps_high,w3,pw_high,bt3,pbt_high,pgrid_high,0,3);
          flag3+=2;
        }
      Serial.println("\nTotal Price = ");
      Serial.print(price1+price2+price3);
    }
    
        
    Serial.print("\nFinal total price after considering peak hour factor: Rs.");
    Serial.print(totprice*mf);


  // LED OUTPUT
  
  if(flag1==0)
   { digitalWrite(2,HIGH);
     digitalWrite(3,LOW);
     digitalWrite(4,LOW);
   }
     
  else if (flag1==4)
   { digitalWrite(4,HIGH);
     digitalWrite(3,LOW);
     digitalWrite(2,LOW);
   }
  else if(flag1==1 || flag1==2 || flag1==3)
   { digitalWrite(3,HIGH);
     digitalWrite(2,LOW);
     digitalWrite(4,LOW);
   }
  
  if(flag2==0)
   { digitalWrite(5,HIGH);
     digitalWrite(6,LOW);
     digitalWrite(7,LOW);
   }
  else if (flag2==4)
   { digitalWrite(7,HIGH);
     digitalWrite(5,LOW);
     digitalWrite(6,LOW);
   }
  else if(flag2==1 || flag2==2 || flag2==3)
   { digitalWrite(6,HIGH);
     digitalWrite(5,LOW);
     digitalWrite(7,LOW);
   }
     
  if(flag3==0)
   { digitalWrite(8,HIGH);
     digitalWrite(9,LOW);
     digitalWrite(10,LOW);
   }
  else if (flag3==4)
   { digitalWrite(10,HIGH);
     digitalWrite(8,LOW);
     digitalWrite(9,LOW);
   }
  else if(flag3==1 || flag3==3 || flag3==3)
   { digitalWrite(9,HIGH);
     digitalWrite(8,LOW);
     digitalWrite(10,LOW);
   }
  
  delay(1000); // Adjust delay as needed
}
