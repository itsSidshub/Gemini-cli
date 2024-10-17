## Converting gemini.c to an Executable

This guide explains how to compile gemini.c into an executable and install it in your local/bin directory.

**Prerequisites:**

* **C Compiler:** You need a C compiler like GCC.
* **Libjson:**  Download and install the libjson library. You can find it at [https://github.com/json-c/json-c](https://github.com/json-c/json-c).
* **Libcurl:** Download and install the libcurl library. You can find it at [https://curl.se/](https://curl.se/).

**Steps:**
0. get a api key form [https://aistudio.google.com/app/apikey](here)
   replace line no 7 with the api key

1. **Compile with libjson:**

    ```bash
    gcc gemini.c -o gemini -I/path/to/libjson/include -L/path/to/libjson/lib -ljson -lcurl
    ```

    * Replace `/path/to/libjson/include` with the actual path to the libjson include directory.
    * Replace `/path/to/libjson/lib` with the actual path to the libjson lib directory.

2. **Copy to local/bin:**

    ```bash
    cp gemini /local/bin
    ```

**Important:**

* You may need to adjust the paths to libjson and libcurl depending on where you installed them.
* If you encounter errors during compilation, ensure you have correctly configured the paths and installed the necessary libraries.

Now you should have a gemini executable in your local/bin directory, ready to use.
