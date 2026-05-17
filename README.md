<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/AviSelvakumar/VexSim">
    <img src="https://ghrb.waren.build/banner?header=VexSim&subheader=A+simple+VEX+C%2B%2B+code+simulator+to+speed+up+development&bg=431586-9231A8&color=FFFFFF" alt="Logo">
  </a>

<h3 align="center">VexSim</h3>

  <p align="center">
    A simple VEX Robotics C++ code simulator to speed up development
    <br />
    <a href="https://github.com/AviSelvakumar/VexSim/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/AviSelvakumar/VexSim/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#configuration">Configuration</a></li>
        <li><a href="#running-the-simulator">Running the Simulator</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

---

<!-- ABOUT THE PROJECT -->
## About The Project

During my years competing in VEX, one of the biggest bottlenecks I kept running into was that build and programming couldn't happen at the same time. The programming team had to wait for the build team to finish before they could start testing anything meaningful, and trying to do both simultaneously just slowed everyone down.

I built VexSim during that idle time to close that gap — it lets programmers iterate on real PROS project code against a simulated robot while the physical build is still in progress.

> **Note:** VexSim is a starting point, not a replacement for on-robot testing. Code will almost always need further tuning once it's running on real hardware.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

VexSim requires CMake, MSYS2, and a few additional libraries. Choose whichever installation method works best for you:

#### Option 1 — Installer (Recommended)

