<?php

class views_og_cache_plugin_cache_time extends views_plugin_cache_time {

  function option_definition() {
    $options = parent::option_definition();
    $options['og_access'] = array('default' => module_exists('og_access'));

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['og_access'] = array(
      '#type' => 'checkbox',
      '#title' => t('Check for access'),
      '#description' => t('When enabled, the group that is marked as the context, by OG-context will be checked to see if it is public or private. Depends on OG-access module.'),
      '#default_value' => $this->options['og_access'],
      '#disabled' => !module_exists('og_access'),
    );
  }

  /**
   * Get array with the unique data of the results of the view.
   */
  function get_results_key_data() {
    global $user;

    if (isset($this->_results_key)) {
      return $this->_results_key;
    }

    if (!$og_context = og_context()) {
      return;
    }

    $build_info = $this->view->build_info;
    foreach (array('query','count_query') as $index) {
      // If the default query back-end is used generate SQL query strings from
      // the query objects.
      if ($build_info[$index] instanceof SelectQueryInterface) {
        // There could be a view with altered query which can lead to caching
        // problems. Using the rendered query will give us a unique cache
        // identifier.
        $query = $this->view->build_info[$index];
        $arguments = $query->getArguments();
        $build_info[$index] = strtr($query, $arguments);
      }
    }

    $key_data = array(
      'build_info' => $build_info,
      'language' => $GLOBALS['language']->language,
      'base_url' => $GLOBALS['base_url'],
      'og_roles' => og_get_user_roles($og_context['group_type'], $og_context['gid']),
      'admin' => user_access('administer group'),
      'display_handler' => array(
        $this->view->display_handler->get_option('style_options'),
        $this->view->display_handler->get_option('pager'),
        $this->view->display_handler->get_option('arguments'),
      ),
    );

    if ($this->options['og_access']) {
      // Check if the group is private, and if so, change the cache type
      // to per-user, instead of per-group.
      $wrapper = entity_metadata_wrapper($og_context['group_type'], $og_context['gid']);
      if (!empty($wrapper->{OG_ACCESS_FIELD}) && $wrapper->{OG_ACCESS_FIELD}->value() == OG_CONTENT_ACCESS_PRIVATE) {
        $key_data['user'] = $user->uid;
      }
    }

    foreach (array('exposed_info', 'page', 'sort', 'order') as $key) {
      if (isset($_GET[$key])) {
        $key_data[$key] = $_GET[$key];
      }
    }

    return $key_data;
  }

  /**
   * Get array with the unique data of the output of the view.
   */
  function get_output_key_data() {
    global $user;

    if (isset($this->_output_key)) {
      return $this->_output_key;
    }

    if (!$og_context = og_context()) {
      return;
    }

    $key_data = array(
      'result' => $this->view->result,
      'theme' => $GLOBALS['theme'],
      'language' => $GLOBALS['language']->language,
      'base_url' => $GLOBALS['base_url'],
      'og_roles' => og_get_user_roles($og_context['group_type'], $og_context['gid']),
      'admin' => user_access('administer group'),
      'display_handler' => array(
        $this->view->display_handler->get_option('style_options'),
        $this->view->display_handler->get_option('pager'),
        $this->view->display_handler->get_option('arguments'),
      ),
    );

    if ($this->options['og_access']) {
      // Check if the group is private, and if so, change the cache type
      // to per-user, instead of per-group.
      $wrapper = entity_metadata_wrapper($og_context['group_type'], $og_context['gid']);
      if (!empty($wrapper->{OG_ACCESS_FIELD}) && $wrapper->{OG_ACCESS_FIELD}->value() == OG_CONTENT_ACCESS_PRIVATE) {
        $key_data['user'] = $user->uid;
      }
    }

    return $key_data;
  }

  function get_results_key() {
    if ($key_data = $this->get_results_key_data()) {
      $og_context = og_context();
      $this->_results_key = 'og:' . $og_context['group_type'] . ':' . $og_context['gid'] . ':' . $this->view->name . ':' . $this->display->id . ':results:' . md5(serialize($key_data));
    }
    return !empty($this->_results_key) ? $this->_results_key : NULL;
  }

  function get_output_key() {
    if ($key_data = $this->get_output_key_data()) {
      $og_context = og_context();
      $this->_output_key = 'og:' . $og_context['group_type'] . ':' . $og_context['gid'] . ':' . $this->view->name . ':' . $this->display->id . ':output:' . md5(serialize($key_data));
    }

    return !empty($this->_output_key) ? $this->_output_key : NULL;
  }
}

