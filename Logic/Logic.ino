void setup(){
Serial.begin(9600);

pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);

}

// DYNAMIC PRICING
    int flag1=0;
   int flag2=0;
   int flag3=0;
#define ps_low 2.1
#define pw_low 2.8
#define pbt_low 3.5
#define pgrid_low 5.4

#define ps_med 2.5
#define pw_med 3.1
#define pbt_med 4.2
#define pgrid_med 4.5

#define ps_high 3.2
#define pw_high 3.9
#define pbt_high 5.2
#define pgrid_high 5.8

float load1m1 = 1.00;
float load2m1 = 0.100;
float load1 = load1m1+load2m1;
float s1 = 1.00;
float w1 = 0.300;
float bt1 = 0.200;
float gen1= s1+w1+bt1;
float bt1max=0.300;

float load1m2 = 1.00;
float load2m2 = 2.00;
float load2 = load1m2+load2m2;
float s2 = 0.8;
float w2 = 0.60;
float bt2 = 0.200;
float gen2= s2+w2+bt2;
float bt2max=0.300;

float load1m3 = 1.80;
float load2m3 = 0.70;
float load3 = load1m3+load2m3;
float s3 = 0.90;
float w3 = 0.70;
float bt3 = 0.30;
float gen3= s3+w3+bt3;
float bt3max=0.400;

float price1;
float price2;
float price3;

float grid1=gen1-load1;
float grid2=gen2-load2;
float grid3=gen3-load3;

float mf;
float source;
float excess;
//float totexcess=0;
//int flag;
float price_new;
//float totprice=0;

void operations(float &source, float psource,float &load1, float &price, float &bt1,float bt1max, float &grid,char *source1, float share,int x)
{ 
price=price+psource*(load1-share);
Serial.print(load1);
Serial.print(" kWhr energy is supplied by ");
Serial.print(source1);
Serial.print(" of the microgrid");
source=source-load1;
excess=grid;
//totexcess=totexcess+excess;
Serial.print("\nExcess generation in microgrid");
Serial.print(x);
Serial.print(" = ");
Serial.print(excess);
load1=0;
}

 
 
 
float grid_ops(float &load1,float &s1,float ps1,float &w1,float pw1,float &bt1,float pbt1,float pgrid1,float share,int i)
{ float grid;

  float price=0;
  
if(load1<s1)
{ grid=s1-load1+w1+bt1;
  //price=ps1*load1; 
  operations(s1,ps1,load1,price,bt1,bt1max,grid,"Solar",share,i);
}
else 
{ price=ps1*s1; 
load1=load1-s1;
Serial.print("\n");
Serial.print(s1);
Serial.print(" kWhr energy is supplied by solar of microgrid ");
Serial.print(i);
Serial.println();
s1=0;  
if (load1<w1)
{ grid=w1-load1+bt1;
  //price=price+pw1*w1;
  operations(w1,pw1,load1,price,bt1,bt1max,grid,"Wind",share,i);
 } 
else 
{ price=price+pw1*w1; 
load1=load1-w1;
Serial.print(w1);
Serial.print("kWhr energy is supplied by wind of microgrid ");
Serial.print(i);
Serial.println();
w1=0;

if (load1<bt1)
{ grid=bt1-load1;
  //price=price+pbt1*load1;
  operations(bt1,pbt1,load1,price,bt1,bt1max,grid,"Battery",share,i);  
}
else 
{ price=price+pbt1*bt1; 
load1=load1-bt1;
Serial.print(bt1);
Serial.print("kWhr energy is supplied by battery of microgrid ");
Serial.print(i);
bt1=0;
grid=-load1;
//return grid;
price=price+pgrid1*(load1-share);}
//grid1=grid1-load1;
//load1=0;

}
}
//totprice=totprice+price;
/*if (i=1)
{float price1 = price;
 Serial.println("Price1 = ");
 Serial.print(price1);
}
else if(i=2)
{float price2 = price;
 Serial.println("Price2 = ");
 Serial.print(price2);
}
else if(i=3)
{float price3 = price;
 Serial.println("Price3 = ");
 Serial.print(price3);
}*/

Serial.print("\n\nRemaining Load=");
Serial.print(load1);
Serial.print(" ; Solar1 = ");
Serial.print(s1);
Serial.print("\nWind = ");
Serial.print(w1);
Serial.print(" ; Battery = ");
Serial.print(bt1);
//Serial.print("\nGrid=");
//Serial.print(grid);
//Serial.print("\nPrice= Rs.");
//Serial.print(price);
Serial.print("\n");
 
return price;
}  


