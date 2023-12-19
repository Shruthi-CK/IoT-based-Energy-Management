#include "MicrogridHelper.h"

void operations(float &source, float psource, float &load1, float &price, float &bt1, float bt1max, float &grid, char *source1, float share, int x) {
  
  price=price+psource*(load1-share); 
  Serial.print(load1);
  Serial.print(" kWhr energy is supplied by ");
  Serial.print(source1);
  Serial.print(" of the microgrid");
  source=source-load1;
  excess=grid;
  Serial.print("\nExcess generation in microgrid");
  Serial.print(x);
  Serial.print(" = ");
  Serial.print(excess);
  load1=0;
  
}

float grid_ops(float &load1, float &s1, float ps1, float &w1, float pw1, float &bt1, float pbt1, float pg1, float &excess) {
  float grid;
  float price=0;
    
  if(load1<s1){
    grid=s1-load1+w1+bt1;
    operations(s1,ps1,load1,price,bt1,bt1max,grid,"Solar",share,i);
    }
  else{
    price=ps1*s1; 
    load1=load1-s1;
    Serial.print("\n");
    Serial.print(s1);
    Serial.print(" kWhr energy is supplied by solar of microgrid ");
    Serial.print(i);
    Serial.println();
    s1=0;  
    if (load1<w1){
      grid=w1-load1+bt1;
      operations(w1,pw1,load1,price,bt1,bt1max,grid,"Wind",share,i);
      } 
    else {
      price=price+pw1*w1; 
      load1=load1-w1;
      Serial.print(w1);
      Serial.print("kWhr energy is supplied by wind of microgrid ");
      Serial.print(i);
      Serial.println();
      w1=0;
  
      if (load1<bt1){
        grid=bt1-load1;
        operations(bt1,pbt1,load1,price,bt1,bt1max,grid,"Battery",share,i);  
        }
      else {
        price=price+pbt1*bt1; 
        load1=load1-bt1;
        Serial.print(bt1);
        Serial.print("kWhr energy is supplied by battery of microgrid ");
        Serial.print(i);
        bt1=0;
        grid=-load1;
        price=price+pgrid1*(load1-share);
        }
      }
    }
    
    Serial.print("\n\nRemaining Load=");
    Serial.print(load1);
    Serial.print(" ; Solar1 = ");
    Serial.print(s1);
    Serial.print("\nWind = ");
    Serial.print(w1);
    Serial.print(" ; Battery = ");
    Serial.print(bt1);
    Serial.print("\n");
     
    return price;
    
}

void gridcheck1(float &grid1, float &grid2, float &grid3, float& total, float &share1, float &share2, float &load1, float &load2, float &load3, int a, int b, int c, int flag1, int flag2, int flag3) {
    
    // Load1 is having the excess energy 
    // If MG2 has excess energy instead of MG1, the value of load2 is passed in place of load1 to get correct results  
    
    float x;
    if(total>0)
    { x=grid1;
      grid1=(-grid2-grid3);
    }
    share1=(grid2 /( grid2 + grid3 ))* grid1;
    share2=(grid3 /( grid2 + grid3 ))* grid1;
    Serial.println("\n"); 
    Serial.println("\nBEFORE LOAD SHARING: ");
    
    // To take peak hour into consideration for all the MGs
    
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

    //Additional price for the MGs that are having deficient power
    load1=share1+share2;

    if((load1+load1m1+load2m1)<=0.6*gen1)
      price_new= grid_ops(load1,s1,ps_low,w1,pw_low,bt1,pbt_low,pgrid_low,0,1);  
    else if((load1+load1m1+load2m1)<1.2*gen1) 
      price_new= grid_ops(load1,s1,ps_med,w1,pw_med,bt1,pbt_med,pgrid_med,0,1); 
    else 
      price_new= grid_ops(load1,s1,ps_high,w1,pw_high,bt1,pbt_high,pgrid_high,0,1);
    
    Serial.println("\nFOR LOAD SHARING: ");
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
    Serial.print("\nPrice for microgrid 1: Rs.");
    Serial.print(price1);
    Serial.print("   Considering peak hour factor: Rs.");
    price1 = price1*mf
    Serial.print(price1);

    //Pricing is done per the proportion it is taking from MG1
    Serial.print("\nPrice for microgrid 2: Rs.");
    price2 = price2+(price_new)*share1/(share1+share2)
    Serial.print(price2);
    Serial.print("   Considering peak hour factor: Rs.");
    Serial.print(price2*mf);

    //Pricing is done per the proportion it is taking from MG1
    Serial.print("\nPrice for microgrid 3: Rs.");
    price3 = price3+(price_new)*share2/(share1+share2)
    Serial.print(price3);
    Serial.print("   Considering peak hour factor: Rs.");
    Serial.print(price3*mf);
    
    return price1+price2+price3;
}

void gridcheck2(float &grid1, float &grid2, float &grid3, float& total, float &share1, float &share2, int a, int b, int c) {
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
          
          
        // To take peak hour into consideration for all the MGs
    
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

      Serial.println("\n\nPrices for this hour: ");
      Serial.print("\nPrice for microgrid 1: Rs.");
      Serial.print(price1+(price2)*share1/(share1+share2)+(price3)*share2/(share1+share2));
      Serial.print("   Considering peak hour factor: Rs.");
      price1 = price1*mf
      Serial.print(price1);
  
      //Pricing is done per the proportion it is taking from MG2 and MG3
      
      Serial.print("\nPrice for microgrid 2: Rs.");
      Serial.print(price2);
      Serial.print("   Considering peak hour factor: Rs.");
      Serial.print(price2*mf);
  
      //Pricing is done per the proportion it is taking from MG2 and MG3
      
      Serial.print("\nPrice for microgrid 3: Rs.");
      Serial.print(price3);
      Serial.print("   Considering peak hour factor: Rs.");
      Serial.print(price3*mf);
      
      return price1+price2+price3;

}
