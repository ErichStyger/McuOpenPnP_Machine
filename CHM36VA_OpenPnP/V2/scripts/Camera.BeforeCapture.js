/**
 * Controls lighting for the two cameras using two named actuators. The lights
 * for the up camera and down camera are turned on and off based on which camera
 * needs to capture.
 */

 /*
 print('*************** Script Camera.BeforeCapture.js ****************');
 var upCamLights = machine.getActuatorByName("UPLED");
 var downCamLights = machine.getActuatorByName("DOWNLED");

if (camera.looking == Packages.org.openpnp.spi.Camera.Looking.Up) {
	if (upCamLights) {
	    upCamLights.actuate(true);
	}
	if (downCamLights) {
    	    downCamLights.actuate(false);
	}
}
else if (camera.looking == Packages.org.openpnp.spi.Camera.Looking.Down) {
	if (upCamLights) {
	    upCamLights.actuate(false);
	}
	if (downCamLights) {
	    downCamLights.actuate(true);
	}
}
 print('*************** END Camera.BeforeCapture.js ****************');
*/

