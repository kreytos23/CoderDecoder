#define D1 4
#define D2 3
#define D3 2

#define C1 12
#define C2 11
#define C3 10
#define C4 9
#define C5 8
#define C6 7
#define C7 6

#define E1 14
#define E2 15
#define E3 16
#define E4 17
#define E5 18
#define E6 19
#define E7 20

#define R1 22
#define R2 24
#define R3 26
#define R4 28
#define R5 30
#define R6 32
#define R7 34 

#define S1 31
#define S2 33
#define S3 35
#define S4 37

#define DC1 40
#define DC2 41
#define DC3 42
#define DC4 43
#define DC5 44
#define DC6 45
#define DC7 46

#define Config A0

#define Tx1 50
#define Rx1 51

#define Tx2 52
#define Rx2 53

int d1, d2, d3, d4;
int c[8]={0,0,0,0,0,0,0,0};
int e[8]={0,0,0,0,0,0,0,0};
int r[8]={0,0,0,0,0,0,0,0};
int rr[8]={0,0,0,0,0,0,0,0};
int s[4]={0,0,0,0};
int dc[8]={0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(9600); 
 
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);  
  pinMode(D3, INPUT);

  pinMode(E1, INPUT);
  pinMode(E2, INPUT);  
  pinMode(E3, INPUT);
  pinMode(E4, INPUT);
  pinMode(E5, INPUT);
  pinMode(E6, INPUT);  
  pinMode(E7, INPUT); 

  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);  
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);  
  pinMode(C7, OUTPUT);  

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);  
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);  
  pinMode(R7, OUTPUT);  

  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);  
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);

  pinMode(DC1, OUTPUT);
  pinMode(DC2, OUTPUT);  
  pinMode(DC3, OUTPUT);
  pinMode(DC4, OUTPUT);
  pinMode(DC5, OUTPUT);
  pinMode(DC6, OUTPUT);  
  pinMode(DC7, OUTPUT);  

  pinMode(Tx1, OUTPUT);
  pinMode(Rx1, INPUT);

  pinMode(Config, INPUT);    

  pinMode(Tx2, OUTPUT);
  pinMode(Rx2, INPUT);
}

void loop() {
  codificador();
 
  if(digitalRead(Config)==1){
    int cr[8]={0,0,0,0,0,0,0,0};
    int contador = 0;
    int contadorLed = R1;
    reiniciar();

    e[0] = digitalRead(E1);
    e[1] = digitalRead(E2);
    e[2] = digitalRead(E3); 
    e[3] = digitalRead(E4);     
    e[4] = digitalRead(E5);
    e[5] = digitalRead(E6); 
    e[6] = digitalRead(E7); 
    
    for(int i=0; i<7; i++){
      digitalWrite(Tx1,c[i]); 
      cr[i]=digitalRead(Rx1);  
      Serial.print(cr[i]);
      delay(1000);

      r[contador] = e[contador] ^ cr[contador];
      digitalWrite(contadorLed, r[contador]);
      
      contador = contador + 1;
      contadorLed = contadorLed + 2;

      delay(1000);
    }
    digitalWrite(Tx1,LOW);

    delay(1500);

    for(int i=0; i<7; i++){
      digitalWrite(Tx2,r[i]);
      rr[i]=digitalRead(Rx2);
      delay(1000);
    }

    digitalWrite(Tx2,LOW);
    calcularSindrome();
    correcionError();
  }  
}

void codificador(){
  d1 = digitalRead(D1);
  d2 = digitalRead(D2);
  d3 = digitalRead(D3);   
 
  c[0] = d1;
  c[1] = d2;
  c[2] = d3;
  c[3] = d1 ^ d2;  
  c[4] = d1 ^ d3;
  c[5] = d2 ^ d3;
  c[6] = d1 ^ d2 ^ d3;

  digitalWrite(C1,c[0]);
  digitalWrite(C2,c[1]);
  digitalWrite(C3,c[2]);
  digitalWrite(C4,c[3]);
  digitalWrite(C5,c[4]);
  digitalWrite(C6,c[5]);
  digitalWrite(C7,c[6]);
}

void calcularSindrome(){
  s[0] = rr[0] ^ rr[1] ^ rr[3]; 
  s[1] = rr[0] ^ rr[2] ^ rr[4]; 
  s[2] = rr[1] ^ rr[2] ^ rr[5]; 
  s[3] = rr[0] ^ rr[1] ^ rr[2] ^ rr[6];

  digitalWrite(S1,s[0]);
  digitalWrite(S2,s[1]);
  digitalWrite(S3,s[2]);
  digitalWrite(S4,s[3]);
}

void correcionError(){
  if( s[0] && s[1] && !s[2] && s[3]){//Detección sindrome: 1101
    dc[0]=rr[0] ^ 1;
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];    
    dc[6]=rr[6];  

   }
  else if(s[0] && !s[1] && s[2] && s[3]){//Detección sindrome: 1011
    dc[0]=rr[0];
    dc[1]=rr[1] ^ 1; 
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];  
    dc[6]=rr[6];   
                
  }
  else if(!s[0] && s[1] && s[2] && s[3]){//Detección sindrome: 0111
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2] ^ 1;
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];   
    dc[6]=rr[6];  
                  
  }
  else if(s[0] && !s[1] && !s[2] && !s[3]){//Detección sindrome: 1000
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3] ^ 1;
    dc[4]=rr[4];
    dc[5]=rr[5];   
    dc[6]=rr[6];  
                  
  }    
  else if(!s[0] && s[1] && !s[2] && !s[3]){//Detección sindrome: 0100 
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4] ^ 1;
    dc[5]=rr[5];  
    dc[6]=rr[6];   
                
  }       
  else if(!s[0] && !s[1] && s[2] && !s[3]){//Detección sindrome: 0010 
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5] ^ 1;
    dc[6]=rr[6]; 
                  
  } 
  else if(!s[0] && !s[1] && !s[2] && s[3]){//Detección sindrome: 0001
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];   
    dc[6]=rr[6] ^ 1;             
  }
  else if(!s[0] && s[1] && s[2] && !s[3]){//Detección sindrome: 0110
    dc[0]=rr[0] ^ 1;
    dc[1]=rr[1] ^ 1;    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];   
    dc[6]=rr[6];             
  }
  else if(s[0] && !s[1] && s[2] && !s[3]){//Detección sindrome: 1010
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3] ^ 1;
    dc[4]=rr[4];
    dc[5]=rr[5] ^ 1;   
    dc[6]=rr[6];             
  }
  else{ //Detección sindrome: 0000 o cualquier otro error
    Serial.print("Sindrome 0000");
    dc[0]=rr[0];
    dc[1]=rr[1];    
    dc[2]=rr[2];
    dc[3]=rr[3];
    dc[4]=rr[4];
    dc[5]=rr[5];   
    dc[6]=rr[6];             
  }
  digitalWrite(DC1,dc[0]);
  digitalWrite(DC2,dc[1]);
  digitalWrite(DC3,dc[2]);
  digitalWrite(DC4,dc[3]);
  digitalWrite(DC5,dc[4]);
  digitalWrite(DC6,dc[5]);  
  digitalWrite(DC7,dc[6]);   
}

void reiniciar(){

  int salida= R1;  
  for(int i = 0; i < 7; i++){
    digitalWrite(salida,LOW);
    salida = salida + 2;
  }
  salida= DC1;  
  for(int i = 0; i < 7; i++){
    digitalWrite(salida,LOW);
    salida++;
  }
  salida= S1;  
  for(int i = 0; i < 4; i++){
    digitalWrite(salida,LOW);
    salida = salida + 2;
  }
}


