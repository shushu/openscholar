<?php
/**
 * @file
 * iqss_scholars_front.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_front_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_header';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Front Page Header Text';
  $box->options = array(
    'text' => array(
      'value' => '<div align="center"><img src="/profiles/openscholar/themes/os_default/images/osfrontpg_logo.gif" class="os-title-image" alt="OpenScholar"></div><br> <strong>Welcome!</strong> Lorem ipsum dolor sit amet, consectetuer
          adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_header'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_create';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'Create A Site';
  $box->options = array(
    'text' => array(
      'value' => '<a href="/site/register"><img src="/profiles/openscholar/themes/os_default/images/osfrontpg_createsite.gif" class="os-title-image" alt="OpenScholar"></a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_create'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_main_support';
  $box->plugin_key = 'os_boxes_html';
  $box->title = 'Support';
  $box->description = 'Support';
  $box->options = array(
    'text' => array(
      'value' => '<div align="center"><img src="/profiles/openscholar/themes/os_default/images/osfrontpg_support.gif" class="os-title-image" alt="OpenScholar"></div><br>We have a very active community-style support site where questions, ideas, and problems can be searched for, posted, and replied to. <a href="http://community.openscholar.harvard.edu/openscholar">Visit the Community</a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_main_support'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_main_configure';
  $box->plugin_key = 'os_boxes_html';
  $box->title = 'Configure this Page';
  $box->description = 'Create A Site';
  $box->options = array(
    'text' => array(
      'value' => '<div align="center"><img src="/profiles/openscholar/themes/os_default/images/osfrontpg_configure.gif" class="os-title-image" alt="OpenScholar"></div><br>Want to change the design/theme of this page or change the content that appears? <a href="">Find out how.</a>',
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_main_configure'] = $box;

  $path = drupal_get_path('theme', 'os_default');
  $variables = array(
    'header' => t('Sample websites'),
    'images' => array(
      0 => array(
        'path' => "$path/images/osfrontpg_samplesite1.gif",
        'alt' => 'Sample site A',
        'title' => 'Sample site A',
      ),
      1 => array(
        'path' => "$path/images/osfrontpg_samplesite2.gif",
        'alt' => 'Sample site B',
        'title' => 'Sample site B',
      ),
      2 => array(
        'path' => "$path/images/osfrontpg_samplesite3.gif",
        'alt' => 'Sample site C',
        'title' => 'Sample site C',
      ),
    ),
  );
  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_front_sample';
  $box->plugin_key = 'os_boxes_html';
  $box->title = '';
  $box->description = 'List example site screenshots';
  $box->options = array(
    'text' => array(
      'value' => theme_os_front_boxcontent_sample($variables),
      'format' => 'filtered_html',
    ),
    'additional_classes' => '',
  );
  $export['os_front_sample'] = $box;

  return $export;
}
