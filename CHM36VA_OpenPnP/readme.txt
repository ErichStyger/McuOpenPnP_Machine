machine configuration files for the CHM36VA with OpenPnP.

Commands:

Defaults:
	COMMAND_CONFIRM_REGEX	
		^ok.*
	CONNECT_COMMAND
		G21 ; Set millimeters mode
		G90 ; Set absolute positioning mode
		M82 ; Set absolute mode for extruder
	HOME_COMMAND
		G28 ; Home all axes
	MOVE_TO_COMMAND
		G0 {X:X%.4f} {Y:Y%.4f} {Z:Z%.4f} {Rotation:E%.4f} F{FeedRate:%.0f} ; Send standard Gcode move
		M400 ; Wait for moves to complete before returning

VAC
	ACTUATE_BOOLEAN_COMMAND		M80{True:8}{False:9} ; M808/809

BLOW
	ACTUATE_BOOLEAN_COMMAND		M81{True:2}{False:3} ; M812/813

UPLED
	ACTUATE_BOOLEAN_COMMAND		M81{True:0}{False:1} ; M810/811
	ACTUATE_DOUBLE_COMMAND		M810 S{IntegerValue}

DOWNLED
	ACTUATE_BOOLEAN_COMMAND		M81{True:4}{False:5} ; M814/815
	ACTUATE_DOUBLE_COMMAND		M814 S{IntegerValue}		

CAMSW (not used any more)
	ACTUATE_BOOLEAN_COMMAND		M81{True:8}{False:9} ; M818/819	

RFEEDER
	ACTUATE_BOOLEAN_COMMAND		T3G92E0G0E5	

Head: N1 is left, N2 is right

H1:N1VSENSE	
	ACTUATOR_READ_COMMAND		M105 ; read temp
	ACTUATOR_READ_REGEX		^ok V1:(?<Value>\d+\.\d+) /0.0 @0 V2:(?<n2vsense>\d+\.\d+) /0.0 @0
ok V1:125.1 /0.0 @0 V2:123.7 /0.0 @0 


H1:N2VSENSE
	ACTUATOR_READ_COMMAND		M105 ; read temp
	ACTUATOR_READ_REGEX		^ok V1:(?<n1vsense>\d+\.\d+) /0.0 @0 V2:(?<Value>\d+\.\d+) /0.0 @0
ok V1:125.1 /0.0 @0 V2:123.7 /0.0 @0 

H1:N1VAC	N1 vaccuum on/off
	ACTUATE_BOOLEAN_COMMAND		M80{True:0}{False:1} ; M800/801
On: RED led ON on soleonid

H1:N2VAC	N2 vaccuum on/off
	ACTUATE_BOOLEAN_COMMAND		M80{True:4}{False:5} ; M804/805
On: RED led ON on soleonid

H1:DRAGPIN (caution: do not enable it for more than a second, otherwise it will be damaged!)
	ACTUATE_BOOLEAN_COMMAND		
		M81{True:6}{False:7} ; M816/817
		G4P50 ; 100ms delay to deploy/retract
	ACTUATOR_READ_COMMAND		M119 ; endstop status
	ACTUATOR_READ_REGEX		^X_min:\d Y_min:\d Z_min:\d pins- \(X\)P\d.\d+:\d \(Y\)P\d.\d+:\d \(Z\)P\d.\d+:\d \(Z\)P\d.\d+:(?<Value>\d)

H1:ZMIN
	ACTUATOR_READ_COMMAND		M119 ; endstop status
	ACTUATOR_READ_REGEX		^X_min:\d Y_min:\d Z_min:\d pins- \(X\)P\d.\d+:\d \(Y\)P\d.\d+:\d \(Z\)P\d.\d+:(?<Value>\d) \(Z\)P\d.\d+:\d
X_min:0 Y_min:0 Z_min:1 pins- (X)P4.4:0 (Y)P4.3:0 (Z)P2.13:1 (Z)P4.2:1 

H1:LFEEDER
	ACTUATE_BOOLEAN_COMMAND		T2G92E0G0E5



H1VAC  On (RED) + VAC => Teil halten
H1VAC  Off(...) + BLOW => Teil blasen

Vac levels:
normal: 123
part on: >140