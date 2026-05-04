#include <Servo.h>

// --- Pins ---
#define L1_R 13
#define L1_Y 12
#define L1_G 11
#define L2_R 4
#define L2_Y 3
#define L2_G 2
#define SERVO_PIN 9

Servo barrier;

volatile int state = 0;
volatile int tick = 0;
volatile int manual_timeout = 0; 
enum Mode {AUTO, STOP, TRAM, MANUAL};
volatile Mode mode = AUTO; 

// التوقيت
int GREEN1_T = 15;  
int GREEN2_T = 15;  
int YELLOW_T = 5;  

String cmd = "";
unsigned long last_char_time = 0; // لحساب وقت التوقف عن الكتابة

void setup() {
  pinMode(L1_R, OUTPUT); pinMode(L1_Y, OUTPUT); pinMode(L1_G, OUTPUT);
  pinMode(L2_R, OUTPUT); pinMode(L2_Y, OUTPUT); pinMode(L2_G, OUTPUT);

  Serial.begin(9600);
  barrier.attach(SERVO_PIN);
  barrier.write(0); 

  cli();
  TCCR2A = 0; TCCR2B = 0; TCNT2 = 0;
  OCR2A = 249; 
  TCCR2A |= (1 << WGM21); 
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); 
  TIMSK2 |= (1 << OCIE2A);
  sei();
  
  Serial.println("--- SYSTEM ONLINE ---");
  Serial.println("Just type STOP or START and wait 1 second...");
}

ISR(TIMER2_COMPA_vect) {
  static int ms_counter = 0;
  ms_counter++;
  if (ms_counter >= 6) { 
    ms_counter = 0;
    if (mode == MANUAL && manual_timeout > 0) {
        manual_timeout--;
        if (manual_timeout == 0) { mode = AUTO; tick = 0; state = 0; }
    }
    if (mode == STOP) {
      static int blink_c = 0;
      blink_c++;
      if (blink_c >= 5) {
        bool currentY = digitalRead(L1_Y);
        setLights(0, !currentY, 0, 0, !currentY, 0);
        blink_c = 0;
      }
    } 
    else if (mode == TRAM) setLights(1, 0, 0, 1, 0, 0); 
    else if (mode == AUTO) updateAutoLogic();
  }
}

void updateAutoLogic() {
  tick++;
  if (state == 0) { setLights(0,0,1, 1,0,0); if (tick >= GREEN1_T) { tick = 0; state = 1; } }
  else if (state == 1) { setLights(0,1,0, 1,0,0); if (tick >= YELLOW_T) { tick = 0; state = 2; } }
  else if (state == 2) { setLights(1,0,0, 0,0,1); if (tick >= GREEN2_T) { tick = 0; state = 3; } }
  else if (state == 3) { setLights(1,0,0, 0,1,0); if (tick >= YELLOW_T) { tick = 0; state = 0; } }
}

void loop() {
  if (mode == TRAM) barrier.write(90); else barrier.write(0);  

  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c); // Echo
    cmd += c;
    last_char_time = millis(); // تحديث وقت آخر حرف
  }

  // إذا مرت 800 ملي ثانية منذ آخر حرف، نفذ الأمر تلقائياً
  if (cmd.length() > 0 && (millis() - last_char_time > 800)) {
    Serial.println("\n[Auto-Executing...]");
    processCommand(cmd);
    cmd = ""; 
  }
}

void processCommand(String c) {
  c.trim();
  c.toUpperCase();
  if (c.indexOf("START") >= 0) { mode = AUTO; state = 0; tick = 0; Serial.println("MODE: AUTO"); }
  else if (c.indexOf("STOP") >= 0) { mode = STOP; Serial.println("MODE: STOP"); }
  else if (c.indexOf("TRAM ON") >= 0) { mode = TRAM; Serial.println("MODE: TRAM"); }
  else if (c.indexOf("TRAM OFF") >= 0) { mode = AUTO; state = 0; tick = 0; Serial.println("MODE: AUTO"); }
  else if (c.startsWith("SET_GREEN1")) {
    int val = c.substring(11).toInt();
    manual_timeout = val / 100; mode = MANUAL;
    setLights(0,0,1, 1,0,0); Serial.println("MODE: MANUAL");
  }
   else if (c.startsWith("SET_GREEN2")) {
   int val = c.substring(11).toInt();
   manual_timeout = val / 100; mode = MANUAL;
   setLights(1,0,0, 0,0,1); Serial.println("MODE: MANUAL");
  }
   else if (c.startsWith("SET_RED1")) {
   int val = c.substring(11).toInt();
   manual_timeout = val / 100; mode = MANUAL;
   setLights(1,0,0, 0,0,1); Serial.println("MODE: MANUAL");
  }
   else if (c.startsWith("SET_RED2")) {
   int val = c.substring(11).toInt();
   manual_timeout = val / 100; mode = MANUAL;
   setLights(0,0,1, 1,0,0); Serial.println("MODE: MANUAL");
  }
}

void setLights(int r1, int y1, int g1, int r2, int y2, int g2) {
  digitalWrite(L1_R, r1); digitalWrite(L1_Y, y1); digitalWrite(L1_G, g1);
  digitalWrite(L2_R, r2); digitalWrite(L2_Y, y2); digitalWrite(L2_G, g2);
}