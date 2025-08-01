BUILD INSTRUCTIONS:
    Pre-reqs:
        Cmake ver. 3.14 or higher, link: https://cmake.org/download/
        gcc/Mingw installed and added to path, link: https://winlibs.com/
            -   Right under the "Help! I don't know which download to choose!", and click the download it here hyper link. It will give you a zip.
            -   In the windows start menu, type in "Edit the system environment variables" and enter the control panel link.
            -   Click on "Environment Variables..." button on the bottom left.
            -   On the new window that opens up, click on the user variable called "Path" on the top list of variables, and then click edit.
            -   Click "New" on the top right, and put in the path to the bin folder in the mingw folder.
            -   After that, you should be able to use gcc on your console.

    Building:
        -   Navigate to the build directory in your command window.
        -   type in "cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++".
        -   After that is done, type "cmake --build .".
        -   When that is done, there should be an app.exe in the build directory.

        *Note*
            If there are any changes in the code, you have to type in "cmake --build ." again for the changes to go through. YOU DO NOT HAVE TO COMPLETELY REBUILD.

-Izaac
