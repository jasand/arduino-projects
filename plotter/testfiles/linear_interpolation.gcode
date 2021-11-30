G90        ;Linear interpolation testdata
G21
G28
G0 X40.0 Y10.0 Z0.5 F3000      ;Go to startpoint
G1 X100.0 Y130.0 Z-0.5 F3000
G1 X40.0 Y130.0 Z-0.5
G1 X100.0 Y10.0 Z-0.5 F1000
G1 X40.0 Y10.0 Z-0.5
G1 Z0.5
