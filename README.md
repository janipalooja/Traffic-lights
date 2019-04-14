# Traffic-lights
Traffic lights simulation program using finite state machine approach.
Mandatory to use:
-	Interrupts with buttons
-	BusOut for LEDs
-	Timeout or ticker for normal cycle

The Objective is:
-	Design a traffic light controller for the intersection of two equally busy one-way streets.
-	The goal is to maximize traffic flow, minimize waiting time at a red light, and avoid accidents.
-	Traffic light Controller using two sensors and 6 lights.
-	Two sensors detects the presence of cars in each direction.
-	Two traffic lights (Red,Yellow,Green) to control the traffic flow.

List heuristics describing how the traffic light is to operate:
-	If no cars are coming, stay in a GREEN state. (Cycle through all four states).
-	To change from GREEN to RED, implement a YELLOW light of 2 seconds.
-	GREEN lights will last at least 10 seconds.
-	If cars are only coming in one direction (Button is pressed), move to GREEN in that direction.
-	Normally cycle through all four states.
