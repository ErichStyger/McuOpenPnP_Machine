/**
 * Controls discard.
 */
 var vac = machine.getActuatorByName("VAC");
 var blow = machine.getActuatorByName("BLOW");
 var sol = machine.getActuatorByName("H1VAC");
 var i, j = 0;
 
 print('Discarding');
 if (vac) {
   print('vac off');
   vac.actuate(false);  /* VAC off */
 }
 if (sol) { /* solenoid */
   print('solenoid for blowing, led off');
   sol.actuate(false); /* LED off on solenoid ==> ready for blow */
 } else {
   print('sol NULL!');
 }
 if (blow) {
 	print('blow on');
	blow.actuate(true); /* BLOW on */
	print('waiting....');
	for(i=0; i<1000000; i++) {
   	//print('.');
		j++;
	}
   	print('blow off');
	blow.actuate(false); /* BLOW off */
 }
