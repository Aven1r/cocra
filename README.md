# Cocra (Codeforces crawler)
Tool for parsing the problems from codeforces.com website and generate problemsets into markdown format

## 💻 Technologies
- C++
- [CMake](https://cmake.org/)
- [Vcpkg](https://vcpkg.io)
## ⚙️ Libraries used
- [CURL](https://github.com/curl/curl/)
- [LibXml2](https://github.com/GNOME/libxml2)
- [html2md](https://github.com/tim-gromeyer/html2md.git)
- [nlohmann-json](https://json.nlohmann.me/)
- [cpr](https://github.com/libcpr/cpr)

## ✨ Installation
### Requirements
To install and run the project, you need to install all libraries mentioned above (except for html2md) with [vcpkg](https://vcpkg.io) dependency manager
### Installation and launch of the project
Clone the project and move to the src directory:
```sh
git clone https://github.com/Aven1r/cocra.git
cd cocra/src
```
Start the build with CMake
```sh
cmake -DCMAKE_TOOLCHAIN_FILE=[pathToVcpkgToolchain] -B ./build
cmake –build ./build
```

## 🧑‍💻 Usage
After build process, the programs will be available for use from the ./src/bin folder
### `./ProblemParser` Parsing Program
Run with command line parameters
```sh
./ProblemParser ./path/to/saved/problems lang limit delay
```
Where 
- `./path/to/saved/problems-path` is the path where all problems will be stored 
- `lang` language of tasks (ru/eng)
- `limit` limit on the number of saved tasks (positive number; 0-no limit)
- `delay` delay between parsing tasks in milliseconds.

After execution, you can access the folder with tasks and the csv file with their attributes (name, rating, path to .md file)
### Program for generating a set of tasks `./SetGenerator`
Run with command line parameters
```sh
./SetGenerator ./path/to/problems/folder rating tag
```
Where
- `./path/to/saved/problems-path` is the path where all problems has been saved
- `rating` required task rating (can be a range, like 800-2000, or just a number. 0 - no rating restrictions)
- `tag` desired task tag (e.g. "math") (You may not write it)

After execution, the file `tagdate.md` will appear in the folder with tasks (where `tag` is the selected task tag, and `date` is the date of generation).

## Contributing
if you have any ideas to improve the project or its individual components, then write [here](https://t.me/avenircs) or make pull requests

## Project Team
- [Andrey Kadomtsev](https://github.com/Aven1r)
- [Vadim Soin](https://github.com/VadimS077)
