<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/AviSelvakumar/VexSim">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">VexSim</h3>

  <p align="center">
    A simple VEX Robotics C++ code simulator to speed up development
    <br />
    <a href="https://github.com/AviSelvakumar/VexSim"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/AviSelvakumar/VexSim">View Demo</a>
    &middot;
    <a href="https://github.com/AviSelvakumar/VexSim/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/AviSelvakumar/VexSim/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

Here's a blank template to get started. To avoid retyping too much info, do a search and replace with your text editor for the following: `AviSelvakumar`, `VexSim`, `twitter_handle`, `linkedin_username`, `email_client`, `email`, `VexSim`, `A simple VEX Robotics C++ code simulator to speed up development`, `project_license`

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites
This project requires CMake and MSYS2, along with some additional libraries.
You may run the following command in [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/windows-powershell/starting-windows-powershell?view=powershell-7.6#run-from-the-start-menu) to automatically install all prerequisites:
```powershell
winget install Kitware.CMake MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements --silent
C:\msys64\usr\bin\pacman.exe -S --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2
$p = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
if ($p -notlike "*ucrt64\bin*") { [System.Environment]::SetEnvironmentVariable("Path", $p + ";C:\msys64\ucrt64\bin", "Machine") }
```
Alternatively, 
1. Install [CMake](https://cmake.org/download/), check "Add to PATH" during install
2. Install [MSYS2](https://www.msys2.org/), then run the app and type in the following command to install all required libraries:
`pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2`
3. Add C:\msys64\ucrt64\bin to the Windows system PATH ([How to Add Executable to your PATH in Windows](https://medium.com/@kevinmarkvi/how-to-add-executables-to-your-path-in-windows-5ffa4ce61a53))

Run VexSimSetup.exe

### Installation

1. Download the installer from releases
2. Run the installer, and follow the prompts

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] Feature 1
- [ ] Feature 2
- [ ] Feature 3
    - [ ] Nested Feature

See the [open issues](https://github.com/AviSelvakumar/VexSim/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/AviSelvakumar/VexSim/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=AviSelvakumar/VexSim" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the project_license. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - [@avinashselvakumar](https://discord.com/users/897920475269853205) - [aviselvak@avinashselvakumar.me](mailto:aviselvak@avinashselvakumar.me)

Project Link: [https://github.com/AviSelvakumar/VexSim](https://github.com/AviSelvakumar/VexSim)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [PROS Team at Purdue](https://github.com/purduesigbots/pros)
* [LemLib](https://github.com/LemLib/LemLib)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/AviSelvakumar/VexSim.svg?style=for-the-badge
[contributors-url]: https://github.com/AviSelvakumar/VexSim/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/AviSelvakumar/VexSim.svg?style=for-the-badge
[forks-url]: https://github.com/AviSelvakumar/VexSim/network/members
[stars-shield]: https://img.shields.io/github/stars/AviSelvakumar/VexSim.svg?style=for-the-badge
[stars-url]: https://github.com/AviSelvakumar/VexSim/stargazers
[issues-shield]: https://img.shields.io/github/issues/AviSelvakumar/VexSim.svg?style=for-the-badge
[issues-url]: https://github.com/AviSelvakumar/VexSim/issues
[license-shield]: https://img.shields.io/github/license/AviSelvakumar/VexSim.svg?style=for-the-badge
[license-url]: https://github.com/AviSelvakumar/VexSim/blob/master/LICENSE.txt
[product-screenshot]: images/screenshot.png
