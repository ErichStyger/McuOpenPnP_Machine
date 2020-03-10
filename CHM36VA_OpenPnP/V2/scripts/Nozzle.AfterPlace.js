/**
 * Controls AfterPlace
 */
 print('*************** Script Nozzle.AfterPlace.js ****************');
 var blow = machine.getActuatorByName("BLOW");
 var sol; 
 var name = nozzle.getName();
 var i, j = 0;

 /* print('Nozzle: ' + name); */
 if (name=='N1') {
   sol = machine.getActuatorByName("H1VAC");
 } else if (name=='N2') {
   sol = machine.getActuatorByName("H2VAC");
 } else {
   sol = null; /* unknown */
 }
 /* print('After Placing...'); */
 if (sol) { /* solenoid */
   /* print('solenoid for blowing, led off'); */
   sol.actuate(false); /* LED off on solenoid ==> ready for blow */
 } else {
   print('sol is NULL!');
 }
 if (blow) {
   /*print('blow on');*/
   blow.actuate(true); /* BLOW on */
   print('waiting....'); /* any better way? Thread.sleep() is not available? */
   for(i=0; i<200000; i++) {
     j++;
   }
   /*print('blow off');*/
   blow.actuate(false); /* BLOW off */
 } else {
   print('blow is NULL!');
 }
 print('*************** END Nozzle.AfterPlace.js ****************');
