<h1 align="center" id="title">Blobber</h1>

<p id="description">Blobber is far from fully featured dungeon crawler video game inspired by games such as Wizardry and older Might and Magic series</p>

  
  
<h2>Features</h2>

Here're some of the project's barely functioning features:

*   ASWD + QE movement
*   Quick Save (F5) and Quick Load (F9) supported
*   Transition between multiple maps
*   Font parser which is used to display text messages
*   Basic inventory screen allowing the player to stash/take items from chest (purple square placehorlder)
*   Includes separate Map Editor (powered by imgui) project which allows creation and editing of new maps

<h2>Installation Steps:</h2>

<p>1. Requires Boost.Filesystem and Boost.Serialization libraries (not included in project)</p>

```
https://www.boost.org/
```

<p>2. Create new user environment variable named "Boost" pointing to the library directory</p>

<p>3. Download project from GitHub</p>

<p>4. Open Blobber.sln with MS Visual Studio and build the debug x64 solution</p>

<p>5. Executables are placed in Solution/x64/Debug directory</p>

  
  
<h2>Built with</h2>

Technologies used in the project:

*   SDL2.0 
*   Dear ImGui 
*   Boost filesystem and serialization libraries
