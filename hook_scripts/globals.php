<?php

ini_set ( "display_errros", 1 );
ini_set ( "error_reporting", E_ALL );

set_include_path ( get_include_path () . PATH_SEPARATOR . dirname ( __FILE__ ) );

/*****************************************************************************
 * Helper Functions
****************************************************************************/

/**
 * Generate a random binary safe string
 *
 * @return string
 */
function generate_random_string($length = 32) {
	return substr(base64_encode(openssl_random_pseudo_bytes($length)), 0, $length);
}

/**
 * Get environment variable
 * 
 * @return string
 */
function getEnvVar($name) {
	$envVar = getenv($name);
	if (! $envVar) {
		exit_with_error($name . ' env var undefined');
	}

	return $envVar;
}

/**
 * Exit with an error message and exit code 1
 *
 * This function exits the program and never returns
 *
 * @param string $error
 * @return void
 */
function exit_with_error($error) {
	echo "ERROR: $error\n";
	exit(1);
}

/**
 * Replace a set of values in a file
 *
 * If '$return' is true, will return the configured data instead of saving to a file
 *
 * @param string  $file
 * @param array   $translate
 * @param boolean $return
 * @return null | string
 */
function replace_values($file, $translate, $return = false) {
	if (($configData = file_get_contents($file)) === false)  {
		exit_with_error("Unable to load data from $file");
	}

	$configData = str_replace(array_keys($translate), array_values($translate), $configData);

	if (! $return) {
		if (! (file_put_contents($file, $configData))) {
			exit_with_error("Unable to write data to $file");
		}
	} else {
		return $configData;
	}
}

function strToHex($string){
	$string = 's:' . strlen($string) . ':"' . $string . '";';
    $hex = '';
    for ($i=0; $i<strlen($string); $i++){
        $ord = ord($string[$i]);
        $hexCode = dechex($ord);
        $hex .= substr('0'.$hexCode, -2);
    }
    return '0x' . strtolower($hex);
}