# RBRHshift3.5
Richard Burns Rally H Shifter plugin v3.5 (Original code by Racer_S)

Added the option to select clutch bite point (how much % of the clutch needs to be pressed for allow gears to change). Originally Racer_S hardcoded it to 85% wich is too much in my opinion. So i moded the source code to let you decide how much.

Rally cars gearbox explanation:
In rally cars you can find various types of gearboxes. Mainly sequentials or h gated. Within H gated gearboxes, we have dogbox or sychromesh gearboxes. With dogbox gearboxes you don´t need to use clutch to shift, but with sychromesh you have to instead. We can find sychromesh gearboxes in low budget rally cars, so don´t expect to find them in Lancer Evos or similar. Read about the car you drive and find wich kind of gearbox it has. I personally use "require clutch" option with small a5 cars and alike.

Files modified:
  - shifter.h: Added bitepoint logic and ini file read and write compatibility for it.
  - TestPlugin.h: Chaged plugin menu to display new bitepoint option.

How to build it yourself:
You can build the dll yourself. Open the RBRTestPlugin.sln with Visual Studio 2022 and compile it. You must have a directory in C: called "C:\Games\Richard Burns Rally Original\Plugins\". There you will find the compiled dll wich will be called RBRTestPlugin.dll, rename it and you are ready to go.
