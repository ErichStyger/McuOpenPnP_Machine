/**
 * Controls discard.
 */
 var vac = machine.getActuatorByName("VAC");
 var blow = machine.getActuatorByName("BLOW");
 
 if (vac) {
	 vac.actuate(false);  /* VAC off */
 }
 if (blow) {
	 blow.actuate(true); /* BLOW on */
	/* wait */
	 blow.actuate(false); /* BLOW off */
 }
