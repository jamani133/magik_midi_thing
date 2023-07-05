int vals[10];    //fader values
boolean button[8];//button values
int vels[18];   //velocity map
int oldvels[18];//magic jitter shit comparative values
boolean sk[18]; //magik jitter compensation

 void setup() {
  //Serial.begin(31250);  //for hardwarre midi
  Serial.begin(115200); //for hairless midi bridge                               //set baudrate
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);                                                             // set mux pins to output
  pinMode(4,OUTPUT);

  for(int i = 0; i < 8;i++){
    pinMode(i+5,INPUT_PULLUP);                                                   // set button pins to input_pullup
  }
}

void loop() {

  for(int a = 0; a < 8;a++){
    digitalWrite(2,bitRead(a,0));
    digitalWrite(3,bitRead(a,1));                                                // read out faders with multiplexer
    digitalWrite(4,bitRead(a,2));
    vals[a] = analogRead(A0);
  }
  vals[8] = analogRead(A1);                                                      // read out speeed & fade time faders
  vals[9] = analogRead(A2);


  for(int i = 0; i < 8;i++){
    button[i] = !digitalRead(i+5);                                               // read out buttons
  }


  for(int a = 0; a < 10;a++){
    vels[a] = map(vals[a],-5,1018,0,127);
    if(vels[a] < 0){vels[a] = 0;}          //value clamping                      // fader values into velociy map
    if(vels[a] > 127){vels[a] = 127;}
  }

  
  for(int a = 0; a < 8;a++){
    vels[a+10] = button[a]*127;                                                  // button values to velocity map 
  }

  for(int i = 0; i < 18; i++){                                                   // final proccessing loop
    
    if( !( vels[i] >= oldvels[i] - 1 && vels[i] <= oldvels[i] + 1) || sk[i]){    // magic jitter stuff
      
      noteOn(0x90,i+100,vels[i]);                                                // send notes
      if(sk[i] == true){sk[i]=false;}                                            // more gitter stuff
      if(vels[i] != oldvels[i]){sk[i]=true;}                                     // even more
      if(vels[i]< 2){vels[i]=0;}                                                 // low deadband 
      if(vels[i]> 125){vels[i]=127;}                                             // high deadband
        oldvels[i] = vels[i];                                                    // JITTER STUFF
      }
  }

  
  delay(20);                                                                     //slow down the entire thing, idk why, take it out if u want
  }

  void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);                                                           //definitly 100% original code thats DEFINITLY from some dude on the internet
  Serial.write(velocity);
}
