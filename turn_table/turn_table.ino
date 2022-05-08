/*
	Copyright (c) [2019] [Orlin Dimitrov]

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma region Headers

#include "ApplicationConfiguration.h"

#include "ActionType.h"

#include <AccelStepper.h>

#pragma endregion

#pragma region Variables

/** @brief Pin stepper driver clock. */
int InputStateCW_g = HIGH;

/** @brief Pin stepper driver clock. */
int InputStateCCW_g = HIGH;

/** @brief Pin stepper driver clock. */
int StatusLEDState_g = LOW; 

/** @brief Pin stepper driver clock. */
Action_t Action_g = Action_t::NONE;

/** @brief Pin stepper driver clock. */
AccelStepper Stepper_g(AccelStepper::DRIVER, PIN_STEPPER_DRIVER_CLK, PIN_STEPPER_DRIVER_DIR);

#pragma endregion

void setup()
{
  pinMode(PIN_STATUS_LED, OUTPUT);
  digitalWrite(PIN_STATUS_LED, LOW);

  pinMode(PIN_CW, INPUT_PULLUP);
  pinMode(PIN_CCW, INPUT_PULLUP);

  Stepper_g.setMaxSpeed(MAX_SPEED);
  Stepper_g.setAcceleration(ACCELERATION);

}

void loop()
{
  if (Action_g == Action_t::NONE)
  {
    //
    Action_g = Action_t::WaitForInput;
  }

  // 
  else if (Action_g == Action_t::WaitForInput)
  {
    blink();

    //
    InputStateCW_g = digitalRead(PIN_CW);
    if (InputStateCW_g == LOW)
    {
      Stepper_g.move(SEGMENT_SIZE);
      Action_g = Action_t::Run;
      return;
    }
    //
    InputStateCCW_g = digitalRead(PIN_CCW);
    if (InputStateCCW_g == LOW)
    {
      Stepper_g.move(-SEGMENT_SIZE);
      Action_g = Action_t::Run;
      return;
    }
  }

  // 
  else if (Action_g == Action_t::Run)
  {
    if (Stepper_g.distanceToGo() == 0)
    {
      Action_g = Action_t::WaitToStop;
    } 
  }

  // 
  else if (Action_g == Action_t::WaitToStop)
  {

    Action_g = Action_t::WaitForInput;
  }

  // 
  Stepper_g.run();   
}

void blink()
{
      // if the LED is off turn it on and vice-versa:
    if (StatusLEDState_g == LOW)
    {
      StatusLEDState_g = HIGH;
    }
    else
    {
      StatusLEDState_g = LOW;
    }

    // set the LED with the StatusLEDState_g of the variable:
    digitalWrite(PIN_STATUS_LED, StatusLEDState_g);
}
