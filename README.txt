Name: <ADD YOUR NAME AND EMAIL BEFORE TURNING IT IN>

Guild Name: Guild the Lily

Guild Members:
Zack Webb 
Kito Acosta 
Brandon Her

Assignment: Midterm Project Guild Wars

Project Description: 
This Program demonstrates our implmentation of several features and concepts:
  -including
    * bezier curves and surfaces
    * traversing bezier curves via arc length and equal parametric space
    * encapsulating objects into classes -> like camera
    * switching multiple views and camera types

Usage Instruction:
    Program initially starts in Arc Ball mode
    Camera controls
    In Arcball Mode:
      Left Click Drag moves the camera's postion about the target
      Left Click + Ctrl + drag zooms camera away or towards target

    In Free Cam Mode:
      Left click Drag changes the orientation of the camera. Updates the direction
      faced
      Pressing 'i' moves camera fowards
      Pressing 'k' moves camera backwards

    Mouse Controls
    Right click brings up the menu
    
    Key Controls
    'w' moves wandering hero forward
    's' moves wandering hero backwards
    'a' rotates hero left
    'd' rotates hero right

    'esc' 'q' exits the program

Compile Instruction:
    compile by running 'make' or 'gmake'
    run by entering './guildWars <name of input file>'

Notes/Bugs:
    Under some bezier curves heroes will not be able to correctly set heading
    possible to trap wandering hero in constant movement

File Input Description:
    An input file consists of a txt file that contain the name of revlevant .csv files
    ex: input.txt contains
          terrain_pts.csv
          pet_path.csv
          objects.csv

    one file contains the bezier patch and curve control points in following format
    terrain_pts.csv
    <number>
    <number, number, number>

    the first line for bezier patch side length
    the following are all the points that construct individual patches
    the first line for bezier curves is the amount of segments
    the following are all the control points that construct the curve

    in the same way as above the paths of pets are stored as
    lunyPath.csv
    <number>
    <number, number, number>
    
    for our objects we have the following format
    objects.csv
    <number> - total objects
    <string> - object type
    <number, number> - coordinates (from 0, 0 position of our terrain -> like a graph style)
    <number> - scale size
    ... - repeat

Work Distribution:
Zack Webb -
  Created classes
    Terrain
    Track
    point

  Set up the main and all the initial pieces
  Implemented the bezier curves and surfaces
  Implemented the drawing of a bezier patch -> terrain
  Implemented logic for finding normals to the bezier surface
  Implemented the logic for orientating and posotioning onto terrain
  Implemented the wandering hero
  Implemented the arc length movement
  Helped to find the correct heading of heroes -> for POV cam
  Designed the terrain_pts file format

Brandon Her - 
  Created classes
    Camera
    Hero
    vector
    
  Implemented the camera system with three modes (arcball, freecam, pov)
  Modified vector class to work with point class for certain functions
  Implemented FPS counter both calculations and text (bit mapping)
  Got hero names to appear over hero (stroke text)
  Implemented miniviewport for hero pov
  Created a hero class that all heroes inherit from in order to simplify processes
  Implemented parametric movement
  Created the menus

Kito Acosta - 
  Created classes
    Object

  Implemented the object class that allows for construction of predetermined objects, but with customizability
  Set up the current input file format unifying all .csv files into txt
  Implemented the object class to be able to attach on to the bezier surface

Time: 48+ hours

Lab Help: 
some of the labs helped -> 7

Fun:
fun in the beginning at 10, but as time went and were running out of time (busy schedules) 6.
