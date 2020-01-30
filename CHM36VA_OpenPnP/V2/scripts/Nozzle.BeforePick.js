/**
 * Controls before picking a part
 */
 print('*************** Script Nozzle.BeforePick.js ****************');
 var sol = machine.getActuatorByName("H1VAC");
 var vac = machine.getActuatorByName("VAC");

 print('SCRIPT: Before Picking......');
 if (vac) { /* vacuum pump */
   print('vac on');
   vac.actuate(true);  /* VAC on */
 } else {
   print('vac NULL!');
 }
 if (sol) { /* solenoid */
   print('solenoid for picking, led on');
   sol.actuate(true); /* LED on on solenoid ==> ready for picking */
 } else {
   print('solenoid NULL!');
 }
 print('*************** END Nozzle.BeforePick.js ****************');
