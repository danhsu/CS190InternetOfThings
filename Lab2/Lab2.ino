#define RED_PIN 2
#define YELLOW_PIN 3
#define GREEN_PIN 4
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3
#define RED_MILLIS 10000    // 10 second
#define YELLOW_MILLIS 2000  // 2 sec
#define GREEN_MILLIS 5000   // 5 sec

#define BUTTON_PIN 6
#define BUZZER_PIN 7

int tl_state; // Traffic light state.
unsigned long tl_timer; // Traffic light timer.

// BUTTON
int buttonState = LOW;
boolean clicked = false;

// BUZZER
unsigned long buzzer_timer = 0;
int frequency = 1000;     // Hz
boolean buzzSound = true; // turn on buzzer

// YELLOW light, NO buzz
void yellow_noBuzz()
{
  noTone(BUZZER_PIN);
}

// RED light, buzz 250 ms interval
void red_buzz()
{
  if (buzzSound)
  {
    if (millis() - buzzer_timer >= 250)
    {
      buzzer_timer = millis();
      tone(BUZZER_PIN, frequency);
      buzzSound = false;
    }
  }
  else // !buzzSound
  {
    if (millis() - buzzer_timer >= 250)
    {
      buzzer_timer = millis();
      noTone(BUZZER_PIN);
      buzzSound = true;
    }
  }
}

// GREEN light, buzz on 500 ms, buzz off 1500 ms
void green_buzz()
{
  if (buzzSound)
  {
    if (millis() - buzzer_timer >= 500)
    {
      buzzer_timer = millis();
      tone(BUZZER_PIN, frequency);
      buzzSound = false;
    }
  }
  else // !buzzSound
  {
    if (millis() - buzzer_timer >= 1500)
    {
      buzzer_timer = millis();
      noTone(BUZZER_PIN);
      buzzSound = true;
    }
  }
}

void setup() 
{
  Serial.begin(9600); 

  pinMode(RED_PIN,OUTPUT);
  pinMode(YELLOW_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN,OUTPUT);

  
  // Initial state for states and timers.
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
}


void loop() 
{
  // TL state machine.
  switch (tl_state) 
  {
    case RED_STATE:
    
      digitalWrite(RED_PIN, HIGH);
      red_buzz();
             
      if ( millis() >= tl_timer) 
      { 
        digitalWrite(RED_PIN, LOW);

        //Set timer for red-yellow state.
        tl_state = RED_YELLOW_STATE;
        tl_timer = millis() + YELLOW_MILLIS;
      }

    break;


    
    case RED_YELLOW_STATE: // FROM RED to RED YELLOW to GREEN

      digitalWrite(YELLOW_PIN, HIGH);
      yellow_noBuzz();  
      
      if ( millis() >= tl_timer) 
      {
        digitalWrite(YELLOW_PIN, LOW);
        tl_state = GREEN_STATE;
      }

    break;


    
    case YELLOW_STATE: // FROM GREEN TO YELLOW TO RED

      digitalWrite(YELLOW_PIN, HIGH);
      yellow_noBuzz();
          
      if ( millis() >= tl_timer )
      {
        digitalWrite(YELLOW_PIN, LOW);
        tl_state = RED_STATE;
        tl_timer = millis() + RED_MILLIS;
      }
    
    break;



    case GREEN_STATE:

      digitalWrite(GREEN_PIN, HIGH); 
      green_buzz();

      // button state, clicked, set green timer
      buttonState = digitalRead(BUTTON_PIN);  
      if (buttonState == HIGH)
      {
        clicked = true;
        tl_timer = millis() + GREEN_MILLIS;
      }
  
      // clicked, light stay for 5 more sec, then off
      if (clicked)
      {
          if (millis() >= tl_timer)
          {
                digitalWrite(GREEN_PIN, LOW);
                tl_state = YELLOW_STATE;
                tl_timer = millis() + YELLOW_MILLIS;
          
                clicked = false;
          }
      }
 
    break;
  }
  
}
