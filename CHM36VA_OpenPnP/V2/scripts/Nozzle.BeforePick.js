/**
 * Controls before picking a part
 */
 print('*************** Script Nozzle.BeforePick.js ****************');
 var sol;
 var name = nozzle.getName();

 print('Nozzle: ' + name);
 if (name=='N1') {
   sol = machine.getActuatorByName("H1VAC");
   print('using H1VAC');
 } else if (name=='N2') {
   sol = machine.getActuatorByName("H2VAC");
   print('using H2VAC');
 } else {
   sol = null; /* unknown */
   print('unknown nozzle!');
} 
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
