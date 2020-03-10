/**
 * Controls before picking a part
 */
 print('*************** Script Nozzle.BeforePick.js ****************');
 var sol;
 var name = nozzle.getName();

 /* print('Nozzle: ' + name); */
 if (name=='N1') {
   sol = machine.getActuatorByName("H1VAC");
 } else if (name=='N2') {
   sol = machine.getActuatorByName("H2VAC");
 } else {
   sol = null; /* unknown */
   print('unknown nozzle!');
 } 
 var vac = machine.getActuatorByName("VAC");

 if (vac) { /* vacuum pump */
   vac.actuate(true);  /* VAC on */
 } else {
   print('vac NULL!');
 }
 if (sol) { /* solenoid */
   sol.actuate(true); /* LED is on back on the solenoid ==> ready for picking */
 } else {
   print('solenoid NULL!');
 }
 print('*************** END Nozzle.BeforePick.js ****************');
