<?php
class os_boxes_facetapi_vocabulary extends os_boxes_default {

  /**
   * Create a new box.
   */
  protected function __construct() {
    parent::__construct();
    $this->title = t('Filter by taxonomy');
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['bundle'] = FALSE;
    $options['vocabularies'] = array();

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $bundle = $this->options['bundle'];
    $selected_vocab = $this->options['vocabularies'];

    if (!empty($form_state['values']['bundle'])) {
      // We have the bundle from the AJAX.
      $bundle = $form_state['values']['bundle'];
      $selected_vocab = 'all';
    }

    $form['bundle'] = array(
      '#type' => 'select',
      '#description' => t('Which bundle would you like to use.'),
      '#title' => t('Content type'),
      '#options' => os_get_bundles(array(OS_PUBLIC_APP, OS_PRIVATE_APP)),
      '#default_value' => $bundle,
      '#required' => TRUE,
      '#default_value' => $bundle,
      '#ajax' => array(
        'callback' => 'os_boxes_facetapi_vocabulary_ajax_callback',
        'wrapper' => 'vocab-wrapper',
      ),
    );

    $form['vocab_wrapper'] = array(
      '#prefix' => '<div id="vocab-wrapper">',
      '#suffix' => '</div>',
      '#parents' => array('vocab_wrapper'),
    );

    $vocabs = os_search_solr_get_bundle_vocabularies($bundle);
    $options = array('all' => t('All vocabularies')) + $vocabs;

    $form['vocab_wrapper']['vocabularies'] = array(
      '#title' => t('Select vocabularies'),
      '#type' => 'select',
      '#options' => $options,
      '#multiple' => TRUE,
      '#default_value' => $selected_vocab,
    );
    return $form;
  }

  /**
   * Facet widgets that in a non-solr search page must fake a search page in
   * order to show the facet. To do so we execute a "silent" search on solr
   * without showing the results.
   */
  public function call_empty_search_if_needed($delta) {
    if (!$feature = os_get_active_feature()) {
      // No active feature.
      return;
    }
    if (!$app_info = os_app_info($feature->name)) {
      // No app information available.
      return;
    }
    if (!$browse_page = reset(array_keys($app_info['browse_pages']))) {
      // No browse_pages.
      return;
    }

    // Execute the query.
    try {
      $page_id = 'browse_' . $browse_page;
      $search_page = apachesolr_search_page_load($page_id);
      $env_id = $search_page['env_id'];

      if (apachesolr_has_searched($env_id)) {
        // Search was executed already.
        return;
      }
      $solr = apachesolr_get_solr($env_id);

      $conditions = apachesolr_search_conditions_default($search_page);

      // Initializes params for empty query.
      $params = array(
        'spellcheck' => 'false',
        'fq' => isset($conditions['fq']) ? $conditions['fq'] : array(),
        'rows' => 1,
      );
      $context['page_id'] = $page_id;
      $context['search_type'] = 'apachesolr_search_show_facets';
      apachesolr_search_run_empty('apachesolr', $params, $search_page['search_path'], $solr, $context);
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    }
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();

    $searcher_name = os_search_solr_get_searcher_name();
    // Gets delta map
    if (!$map = facetapi_get_delta_map()) {
      return $block;
    }

    $delta = $block['delta'];

    // Filter by OG vocab.
    $string_og_vocab = facetapi_build_delta($searcher_name, 'block', 'sm_og_vocabulary');
    $delta_og_vocab = array_search($string_og_vocab, $map);

    // Verify the facet block has something to work on (means - it is in a solr
    // search page). If not - execute the empty solr search without showing
    // the results.
    $this->call_empty_search_if_needed($delta_og_vocab);

    $block = module_invoke('facetapi', 'block_view', $delta_og_vocab);
    $block['delta'] = $delta;

    // Set block subject/title from box configuration.
    $block['subject'] = $this->title;
    $block['title'] = $this->title;

    if (empty($block['content'])) {
      // Block content must be defined, even if it is empty, to avoid errors.
      // os_boxes_widget_submit() renders blocks into a 'dummy' region, but
      // gives errors in case the content is empty, hence the space.
      $block['content'] = ' ';
    }
    else {
      if (!empty($this->options['vocabularies'])) {
        // Get the list of selected vocabularies
        $selected = array();
        foreach ($this->options['vocabularies'] as $vocabulary_name => $value) {
          if ($value == "all") {
            $selected = array();
            break;
          }
          if ($value) {
            $selected[$vocabulary_name] = $value;
          }
        }
        // If no selection - show all. If there is selection - show only them.
        if (count($selected) > 0) {
          foreach (array_keys($block['content']['sm_og_vocabulary']) as $vocabulary_name) {
            if (empty($selected[$vocabulary_name])) {
              unset($block['content']['sm_og_vocabulary'][$vocabulary_name]);
            }
          }
        }
      }

      if (!empty($block['content']['sm_og_vocabulary'])) {
        // Sort by vocab's title.
        ksort($block['content']['sm_og_vocabulary']);
      }

      $block['content'] = render($block['content']);
    }
    return $block;
  }
}

/**
 * AJAX callback to attach the vocab to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function os_boxes_facetapi_vocabulary_ajax_callback($form, &$form_state) {
  return $form['options']['vocab_wrapper'];
}
