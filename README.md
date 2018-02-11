# DeepBlue for AMNH #HackTheDeep

This project is team [DeepBlue](https://github.com/bluecadet)'s contribution to the [AMNH HackTheDeep](https://github.com/amnh/HackTheDeep/) hackathon on Feb 10/11, 2018.

The main challenged we tackled is [The Eye of Maria](https://github.com/amnh/HackTheDeep/wiki/The-Eye-of-Maria). Our primary goal was to build the foundation for an app that could read and render the geo data provided in real time and provide an interactive interface to explore connections and facilitate research by eliminating the need to export visualizations offline. The secondary goal was to provide an accessible experience that's more approachable than some of the scientific tools currently used for the job.

In addition to the main challenge, the [Prototypes](Prototypes/) folder is a WIP snapshot of early prototypes for both [The Eye of Maria](https://github.com/amnh/HackTheDeep/wiki/The-Eye-of-Maria) and [Iron out the Kinks](https://github.com/amnh/HackTheDeep/wiki/Iron-Out-the-Kinks).

## Team

- [Adiel Fernandez](https://github.com/adielfernandez)
- [Ben Bojko](https://github.com/adielfernandez)
- [Devon Burgoyne](https://github.com/DevonBurgoyne)
- [Kevin Zakzsevski](https://github.com/kevinzak)

## Tech

The project is primarily built with [Cinder](https://github.com/cinder/Cinder/), written in C++/OpenGL and built on Windows using Visual Studio.

## Setup

### 1. Clone the repo and all submodules (make sure you have an [SSH key](https://github.com/settings/keys) set up).

```bash
# clone this repo recursively
git clone git@github.com:bluecadet/amnh-hackthedeep.git --recursive --depth=1
git submodule update --init --recursive
```

### 2. Build Cinder

- Open the Cinder solution at [Cinder/proj/vc2015/cinder.sln](Cinder/proj/vc2015/cinder.sln)
- In the top menu bar, select Build > Batch Build
- Check Debug for x64 and Release for x64
- Press Build

### 3. Build and Run the Project

- Open the project solution at [Challenges/TheEyeOfMaria/OceanViz/vc2015/OceanViz.sln](Challenges/TheEyeOfMaria/OceanViz/vc2015/OceanViz.sln)
- Select *Release* from the target dropdown at the top and click the green play button to build and run the app (or press `F5`)