void gridcheck1(float &grid1, float &grid2, float &grid3,float& total, float &share1, float &share2,float &load1,int a,int b,int c)
{ 
    float x;
      if(total>0)
       { x=grid1;
         grid1=(-grid2-grid3);
       }
    share1=(grid2 /( grid2 + grid3 ))* grid1;
    share2=(grid3 /( grid2 + grid3 ))* grid1;
   Serial.println("\n"); 
   Serial.println("\nBEFORE LOAD SHARING: ");

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
  price2= grid_ops(load2,s2,ps_low,w2,pw_low,bt2,pbt_low,pgrid_low,share1,2);
else if(load2<1.2*gen2) 
  { price2= grid_ops(load2,s2,ps_med,w2,pw_med,bt2,pbt_med,pgrid_med,share1,2);
    flag2++;
  }
else 
  { price2= grid_ops(load2,s2,ps_high,w2,pw_high,bt2,pbt_high,pgrid_high,share1,2);
    flag2+=2;
  }
    
if(load3<=0.6*gen3)
  price3= grid_ops(load3,s3,ps_low,w3,pw_low,bt3,pbt_low,pgrid_low,share2,3);
else if(load3<1.2*gen3) 
  { price3= grid_ops(load3,s3,ps_med,w3,pw_med,bt3,pbt_med,pgrid_med,share2,3); 
    flag3++;
  }
else 
  { price3= grid_ops(load3,s3,ps_high,w3,pw_high,bt3,pbt_high,pgrid_high,share2,3);
    flag3+=2;
  }
    load1=share1+share2;

Serial.println("\nFOR LOAD SHARING: ");


if((load1+load1m1+load2m1)<=0.6*gen1)
  price_new= grid_ops(load1,s1,ps_low,w1,pw_low,bt1,pbt_low,pgrid_low,0,1);  
else if((load1+load1m1+load2m1)<1.2*gen1) 
  price_new= grid_ops(load1,s1,ps_med,w1,pw_med,bt1,pbt_med,pgrid_med,0,1); 
else 
  price_new= grid_ops(load1,s1,ps_high,w1,pw_high,bt1,pbt_high,pgrid_high,0,1);

//float totprice= price1+price2+price3;
    Serial.println("\n");
    Serial.print(share1);
    Serial.print(" kwh of energy is transferred from grid");
    Serial.print(a);
    Serial.print(" to grid");
    Serial.print(b);
    Serial.print("\n");
    Serial.print(share2);
    Serial.print(" kwh of energy is transferred from grid");
    Serial.print(a);
    Serial.print(" to grid");
    Serial.print(c);
    Serial.print("\n");
    if(total>0)
     { Serial.print(total);
       Serial.print(" kwh of energy is excess in the grid after load sharing\n");
       x=x-share1-share2;
       grid1=x;
     }
    else
    { Serial.print(-total);
      Serial.print(" kwh of energy is required after performing load sharing\n");
      Serial.print("Hence Non Critical Load Shedding can be performed here\n");
      grid1=0;
    }
    grid2=grid2+share1;
    grid3=grid3+share2;
      Serial.print("\nFinal value of energy in grid");
      Serial.print(a);
      Serial.print(" = ");
      Serial.print(grid1);
      Serial.print(" kwh");
      Serial.print("\nFinal value of energy in grid");
      Serial.print(b);
      Serial.print(" = ");
      Serial.print(-grid2);
      Serial.print(" kwh (deficient)");
      Serial.print("\nFinal value of energy in grid");
      Serial.print(c);
      Serial.print(" = ");
      Serial.print(-grid3);
      Serial.print(" kwh (deficient)");
      Serial.println("\n\nPrices for this hour: ");
      //Serial.println("\nPrice of each microgrid: ");
      Serial.print("\nPrice for microgrid 1: Rs.");
      Serial.print(price1);
      Serial.print("   Considering peak hour factor: Rs.");
      Serial.print(price1*mf);
      //Serial.println(price_new);
      //Serial.println(price2);
      //Serial.println(price3);
      Serial.print("\nPrice for microgrid 2: Rs.");
      Serial.print(price2+(price_new)*share1/(share1+share2));
      Serial.print("   Considering peak hour factor: Rs.");
      Serial.print((price2+(price_new)*share1/(share1+share2))*mf);
      Serial.print("\nPrice for microgrid 3: Rs.");
      Serial.print(price3+(price_new)*share2/(share1+share2));
      Serial.print("   Considering peak hour factor: Rs.");
      Serial.print((price3+(price_new)*share2/(share1+share2))*mf);
      //Serial.print("\nTotal Price: ");
      //Serial.print(price_new+price1+price2+price3);
    }
  

