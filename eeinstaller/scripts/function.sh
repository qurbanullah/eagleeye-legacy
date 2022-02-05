#!/bin/bash

########################################################################
# check if messages are to be printed using color
unset ALL_OFF BOLD BLUE GREEN RED YELLOW

	if tput setaf 0 &>/dev/null; then
		ALL_OFF="$(tput sgr0)"
		BOLD="$(tput bold)"
		BLUE="${BOLD}$(tput setaf 4)"
		GREEN="${BOLD}$(tput setaf 2)"
		RED="${BOLD}$(tput setaf 1)"
		YELLOW="${BOLD}$(tput setaf 3)"
	else
		ALL_OFF="\e[0m"
		BOLD="\e[1m"
		BLUE="${BOLD}\e[34m"
		GREEN="${BOLD}\e[32m"
		RED="${BOLD}\e[31m"
		YELLOW="${BOLD}\e[33m"
	fi

readonly ALL_OFF BOLD BLUE GREEN RED YELLOW

### SUBROUTINES ###
plain() {
	local mesg=$1; shift
	printf "${BOLD} ${mesg}${ALL_OFF}\n" "$@" >&2
}

success() {
	local mesg=$1; shift
	printf "${GREEN} $(gettext "Success:")${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@" >&2
}

message() {
	local mesg=$1; shift
	printf "${BLUE} $(gettext "Message:")${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@" >&2
}

warning() {
	local mesg=$1; shift
	printf "${YELLOW} $(gettext "Warning:")${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@" >&2
}

error() {
	local mesg=$1; shift
	printf "${RED} $(gettext "Error:")${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@" >&2
}

########################################################################
# function to exit script when command failed with spsific codes.
exitOnError() {
    exit_code=$1
    last_command=${@:2}
    if [ $exit_code -ne 0 ]; then
        >&2 echo "\"${last_command}\" command failed with exit code ${exit_code}."
        exit $exit_code
    fi
}

########################################################################
# exit on failed command and display an error message
# PROGNAME holdes the command being execued taken from the 
# first environmental variable i.e. $0
die(){
	PROGNAME=$(basename $0)
	echo "${PROGNAME}: ${1:-"Unknown Error"}" 1>&2
	exit 1
}
########################################################################
checkForRoot () {
	if test "x`id -u`" != "x0"; then
		return 1
	else
		return 0
	fi

}
########################################################################
command_exists () {
    command -v "$1" >/dev/null 2>&1
}
########################################################################
build_tools_exists () {
    if command_exists "gcc"; then
        message "Found usable gcc compiler!"
    else
        message "Your system does not have usable gcc compiler"
        exit 1
    fi
    if command_exists "make"; then
        message "make utility exists!"
    else
        message "Your system does not have make utility"
        exit 1
    fi

}
########################################################################
disable_ld_gold(){
	message "Using linker ld.bfd instead of ld.gold"
	# test wether the gold is the default linker
	# TODO
	local test_for_gold_linker=$(/usr/bin/ld --version | grep "GNU gold")
	if [ -z "${test_for_gold_linker}" ]; then
		message "Gold linker is already disabled"
	else
		rm /usr/bin/ld
		ln -svf ld.bfd /usr/bin/ld
	fi
}
########################################################################
enable_ld_gold(){
	message "Enabling Gold linker"
	# test wether the gold is the default linker
	# TODO
	local test_for_gold_linker=$(/usr/bin/ld --version | grep "GNU ld")
	if [ -z "${test_for_gold_linker}" ]; then
		message "Gold linker is already enabled"
	else
		rm /usr/bin/ld
		ln -svf ld.gold /usr/bin/ld
	fi
}

function_exists(){
	
    FUNCTION_NAME=${1}

    [ -z "$FUNCTION_NAME" ] && return 1

    declare -F "$FUNCTION_NAME" > /dev/null 2>&1

    return $?
}
########################################################################

##
#  usage : check_item_in_options_array( $needle, $haystack )
# return : 0   - enabled
#          1   - disabled
#          127 - not found
##
check_item_in_options_array() {	
	local needle=$1; shift
	local i opt
	for (( i = $#; i > 0; i-- )); do
		opt=${!i}
		if [[ $opt = "$needle" ]]; then
			# enabled
			#echo "enabled"
			return 0
		elif [[ $opt = "!$needle" ]]; then
			# disabled
			#echo "disabled"
			return 1
		fi
	done

	# not found
	return 127
}

########################################################################
# Checks to see if options are present in ee.conf or .avhb;
# .avhb options always take precedence.
#
#  usage : check_option( $option, $expected_val )
# return : 0   - matches expected
#          1   - does not match expected
#          127 - not found
##
check_option() {
	check_item_in_options_array "$1" ${options[@]}
	case $? in
		0) # assert enabled
			[[ $2 = y ]]
			return ;;
		1) # assert disabled
			[[ $2 = n ]]
			return ;;
	esac

	# fall back to makepkg.conf options
	check_item_in_options_array "$1" ${OPTIONS[@]}
	case $? in
		0) # assert enabled
			[[ $2 = y ]]
			return ;;
		1) # assert disabled
			[[ $2 = n ]]
			return ;;
	esac

	# not found
	return 127
}


