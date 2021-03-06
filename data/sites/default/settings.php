<?php

/**
 * Access control for update.php script.
 */
$update_free_access = FALSE;

/**
 * Salt for one-time login links and cancel links, form tokens, etc.
 */
$drupal_hash_salt = '@ZEND_RANDOM_HASH@';

/**
 * PHP settings:
 */
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);

ini_set('max_execution_time', 0);

/**
 * Fast 404 pages:
 */
$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html'] = '<html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

// Database configuration.
$databases['default']['default'] = array(
  'driver' => 'mysql',
  'database' => '@ZEND_DB_DBNAME@',
  'host' => '@ZEND_DB_HOST@',
  'username' => '@ZEND_DB_USERNAME@',
  'password' => '@ZEND_DB_PASSWORD@',
  'prefix' => '@ZEND_DB_TABLE_PREFIX@',
);

// Use Devel to redirect SMTP to file.
$conf['mail_system'] = array('default-system' => 'DevelMailLog', 'mimemail' => 'MimeMailSystem');

