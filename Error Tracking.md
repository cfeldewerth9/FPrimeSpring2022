# **Cross Compile Errors**
After creating my own repo for fprime files and changing the remote connection, I attempted
to re-generate and build the raspberrypi deployment.

The build files were succesfully generated.

This error ocurred while the build was running:

```
Scanning dependencies of target version
[ 72%] Generating version.hpp, __PHONY__
fatal: Not a valid object name HEAD
Traceback (most recent call last):
  File "/home/cfeldewerth/fprime/cmake/target/version/generate_version_header.py", line 68, in <module>
    main()
  File "/home/cfeldewerth/fprime/cmake/target/version/generate_version_header.py", line 64, in main
    create_version_file(fid)
  File "/home/cfeldewerth/fprime/cmake/target/version/generate_version_header.py", line 50, in create_version_file
    fid.write('static const char* VERSION = "%s";\n' % (get_version_str()))
  File "/home/cfeldewerth/fprime/cmake/target/version/generate_version_header.py", line 31, in get_version_str
    output = subprocess.check_output(["git", "describe", "--tags", "--always"])
  File "/usr/lib/python3.8/subprocess.py", line 415, in check_output
    return run(*popenargs, stdout=PIPE, timeout=timeout, check=True,
  File "/usr/lib/python3.8/subprocess.py", line 516, in run
    raise CalledProcessError(retcode, process.args,
subprocess.CalledProcessError: Command '['git', 'describe', '--tags', '--always']' returned non-zero exit status 128.
make[3]: *** [CMakeFiles/version.dir/build.make:62: version.hpp] Error 1
make[3]: *** Deleting file 'version.hpp'
make[2]: *** [CMakeFiles/Makefile2:2260: CMakeFiles/version.dir/all] Error 2
make[1]: *** [CMakeFiles/Makefile2:2240: CMakeFiles/Ref.dir/rule] Error 2
make: *** [Makefile:164: Ref] Error 2
[ERROR] CMake erred with return code 2
```
