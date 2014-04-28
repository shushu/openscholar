<?php
$adminPassword 	= getEnvVar('ZS_ADMIN_PASSWORD');
$adminEmail 	= getEnvVar('ZS_ADMIN_EMAIL');
$siteName 		= getEnvVar('ZS_BLOG_NAME');
$siteUrl		= getEnvVar('ZS_SITE_URL');

$link = mysql_connect($dbHost, $dbUsername, $dbPassword);
if (!$link) {
	exit_with_error('Could not connect: ' . mysql_error());
}

if( ! mysql_select_db($dbName, $link)) {
	$query = "CREATE DATABASE " . $dbName . ";";
	$result = mysql_query($query, $link);
	if (! $result) {
		exit_with_error('Invalid query [' . $query . ']: ' . mysql_error());
	}
	mysql_select_db($dbName, $link);
}

$sqlFilePath = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'drupal_dump.sql';
if (! file_exists($sqlFilePath)) {
	exit_with_error("SQL file is missing: $sqlFilePath");
}

// Load Drupal's password API
require_once $appBaseDir . '/includes/bootstrap.inc';
require_once $appBaseDir . '/includes/password.inc';

// Replace values
$sqlFile = replace_values($sqlFilePath, array(
	"@ADMIN_PASSWORD@" => user_hash_password($adminPassword),
	"@ADMIN_EMAIL@" => strToHex($adminEmail),
	"@ZEND_SITE_NAME@" => strToHex($siteName),
	"@ZEND_SITE_URL@" => $siteUrl,
	"@CURR_DATE@" => time(),
	"@ZEND_TEMP_DIR@" => strToHex(sys_get_temp_dir()),
), true);

$sqlCommands = preg_split("/;[\r\n|\r|\n]/", $sqlFile);

foreach ($sqlCommands as $query) {
	$query = trim($query);
	if (! empty($query)) {
		$result = mysql_query($query , $link);
		if (!$result) {
			exit_with_error('Invalid query [' . $query . ']: ' . mysql_error());
		}
	}
}

mysql_close($link);
