/**
 * Controls before picking a part
 */
 print('*************** Script Nozzle.BeforePick.js ****************');
 var sol = machine.getActuatorByName("H1VAC");
 
 print('SCRIPT: Before Picking......');
 if (sol) { /* solenoid */
   print('solenoid for picking, led on');
   sol.actuate(true); /* LED on on solenoid ==> ready for picking */
 } else {
   print('solenoid NULL!');
 }
 print('*************** END Nozzle.BeforePick.js ****************');
