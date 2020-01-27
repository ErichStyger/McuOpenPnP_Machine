/**
 * Called at the end of a job
 */
 print('*************** Script Job.Finished.js ****************');
 var vac = machine.getActuatorByName("VAC");
 if (vac) {
   print('vacuum pump off');
   vac.actuate(false);  /* VAC off */
 }
 print('*************** END Job.Finished.js ****************');
