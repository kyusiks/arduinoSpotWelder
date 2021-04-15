int gvPinSensor  = A5; // 로터리
int gvPinButton  = A4; // 스팟버튼
int gvPinSpeaker = A3; // 스피커
int gvPinRelay   = A2; // 트랜스 연결 릴레이
int gvPinLedSelect[] = { 2, 3, 4, 5 }; // 자리선택 핀. 일의자리, 십의자리, 백의자리...순서로 넣는다. 배열의 수는 자릿수가 된다. 배열이2개면 두자리
int gvPinLedSeven[]  = { 6, 7, 8, 9, 10, 11, 12, 13 }; // a~g,dp5t 까지의 led 핀. 반드시 8자리여야한다.

int gvMax        = 99.00; // 최대 스팟시간 0~0.99초
int gvDelayTime  = 10; // 스팟 인가 대기시간(ms)

boolean gvReadyOk = true;

void setup() {
  Serial.begin(9600);
  pinMode(gvPinButton , INPUT_PULLUP);
  pinMode(gvPinSensor , INPUT );
  pinMode(gvPinRelay  , OUTPUT);
  pinMode(gvPinSpeaker, OUTPUT);

  digitalWrite(gvPinRelay, HIGH);
  setupLed();

  readySong();
}

void loop() {
  gvDelayTime = (int)(analogRead(gvPinSensor) / 1023.00 * gvMax * 10);
  setNumLed();

  if ( digitalRead(gvPinButton) == HIGH ) { // button depress
    if ( gvReadyOk == false ) gvReadyOk = true;
  } else {
    if ( gvReadyOk == false ) return;
    if ( gvDelayTime == 0 ) return; 
    gvReadyOk = false;
    busySong();
    digitalWrite(gvPinRelay, LOW);
    delay(gvDelayTime);
    digitalWrite(gvPinRelay, HIGH);
  }
}

void busySong() {
  tone(gvPinSpeaker, 1047, 50);
  delay(100);
  tone(gvPinSpeaker, 1047, 50);
}
void readySong() {
  tone(gvPinSpeaker, 262, 50);
  delay(100);
  tone(gvPinSpeaker, 294, 50);
  delay(100);
  tone(gvPinSpeaker, 330, 50);
}

// 세그먼트 파트
int gvLedLength = sizeof(gvPinLedSelect) / sizeof(gvPinLedSelect[0]); // 7세그먼트 자릿수
int gvLedNum[10][8] = {
  {0, 0, 0, 0, 0, 0, 1, 0}, //0
  {1, 0, 0, 1, 1, 1, 1, 0}, //1
  {0, 0, 1, 0, 0, 1, 0, 0}, //2
  {0, 0, 0, 0, 1, 1, 0, 0}, //3
  {1, 0, 0, 1, 1, 0, 0, 0}, //4
  {0, 1, 0, 0, 1, 0, 0, 0}, //5
  {0, 1, 0, 0, 0, 0, 0, 0}, //6
  {0, 0, 0, 1, 1, 1, 1, 0}, //7
  {0, 0, 0, 0, 0, 0, 0, 0}, //8
  {0, 0, 0, 0, 1, 0, 0, 0}  //9
};
void setupLed() {
  for ( int i = 0; i < gvLedLength; i++ ) { // 자릿수 1~n핀 출력모드로 세팅
    pinMode(gvPinLedSelect[i], OUTPUT);
  }
  for ( int i = 0; i < sizeof(gvPinLedSeven) / sizeof(gvPinLedSeven[0]); i++ ) { // 세그먼트 1~8핀 출력모드로 세팅
    pinMode(gvPinLedSeven[i], OUTPUT);
  }
}
void setNumLed() {
  int vNumber = gvDelayTime / 10;
  int vNowVal = 0;
  for ( int i = 0; i < gvLedLength; i++ ) {
    vNowVal = vNumber % 10;
    vNumber = vNumber / 10;
    setLedNumberOn(gvLedLength - i, vNowVal);
  }
}

void setLedNumberOn(int pPos, int pNumber) {
  for ( int i = 0; i < gvLedLength; i++ ) {
    digitalWrite(gvPinLedSelect[i], ( i == pPos - 1  ) ? HIGH : LOW);
  }
  for ( int j = 0; j < 8; j++ ) {
    digitalWrite(gvPinLedSeven[j], gvLedNum[pNumber][j]);
  }
  delay(5);
}
// 세그먼트 파트 끝
