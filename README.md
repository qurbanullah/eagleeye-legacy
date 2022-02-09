EagleEye - Readme
===================

Version 0.2.0

Package manager of Avouch Linux for creating, installing and updating packges.

https://avouch.org/packages/package-info/eagleeye

Summary
-------

EagleEye is intended as Package manager of Avouch Linux for creating, installing and updating packges.

Usage
---------

Usage: ee [OPTIONS...] <package_name>

	-h                              how usage information
	--help                          Show full help
	-b|--build                      Build packsge
	-bi|--build-install             Build and install package"
	-bl|--build-list                Build package from a list given in text file
	-bli|--build-list-install       Build package from a list given in text file and install built package
	-i|--install                    Install package
	-if|--install-file              Install package from file available in local computer
	-r|--remove                     Remove package permanently 
	-ud|--update-database           Update package database
	-up|--update-package            Update package 

Example
---------

# For installing a package rxecute the following command:-
ee -i <package_name>

# For removing a package rxecute the following command:-
ee -r <package_name>

# For building a package rxecute the following command:-
ee -b <package_name>

# For building and installing a package rxecute the following command:-
ee -bi <package_name>

# For building and installing multiple packages from a list file rxecute the following command:-
ee -bli <package_name>

Copyright
---------

Copyright © 2009 onwards -- Qurban Ullah

Certain libraries are copyrighted by their respective authors;

License
-------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 3, as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
