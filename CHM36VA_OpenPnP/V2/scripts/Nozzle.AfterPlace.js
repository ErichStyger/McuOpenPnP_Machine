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
   blow.actuate(true); /* BLOW on */
   java.lang.Thread.sleep(100);
   blow.actuate(false); /* BLOW off */
 } else {
   print('blow is NULL!');
 }
 print('*************** END Nozzle.AfterPlace.js ****************');

/*
// example to send a G code. G4 P100 ; wait 100 ms   G4P0.1 ; delay 0.1 sec
machine.getDriver().sendCommand("G4P100");
or
machine.getDriver().subDrivers.get(0).sendCommand("G4P100");
*/