##
# Check if option is present in BUILDENV
#
#  usage : check_buildenv( $option, $expected_val )
# return : 0   - matches expected
#          1   - does not match expected
#          127 - not found
##
check_buildenv() {
	check_item_in_options_array "$1" ${BUILDENV[@]}
	case $? in
		0) # assert enabled
			[[ $2 = "y" ]]
			return ;;
		1) # assert disabled
			[[ $2 = "n" ]]
			return ;;
	esac

	# not found
	return 127
}



##
#  usage : in_array( $needle, $haystack )
# return : 0 - found
#          1 - not found
##
check_item_in_array() {
	local needle=$1; shift
	local item
	for item in "$@"; do
		[[ $item = "$needle" ]] && return 0 # Found
	done
	return 1 # Not Found
}

extractPackage(){	
	# hack for the tmp dir as QT or polkit sets its permisssion to 'drwx------' of temp folder only.
	# while we want the permission of 'drwxr-xr-x'
	# othervise it will crash the system
	#chmod 755 "${TMPDIR}"
	# message "$(gettext "Command : /var/avouch/ee/cache/${1} -C ${2}")"  
	# go to root directory	
	#echo "tmpdir is ${TMPDIR}"
	# pushd "/var/avouch/ee/cache"
	#info "$(gettext "Package is : ${1}")"
		# extract package
		tar -Jxf "/var/avouch/ee/cache/${1}" -C "${2}" || exit 1 
		#cp -f "${TMPDIR}"/.PKGINFO "${PKGINFODIR}"
	# popd
}
########################################################################
checkConectivity(){
	ping -c 1 -q google.com >&/dev/null
	if [[ $? == 0 ]]; then
		return 0    
    else
        return 1
    fi
}
########################################################################
verifyUrl() {
    curl -f "${1}" >/dev/null 2>&1
    if [[ $? == 0 ]]; then
		return 0    
    else
        return 1
    fi
}
########################################################################
downloadPackage() {
	#info "$(gettext "Downloading package : ${URL}/${AVHVER}/packages/${ARCH}/${1}")"	   
    if verifyUrl "${PKGDLURL}${1}" ;then
		# echo "Package URL is : ${PKGDLURL}${1}"
		# message "$(gettext "Downloading package...")"  
		#curl --progress-bar "${DLURL}/${1}" -o "${cachedir}/${1}"
		curl  --progress-bar "${PKGDLURL}${1}" -o "${cachedir}/${1}"		
	else 
		message "$(gettext "Package is not available yet, please try later.")"
		exit 0
	fi
}
########################################################################
download_pkg_db(){
    #info "$(gettext "Downloading package database: https://packages.avouch.org/al01/pkginfo/${1}")"
    verify_url "${INFOURL}/${1}"
    message "$(gettext "Downloading package database...")"  
    curl  --progress-bar "${INFOURL}/${1}" -o "${cachedir}/${1}"	
}
########################################################################
getDatabaseDate(){
	if [ -f "/usr/bin/xmllint" ]; then
		# message "$(gettext "Name is ${1} and File is ${2}")" 
        local databaseDate=$(xmllint --xpath "//Packages/DatabaseDate/text()" "${1}" 2> /dev/null)
		echo "${databaseDate}"
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
}
########################################################################
getPackageName(){
	if [ -f "/usr/bin/xmllint" ]; then
		# message "$(gettext "Name is ${1} and File is ${2}")" 
        local packageName=$(xmllint --xpath "//${1}/Name/text()" "${2}" 2> /dev/null)
		echo "${packageName}"
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
}
########################################################################
getPackageVersion(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageVersion=$(xmllint --xpath "//${1}/Version/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageVersion}"
}
########################################################################
getPackageRelease(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageRelease=$(xmllint --xpath "//${1}/Release/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageRelease}"
}
########################################################################
getPackageDistribution(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageDistribution=$(xmllint --xpath "//${1}/Distribution/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageDistribution}"
}
########################################################################
getPackageArchitecture(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageArchitecture=$(xmllint --xpath "//${1}/Architecture/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageArchitecture}"
}
########################################################################
getPackageDescription(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageDescription=$(xmllint --xpath "//${1}/Description/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageDescription}"
}
########################################################################
getPackageSourceUrl(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageSourceUrl=$(xmllint --xpath "//${1}/SourceUrl/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageSourceUrl}"
}
########################################################################
getPackagePackageUrl(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packagePackageUrl=$(xmllint --xpath "//${1}/PackageUrl/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packagePackageUrl}"
}
########################################################################
getPackageLicense(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageLicense=$(xmllint --xpath "//${1}/License/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageLicense}"
}
########################################################################
getPackageDateCreated(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageDateCreated=$(xmllint --xpath "//${1}/DateCreated/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageDateCreated}"
}
########################################################################
getPackageProvides(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageDependancies=$(xmllint --xpath "//${1}/Provides/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageProvides}"
}
########################################################################
getPackageConflicts(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageConflicts=$(xmllint --xpath "//${1}/Conflicts/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageConflicts}"
}
########################################################################
getPackageGroups(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageGroups=$(xmllint --xpath "//${1}/Groups/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageGroups}"
}
########################################################################
getPackageDependancies(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageDependancies=$(xmllint --xpath "//${1}/Dependancies/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageDependancies}"
}
########################################################################
getPackageMaintainers(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageMaintainers=$(xmllint --xpath "//${1}/Maintainers/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageMaintainers}"
}
########################################################################
getPackageContributors(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageContributors=$(xmllint --xpath "//${1}/Contributors/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageContributors}"
}
########################################################################
getPackageInstalledSize(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageInstalledSize=$(xmllint --xpath "//${1}/InstalledSize/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageInstalledSize}"
}
########################################################################
getPackageFiles(){
	if [ -f "/usr/bin/xmllint" ]; then
		local packageFiles=$(xmllint --xpath "//${1}/Files/text()" "${2}" 2> /dev/null)
    else
		error "$(gettext "xmllint not found...")"
        exit 1
    fi
	echo "${packageFiles}"
}
########################################################################
#  usage : in_array( $needle, $haystack )
# return : 0 - found
#          1 - not found
##
verifyPackageExistance() {
	if [ -f ${packagesDatabase} ]; then
		local packageName=$(getPackageName ${1} ${packagesDatabase})
		# message "$(gettext "Package is : $packageName")"
		if [[ -n "${packageName}" ]]  && [[ "${packageName}" = "${1}" ]]; then
			return 0 # Found
		else
			return 1 # Not Found
		fi
	else
		return 127
	fi
}

########################################################################
cleanup(){
	# remove eagleEye spesific temporary directory
	rm -rf /usr/share/avouch/temp
	rm -rf "${TMPDIR}"
}

########################################################################
post(){	
	
	if [ -f "${TMPDIR}/usr/share/avouch/temp/INSTAL" ]; then
		source "${TMPDIR}/usr/share/avouch/temp/INSTAL"	
		if ( function_exists 'post_install' );then
			# run function prepare() from INSTALL file		
			#prepare 2>&1 | tee -a "${srcdir}/build.log"
			# pushd "/"
				post_install
			# popd
		else
			message "$(gettext "Post installation script not specified")"			
		fi	
	else 
		message "$(gettext "Post installation script not required")"
	fi
	
	cleanup	
}

checkIfPackageAlradyInstalled(){
	#if [ -f "${installedPackagesDatabase}" ]; then
	local databaseFile="/usr/share/avouch/pkginfo/${1}.xml"
	# message "$(gettext "Database is : $databaseFile")"
	if [ -f "${databaseFile}" ]; then
		local packageName=$(getPackageName ${1} ${databaseFile})
		# message "$(gettext "Package is : $packageName")"
		if [[ -n "${packageName}" ]]  && [[ "${packageName}" = "${1}" ]]; then
			return 0 # Found
		else
			return 1 # Not Found
		fi
	else
		return 127
	fi
}

checkIfSamePackageAlradyInstalled(){
	if [ -f "${packagesDatabase}" ] && [ -f "${installedPackagesDatabase}" ]; then
		local packageVersion=$(getPackageVersion ${1} ${packagesDatabase})
		local installedPackageVersion=$(getPackageVersion ${1} ${installedPackagesDatabase})
		# message "$(gettext "Package is : $packageName")"
		if [ -n "${packageVersion}" ]  && [  -n "${installedPackageVersion}" ] && [ "${packageVersion}" = "${installedPackageVersion}" ]; then
			return 0 # Found
		else
			return 1 # Not Found
		fi
	else
		return 127
	fi
}



# gzip info files

compress_info_files(){
	# compress the info files with gzip
	find "${pkgdir}" -type f \( -name "*.info" -o -name "*.info*" \) -exec gzip -9 {} \;
	# delete the usr/share/indo/dir file
	find "${pkgdir}" -name 'dir' -delete

}

get_file_contents() {	
    echo "$(cat ${1})" 
}