Download the extention from [the VSCode Marketplace]([https://github.com/AviSelvakumar/VexSim/releases](https://marketplace.visualstudio.com/items?itemName=AvinashSelvakumar.vex-sim)), or search "VexSim" in the extensions tab. If prerequisites are missing, you'll be prompted to install them automatically.

#### Option 2 — PowerShell

Run the following in [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/windows-powershell/starting-windows-powershell?view=powershell-7.6#run-from-the-start-menu):

```powershell
winget install Kitware.CMake MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements --silent
C:\msys64\usr\bin\pacman.exe -S --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2
$p = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
if ($p -notlike "*ucrt64\bin*") { [System.Environment]::SetEnvironmentVariable("Path", $p + ";C:\msys64\ucrt64\bin", "Machine") }
```

#### Option 3 — Manual

1. Install [CMake](https://cmake.org/download/) — check "Add to PATH" during setup
2. Install [MSYS2](https://www.msys2.org/), then open it and run:
   ```
   pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2
   ```
3. Add `C:\msys64\ucrt64\bin` to your Windows system PATH ([guide](https://medium.com/@kevinmarkvi/how-to-add-executables-to-your-path-in-windows-5ffa4ce61a53))

### Installation

1. Download the latest installer from [releases](https://github.com/AviSelvakumar/VexSim/releases)
2. Run the installer and follow the prompts

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- USAGE -->
## Usage

Open a PROS project in VSCode. A **"Run Sim"** button will appear in the bottom-left status bar:

<img width="293" height="153" alt="Run Sim button" src="https://github.com/user-attachments/assets/09d3ae64-99bc-4b1d-99e3-b74ce24dc1c9" />

### Configuration

Before running the simulator for the first time, configure it to match your robot:

1. Right-click **"Run Sim"** → **Manage Extension**
2. Click the settings icon beneath the extension title:

<img width="1920" height="1020" alt="Extension settings icon" src="https://github.com/user-attachments/assets/ff972f8b-d459-430a-8fd7-3814b862c842" />

3. Select **Settings** from the menu:

<img width="1920" height="1020" alt="Settings menu" src="https://github.com/user-attachments/assets/f556f823-715f-4794-833c-657ce0cbac02" />

4. Set your drive motor ports, tracking wheel ports, and physical robot dimensions to match your code.

### Running the Simulator

Click **"Run Simulator"** to build your project and launch the simulator window.

#### Controls

| Key | Action |
|-----|--------|
| **F5** | Autonomous mode |
| **F6** | Opcontrol mode |
| **F7** | Disable |
| **ESC** | Quit |
| **W / S / A / D** | Left stick |
| **↑ / ↓ / ← / →** | Right stick |
| **L** | L1 |
| **;** | L2 |
| **P** | R1 |
| **[** | R2 |
| **1** | X button |
| **2** | B button |
| **3** | Y button |
| **4** | A button |

#### Supported Libraries

Currently, **the only supported library beyond PROS itself is [LemLib](https://github.com/LemLib/LemLib)**. Adding support for other libraries is straightforward — see [Contributing](#contributing) for instructions. If you add one, please submit a PR! If you'd like a specific library added, open an issue or reach out and I'll add it (allow about a week).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- ROADMAP -->
## Roadmap

- [ ] Support for more libraries
- [ ] Proper drivetrain gear ratio support
- [ ] Support for 5.5W motors
- [ ] Physical controller input

See the [open issues](https://github.com/AviSelvakumar/VexSim/issues) for a full list of proposed features and known bugs.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion, fork the repo and open a pull request, or open an issue with the tag `enhancement`. Don't forget to star the project!

1. Fork the project
2. Create your feature branch: `git checkout -b feature/AmazingFeature`
3. Commit your changes: `git commit -m 'Add some AmazingFeature'`
4. Push to the branch: `git push origin feature/AmazingFeature`
5. Open a pull request

### Adding Library Support

**1. Get the library source**

Clone or download it somewhere on your machine, e.g. `C:\MyLib-1.0.0`.

**2. Create a wrapper `CMakeLists.txt`**

Add a new directory to the project (e.g. `mylib/`) with a `CMakeLists.txt` modelled on `lemlib/CMakeLists.txt`:

```cmake
if(NOT DEFINED MYLIB_DIR OR MYLIB_DIR STREQUAL "")
    message(STATUS "MYLIB_DIR not set — MyLib will not be compiled.")
    return()
endif()

file(GLOB_RECURSE MYLIB_SOURCES "${MYLIB_DIR}/src/*.cpp")

add_library(mylib STATIC ${MYLIB_SOURCES})

target_include_directories(mylib PUBLIC "${MYLIB_DIR}/include")
target_include_directories(mylib BEFORE PRIVATE ${CMAKE_SOURCE_DIR}/stubs/include)

target_link_libraries(mylib PUBLIC pros_stubs Threads::Threads)
target_compile_features(mylib PUBLIC cxx_std_17)
target_compile_options(mylib PRIVATE -w)
```

**3. Register it in the top-level `CMakeLists.txt`**

```cmake
add_subdirectory(mylib)
```

**4. Link it to robot code**

In `robot/CMakeLists.txt`, add it alongside lemlib:

```cmake
target_link_libraries(robot_code PRIVATE mylib)
```

**5. Add the preset variable**

In `CMakePresets.json`, add `MYLIB_DIR` to the `msys2-ucrt64-lemlib` preset:

```json
"MYLIB_DIR": "${sourceDir}/mylib-src"
```

**6. Create a junction on your dev machine**

```powershell
New-Item -ItemType Junction -Path "C:\VEX Sim\mylib-src" -Target "C:\MyLib-1.0.0"
```

**7. Add to the installer**

In `installer/VexSim.iss`, add a `[Files]` entry:

```
Source: "C:\MyLib-1.0.0\*"; DestDir: "{app}\mylib-src"; Flags: ignoreversion recursesubdirs
```

**8. Add stubs if needed**

If the library calls PROS APIs not yet covered by the stubs, you'll get linker errors. Add the missing functions to the appropriate file in `stubs/src/` — they just need to return a sensible default value.

### Top Contributors

<a href="https://github.com/AviSelvakumar/VexSim/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=AviSelvakumar/VexSim" />
</a>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- LICENSE -->
## License

Distributed under the GPL v3 License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- CONTACT -->
## Contact

Avinash Selvakumar — [@avinashselvakumar](https://discord.com/users/897920475269853205) — [aviselvak@avinashselvakumar.me](mailto:aviselvak@avinashselvakumar.me)

Project Link: [https://github.com/AviSelvakumar/VexSim](https://github.com/AviSelvakumar/VexSim)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

- [PROS Team at Purdue](https://github.com/purduesigbots/pros)
- [LemLib](https://github.com/LemLib/LemLib)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Use of AI
Claude was used for the following tasks:
* Clarifying directions in README
* Adding comments to code to make it easier to contribute
* Light debugging (3 sessions)

ChatGPT was consulted on mathematical explanations for physics. It did not contribute any code. VSCode's built-in AI autocomplete was also used. GitHub Copilot generated commit messages for some commits.

I'm a strong believer in AI as a supplement to the developer rather than a full replacement. Programming is my passion, and I would never use AI to fully generate a project.

<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/AviSelvakumar/VexSim.svg?style=for-the-badge
[contributors-url]: https://github.com/AviSelvakumar/VexSim/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/AviSelvakumar/VexSim.svg?style=for-the-badge
[forks-url]: https://github.com/AviSelvakumar/VexSim/network/members
[stars-shield]: https://img.shields.io/github/stars/AviSelvakumar/VexSim.svg?style=for-the-badge
[stars-url]: https://github.com/AviSelvakumar/VexSim/stargazers
[issues-shield]: https://img.shields.io/github/issues/AviSelvakumar/VexSim.svg?style=for-the-badge
[issues-url]: https://github.com/AviSelvakumar/VexSim/issues
[license-shield]: https://img.shields.io/github/license/AviSelvakumar/VexSim.svg?style=for-the-badge
[license-url]: https://github.com/AviSelvakumar/VexSim/blob/master/LICENSE
