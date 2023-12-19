#ifndef MicrogridHelper_h
#define MicrogridHelper_h

// Constants for price calculation
  #define ps_low 2.1 // Price per kWh of solar energy when consumption is low (less than 60% of microgrid generation)
  #define pw_low 2.8 // Price per kWh of wind energy when consumption is low (less than 60% of microgrid generation)
  #define pbt_low 3.5 // Price per kWh of battery energy when consumption is low (less than 60% of microgrid generation)
  #define pgrid_low 5.4 // Not applicable since consumption is less than generation
  
  #define ps_med 2.5 // Price per kWh of solar energy when consumption is medium (60-120% of microgrid generation)
  #define pw_med 3.1 // Price per kWh of wind energy when consumption is medium (60-120% of microgrid generation)
  #define pbt_med 4.2 // Price per kWh of battery energy when consumption is medium (60-120% of microgrid generation)
  #define pgrid_med 4.5 // Price per kWh of Electricity Board supply when consumption is medium (60-120% of microgrid generation)
  
  #define ps_high 3.2 // Price per kWh of solar energy when consumption is high (greater than 120% of microgrid generation)
  #define pw_high 3.9 // Price per kWh of wind energy when consumption is high (greater than 120% of microgrid generation)
  #define pbt_high 5.2 // Price per kWh of battery energy when consumption is high (greater than 120% of microgrid generation)
  #define pgrid_high 5.8 //Price per kWh of Electricity Board supply when consumption is high (greater than 120% of microgrid generation)

// Function declarations

  void operations(float &source, float psource, float &load1, float &price, float &bt1, float bt1max, float &grid, char *source1, float share, int x);
  float grid_ops(float &load1, float &s1, float ps1, float &w1, float pw1, float &bt1, float pbt1, float pg1, float &excess);
  void gridcheck1(float &grid1, float &grid2, float &grid3, float& total, float &share1, float &share2, float &load1, int a, int b, int c);
  void gridcheck2(float &grid1, float &grid2, float &grid3, float& total, float &share1, float &share2, int a, int b, int c);

#endif
