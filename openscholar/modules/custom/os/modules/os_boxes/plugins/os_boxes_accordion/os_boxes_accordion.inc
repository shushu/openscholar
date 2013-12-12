<?php

class os_boxes_accordion extends os_boxes_widget_collection {


  function render() {
    $output = parent::render();

    $content = array(
      '#prefix' => '<div class="accordion">',
      '#suffix' => '</div>',
      '#attached' => array(
        'library' => array(
          array('system', 'ui.accordion'),
        ),
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_accordion/os_boxes_accordion.render.js',
        )
      )
    );


    foreach ($this->options['widgets'] as $k => $w) {
      $block_output = $this->render_widget($k);
      unset($block_output['#theme_wrappers']);
      $content[$k] = array(
        '#weight' => $w['weight'],
        'title' => array(
          '#prefix' => '<h3>',
          '#suffix' => '</h3>',
          '#markup' => $w['title'],
        ),
        'body' => array(
          '#prefix' => '<div class="accordion-panel">',
          '#suffix' => '</div>',
          'output' => $block_output,
        )
      );
    }

    $output['content'] = drupal_render($content);
    return $output;
  }
}