void gridcheck2(float &grid1, float &grid2, float &grid3,float& total, float &share1, float &share2,int a,int b,int c)
{

 Serial.print("Excess energy from grids ");
 Serial.print(a);
 Serial.print(" and ");
 Serial.print(b);
 Serial.print(" is ");
 Serial.print(total-grid3);
 Serial.print(" kwh \n");
  if(total>0)
  { share1=-grid3*(grid1/(grid1+grid2));
    share2=-grid3*(grid2/(grid1+grid2)); 
   }
  else {share1=grid1;share2=grid2;}
    Serial.print(share1);
    Serial.print(" kwh of energy from grid");
    Serial.print(a);
    Serial.print(" and ");
    Serial.print(share2);
    Serial.print(" kwh of energy from grid");
    Serial.print(b);
    Serial.print(" is tranferred to grid");
    Serial.print(c);
    if (total>0)
    { Serial.print("\nExcess energy in the system after load sharing = ");
      Serial.print(total);
      Serial.print(" kwh. \nThis energy can be given back to the main grid.");
    }
    else
    { Serial.print("\n");
      Serial.print(-total);
      Serial.print(" kwh of energy is still required ,do ncl shedding\n");
     }
      grid1=grid1-share1;
      grid2=grid2-share2;
      grid3=grid3+share1+share2;
      Serial.print("\nGrid");
      Serial.print(a);
      Serial.print(" = ");
      Serial.print(grid1);
      Serial.print("\nGrid");
      Serial.print(b);
      Serial.print(" = ");
      Serial.print(grid2);
      Serial.print("\nGrid");
      Serial.print(c);
      Serial.print(" = ");
      Serial.print(grid3);

}


void loop()
{ 
Serial.print("Demand of microgrid1 = ");
Serial.print(load1);
Serial.print(" kwh");
Serial.print("\nDemand of microgrid2 = ");
Serial.print(load2);
Serial.print(" kwh");
Serial.print("\nDemand of microgrid3 = ");
Serial.print(load3);
Serial.print(" kwh");

Serial.print("\n\nSolar1 generation of microgrid1 = ");
Serial.print(s1);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.3 per unit)");
Serial.print("\nWind generation of microgrid1 = ");
Serial.print(w1);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.4 per unit)");
Serial.print("\nBattery power of microgrid1 = ");
Serial.print(bt1);
Serial.print(" kwh");

Serial.print("\n\nSolar1 generation of microgrid2 = ");
Serial.print(s2);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.3 per unit)");
Serial.print("\nWind generation of microgrid2 = ");
Serial.print(w2);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.4 per unit)");
Serial.print("\nBattery power of microgrid2 = ");
Serial.print(bt2);
Serial.print(" kwh");

Serial.print("\n\nSolar1 generation of microgrid3 = ");
Serial.print(s3);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.3 per unit)");
Serial.print("\nWind generation of microgrid3 = ");
Serial.print(w3);
Serial.print(" kwh");
//Serial.print("(Cost: Rs.4 per unit)");
Serial.print("\nBattery power of microgrid3 = ");
Serial.print(bt3);
Serial.print(" kwh");

//float price=0;


if ((load1+load2+load3)< 0.75*(gen1+gen2+gen3))
  mf=1;
else if ((load1+load2+load3)< 1.25*(gen1+gen2+gen3))    
  { mf=1.33;
    flag1++;
    flag2++;
    flag3++;
  }
else 
  { mf=2;
    flag1+=2;
    flag2+=2;
    flag3+=2;
  }  
  
//float tot_price=price1+price2+price3;

// LOAD SHARING
 
float total=grid1+grid2+grid3; 
float share1=0;
float share2=0;


if(grid1>0 && grid2>0 && grid3>0)
{ //Serial.print(totexcess);
  Serial.print(" kwh of energy is excess in the grid. This energy can be supplied back to the main grid.\n");
  Serial.println("\nTotal Price = ");
 // Serial.print(totprice);
}
else if (grid1>0 && grid2<0 && grid3<0 || grid1<0 && grid2>0 && grid3<0 || grid1<0 && grid2<0 && grid3>0)
{ if (grid1>0)
   gridcheck1(grid1,grid2,grid3,total,share1,share2,load1,1,2,3);     
  else if (grid2>0)
   gridcheck1(grid2,grid1,grid3,total,share1,share2,load2,2,1,3);
  else
   gridcheck1(grid3,grid1,grid2,total,share1,share2,load3,3,1,2); 
 }

else if ( grid1>0 && grid2>0 &&grid3<0 || grid1>0 && grid2<0 && grid3>0 || grid1<0 && grid2>0 && grid3>0 )
{ if (grid3<0)
   gridcheck2(grid1,grid2,grid3,total,share1,share2,1,2,3);
  else if (grid2<0)
   gridcheck2(grid1,grid3,grid2,total,share1,share2,1,3,2);
  else
   gridcheck2(grid2,grid3,grid1,total,share1,share2,2,3,1);  
}

else
{Serial.print("\n");
Serial.print(-total);
Serial.print(" kwh of energy is required from main grid. NCL must take place."); }


Serial.print("\nFinal total price after considering peak hour factor: Rs.");
Serial.print((price_new+price1+price2+price3)*mf);

 digitalWrite(9, HIGH);

//LED OUTPUT

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

delay(30000);
}
