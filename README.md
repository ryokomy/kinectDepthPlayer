# kinectDepthPlayer
play kinectV2 depth data recorded using https://github.com/RyoheiKomiyama/kinectDepthRecorder  
you can also saves ply files (pointcloud) to see them using application like meshlab.
[![](https://i.gyazo.com/0f4c83f5fb43bf2f37d34e85184a66f6.gif)](https://gyazo.com/0f4c83f5fb43bf2f37d34e85184a66f6)

## OS
mac

## How to use (just use this app)
1. clone repository
```
git clone https://github.com/RyoheiKomiyama/kinectDepthPlayer.git
```
2. copy your own kinect ir params file  
from **kinectDepthRecorder/bin/data/ir_params.json** to **kinectDepthRecorder/bin/data/ir_params.json**  
3. double click bin/kinectDepthPlayer

- 'l' key
choose recorded directory created using https://github.com/RyoheiKomiyama/kinectDepthRecorder
- space key
toggle play: start <-> stop
- 'p' key
togle view: depth image <-> pointcloud
- 's' key
save ply files corresponding to all files in chosen directory

## Develop
this code is developed using openframeworks.
