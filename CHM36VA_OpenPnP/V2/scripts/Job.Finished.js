/**
 * Called at the end of a job
 */
 var vac = machine.getActuatorByName("VAC");
 
 print('*************** Script Job.Finished.js ****************');
 if (vac) {
   print('vacuum pump off');
   vac.actuate(false);  /* VAC off */
 }
