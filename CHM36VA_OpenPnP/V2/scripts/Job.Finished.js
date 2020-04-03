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


// Job.Finished.js
//
// resets all parts to not placed

//print("### JOB FINISHED ###");

//var n = 0;
//for each (var boardlocation in job.getBoardLocations()) {
//  boardlocation.clearAllPlaced();
//  n++;
//}
//print(n + " Board(s) reset.");
