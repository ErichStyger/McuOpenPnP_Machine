/**
 * Controls lighting for the two cameras using two named actuators.
 */
 print('*************** Script Camera.AfterCapture.js ****************');

 var upCamLights = machine.getActuatorByName("UPLED");
 var downCamLights = machine.getActuatorByName("DOWNLED");
 
 /* turn off up looking camera lights, but always turn on down looking camera lights */
 if (upCamLights) {
   upCamLights.actuate(false);
 }
 if (downCamLights) {
   downCamLights.actuate(true); 
 }
 print('*************** END Camera.AfterCapture.js ****************');
